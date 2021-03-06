/*
 * SearchEngine.cpp
 *
 *  Created on: Apr 10, 2020
 *      Author: mad
 */

#include <vnx/search/SearchEngine.h>
#include <vnx/search/Util.h>
#include <vnx/search/UrlIndex.hxx>
#include <vnx/search/PageIndex.hxx>

#include <url.h>
#include <omp.h>
#include <jemalloc/jemalloc.h>
#include <cmath>
#include <chrono>
#include <algorithm>


namespace vnx {
namespace search {

SearchEngine::SearchEngine(const std::string& _vnx_name)
	:	SearchEngineBase(_vnx_name)
{
	input_page_info = vnx_name + ".page_info.updates";
	
	protocols.push_back("http");
	protocols.push_back("https");
}

void SearchEngine::init()
{
	private_addr = Hash64::rand();
	vnx::open_pipe(private_addr, this, UNLIMITED, UNLIMITED);
	vnx::open_pipe(vnx_name, this, 1000, UNLIMITED, PRIORITY_HIGH);
}

void SearchEngine::main()
{
	if(info_commit_interval > word_commit_interval) {
		throw std::logic_error("info_commit_interval > word_commit_interval");
	}
	subscribe(input_page_info, 100, 1000);
	
	protocols = unique(protocols);
	
	module_page_info = new keyvalue::Server("PageInfo");
	module_page_info->collection = "page_info";
	module_page_info->update_topic = input_page_info;
	module_page_info.start();
	
	module_word_context = new keyvalue::Server("WordContext");
	module_word_context->collection = "word_context";
	module_word_context.start();
	
	module_word_array = new keyvalue::Server("WordArray");
	module_word_array->collection = "word_array";
	module_word_array.start();
	
	url_index_stream = std::make_shared<Stream>(url_index_server);
	page_info_stream = std::make_shared<Stream>(module_page_info.get_name());
	page_index_stream = std::make_shared<Stream>(page_index_server);
	page_content_stream = std::make_shared<Stream>(page_content_server);
	word_context_stream = std::make_shared<Stream>(module_word_context.get_name());
	
	url_index_stream->connect(this, 100, 1000);
	page_info_stream->connect(this, 100, 1000);
	page_index_stream->connect(this, 100, 1000);
	page_content_stream->connect(this, 100, 1000);
	word_context_stream->connect(this, 100, 1000);
	
	search_async = std::make_shared<SearchEngineAsyncClient>(private_addr);
	page_info_async = std::make_shared<keyvalue::ServerAsyncClient>(module_page_info.get_name());
	word_context_async = std::make_shared<keyvalue::ServerAsyncClient>(module_word_context.get_name());
	word_array_async = std::make_shared<keyvalue::ServerAsyncClient>(module_word_array.get_name());
	url_index_async = std::make_shared<keyvalue::ServerAsyncClient>(url_index_server);
	page_index_async = std::make_shared<keyvalue::ServerAsyncClient>(page_index_server);
	page_content_async = std::make_shared<keyvalue::ServerAsyncClient>(page_content_server);
	
	add_async_client(search_async);
	add_async_client(page_info_async);
	add_async_client(word_context_async);
	add_async_client(word_array_async);
	add_async_client(url_index_async);
	add_async_client(page_index_async);
	add_async_client(page_content_async);
	
	set_timer_millis(queue_interval_ms, std::bind(&SearchEngine::check_queues, this));
	set_timer_millis(stats_interval_ms, std::bind(&SearchEngine::print_stats, this));
	set_timer_millis(600 * 1000, std::bind(&SearchEngine::write_info, this));
	
	page_info_async->sync_all_private(page_info_stream->get_src_mac());
	
	update_threads = std::make_shared<ThreadPool>(num_update_threads);
	
	Super::main();
	
	update_threads->close();
}

void SearchEngine::get_domain_info_async(	const std::string& host,
											const int32_t& limit,
											const uint32_t& offset,
											const request_id_t& req_id) const
{
	Object result;
	const auto iter = domain_map.find(host);
	if(iter != domain_map.end()) {
		if(const auto* domain = find_domain(iter->second)) {
			std::vector<std::pair<std::pair<uint32_t, uint32_t>, const page_t*>> sorted;
			for(const auto page_id : domain->pages) {
				if(const auto* page = find_page(page_id)) {
					sorted.emplace_back(std::make_pair(page->reverse_domains, page->reverse_links), page);
				}
			}
			std::sort(sorted.begin(), sorted.end(),
					std::greater<std::pair<std::pair<uint32_t, uint32_t>, const page_t*>>());
			
			std::vector<std::string> pages;
			for(int i = 0; i < limit; ++i) {
				const size_t k = offset + i;
				if(k < sorted.size()) {
					const auto* page = sorted[k].second;
					pages.push_back(page->url_key.str());
				}
			}
			result["host"] = host;
			result["num_pages"] = sorted.size();
			result["pages"] = pages;
		}
	}
	get_domain_info_async_return(req_id, result);
}

void SearchEngine::get_page_info_async(const std::string& url_key, const request_id_t& req_id) const
{
	page_info_async->get_value(Variant(url_key),
		[this, req_id](std::shared_ptr<const keyvalue::Entry> entry) {
			Object result;
			if(auto info = std::dynamic_pointer_cast<const PageInfo>(entry->value)) {
				result["is_deleted"] = info->is_deleted;
				result["rank_value"] = info->rank_value;
				result["last_updated"] = info->last_updated;
				result["words"] = info->words.size();
				result["links"] = info->links.size();
				result["reverse_links"] = info->reverse_links.size();
				result["reverse_domains"] = info->reverse_domains.size();
				if(const auto* page = find_page(info->id)) {
					result["id"] = page->id;
					result["url"] = page->get_url();
					result["rank_value"] = page->rank_value;
					result["last_modified"] = page->last_modified;
					result["first_seen"] = page->first_seen;
					result["next_rank_update"] = page->next_rank_update;
					if(const auto* domain = find_domain(page->domain_id)) {
						result["domain"] = domain->host.str();
					}
				}
			}
			get_page_info_async_return(req_id, result);
		},
		std::bind(&SearchEngine::vnx_async_return_ex, this, req_id, std::placeholders::_1));
}

void SearchEngine::get_page_entries_async(const std::vector<uint32_t>& page_ids, const vnx::request_id_t& req_id) const
{
	std::vector<page_entry_t> result(page_ids.size());
	for(size_t i = 0; i < page_ids.size(); ++i) {
		if(const auto* page = find_page(page_ids[i])) {
			page_entry_t entry;
			entry.id = page->id;
			entry.rank_value = page->rank_value;
			entry.url = page->get_url();
			result[i] = std::move(entry);
		}
	}
	get_page_entries_async_return(req_id, result);
}

void SearchEngine::get_page_ranks_async(const std::vector<std::string>& url_keys,
										const vnx::bool_t& direct,
										const request_id_t& req_id) const
{
	std::vector<float> result;
	result.resize(url_keys.size());
	for(size_t i = 0; i < url_keys.size(); ++i) {
		if(auto* page = find_page_url(url_keys[i])) {
			result[i] = direct ? page->reverse_domains : page->rank_value;
		}
	}
	get_page_ranks_async_return(req_id, result);
}

std::vector<Object> SearchEngine::get_domain_list(const int32_t& limit, const uint32_t& offset) const
{
	std::vector<std::pair<size_t, const domain_t*>> sorted;
	for(const auto& entry : domain_index) {
		const auto& domain = entry.second;
		sorted.emplace_back(domain.pages.size(), &domain);
	}
	std::sort(sorted.begin(), sorted.end(), std::greater<std::pair<size_t, const domain_t*>>());
	
	std::vector<Object> result;
	for(int i = 0; i < limit; ++i) {
		const size_t k = offset + i;
		if(k < sorted.size()) {
			Object info;
			const auto* domain = sorted[k].second;
			info["host"] = domain->host.str();
			info["num_pages"] = domain->pages.size();
			result.push_back(info);
		}
	}
	return result;
}

void SearchEngine::get_page_ranking_async(	const int32_t& limit, const uint32_t& offset,
											const request_id_t& req_id) const
{
	std::vector<std::pair<std::string, float>> result;
	uint32_t i = 0;
	for(const auto& entry : page_ranking) {
		if(const auto* page = find_page(entry.second)) {
			if(i++ >= offset) {
				result.emplace_back(page->url_key.str(), entry.first);
				if(result.size() >= size_t(limit)) {
					break;
				}
			}
		}
	}
	get_page_ranking_async_return(req_id, result);
}

void SearchEngine::get_word_context_async(	const std::string& word, const int32_t& limit, const uint32_t& offset,
											const request_id_t& req_id) const
{
	word_context_async->get_value(Variant(word),
		[this, limit, offset, req_id](std::shared_ptr<const keyvalue::Entry> entry) {
			std::vector<std::pair<std::string, float>> result;
			if(auto context = std::dynamic_pointer_cast<const WordContext>(entry->value)) {
				for(int i = 0; i < limit; ++i) {
					const size_t k = offset + i;
					if(k < context->pages.size()) {
						const auto& entry = context->pages[k];
						if(const auto* page = find_page(entry.first)) {
							result.emplace_back(page->url_key.str(), entry.second);
						}
					}
				}
			}
			get_word_context_async_return(req_id, result);
		},
		std::bind(&SearchEngine::vnx_async_return_ex, this, req_id, std::placeholders::_1));
}

void SearchEngine::reverse_lookup_async(const std::string& url_key, const int32_t& limit, const uint32_t& offset,
										const request_id_t& req_id) const
{
	page_info_async->get_value(Variant(url_key),
		[this, limit, offset, req_id](std::shared_ptr<const keyvalue::Entry> entry) {
			std::vector<std::pair<std::string, float>> sorted;
			if(auto info = std::dynamic_pointer_cast<const PageInfo>(entry->value)) {
				for(const auto& link : info->reverse_links) {
					if(const auto* parent = find_page_url(link.url_key)) {
						sorted.emplace_back(parent->url_key.str(), parent->rank_value);
					}
				}
			}
			std::sort(sorted.begin(), sorted.end(),
					[] (const std::pair<std::string, float>& A, const std::pair<std::string, float>& B) -> bool {
						return A.second > B.second;
					});
			reverse_lookup_async_return(req_id, get_vector_range(sorted, limit, offset));
		},
		std::bind(&SearchEngine::vnx_async_return_ex, this, req_id, std::placeholders::_1));
}

void SearchEngine::reverse_domain_lookup_async(	const std::string& url_key, const int32_t& limit, const uint32_t& offset,
												const request_id_t& req_id) const
{
	page_info_async->get_value(Variant(url_key),
		[this, limit, offset, req_id](std::shared_ptr<const keyvalue::Entry> entry) {
			std::vector<std::pair<std::string, uint32_t>> sorted;
			if(auto info = std::dynamic_pointer_cast<const PageInfo>(entry->value)) {
				for(const auto& entry : info->reverse_domains) {
					sorted.emplace_back(entry);
				}
			}
			std::sort(sorted.begin(), sorted.end(),
					[] (const std::pair<std::string, uint32_t>& A, const std::pair<std::string, uint32_t>& B) -> bool {
						return A.second > B.second;
					});
			reverse_domain_lookup_async_return(req_id, get_vector_range(sorted, limit, offset));
		},
		std::bind(&SearchEngine::vnx_async_return_ex, this, req_id, std::placeholders::_1));
}

std::vector<std::pair<std::string, uint32_t>> SearchEngine::suggest_words(const std::string& prefix, const int32_t& limit) const
{
	std::vector<std::pair<std::string, uint32_t>> result;
	for(auto it = word_suggest_map.lower_bound(prefix); it != word_suggest_map.end() && result.size() < size_t(limit); ++it) {
		auto iter = word_index.find(it->second);
		if(iter != word_index.end()) {
			const auto word = iter->second.value.str();
			if(word.find_first_of(bad_word_chars) == std::string::npos) {
				result.emplace_back(word, iter->second.num_pages);
			}
		}
	}
	return result;
}

SearchEngine::page_t* SearchEngine::find_page(uint32_t page_id)
{
	const auto iter = page_index.find(page_id);
	if(iter != page_index.end()) {
		return &iter->second;
	}
	return nullptr;
}

const SearchEngine::page_t* SearchEngine::find_page(uint32_t page_id) const
{
	const auto iter = page_index.find(page_id);
	if(iter != page_index.end()) {
		return &iter->second;
	}
	return nullptr;
}

template<typename T>
SearchEngine::page_t* SearchEngine::find_page_url(const T& url_key)
{
	const auto iter = page_map.find(url_key);
	if(iter != page_map.end()) {
		return find_page(iter->second);
	}
	return nullptr;
}

template<typename T>
const SearchEngine::page_t* SearchEngine::find_page_url(const T& url_key) const
{
	const auto iter = page_map.find(url_key);
	if(iter != page_map.end()) {
		return find_page(iter->second);
	}
	return nullptr;
}

template<typename T>
uint32_t SearchEngine::find_word_id(const T& word) const
{
	const auto iter = word_map.find(word);
	if(iter != word_map.end()) {
		return iter->second;
	}
	return 0;
}

const SearchEngine::word_t* SearchEngine::find_word(uint32_t word_id) const
{
	const auto iter = word_index.find(word_id);
	if(iter != word_index.end()) {
		return &iter->second;
	}
	return nullptr;
}

uint32_t SearchEngine::add_word(const std::string& word)
{
	const auto iter = word_map.find(word);
	if(iter == word_map.end()) {
		const auto id = next_word_id++;
		word_map[word] = id;
		
		auto& index = word_index[id];
		index.id = id;
		index.value = word;
		return id;
	}
	return iter->second;
}

template<typename T>
SearchEngine::domain_t& SearchEngine::get_domain(const T& host)
{
	const auto iter = domain_map.find(host);
	if(iter != domain_map.end()) {
		return domain_index[iter->second];
	}
	else {
		const auto id = next_domain_id++;
		domain_map[host] = id;
		
		auto& domain = domain_index[id];
		domain.id = id;
		domain.host = host;
		return domain;
	}
}

const SearchEngine::domain_t* SearchEngine::find_domain(uint32_t domain_id) const
{
	const auto iter = domain_index.find(domain_id);
	if(iter != domain_index.end()) {
		return &iter->second;
	}
	return 0;
}

std::shared_ptr<SearchEngine::info_cache_t> SearchEngine::get_info_cache(const std::string& url_key)
{
	auto& cache = info_cache[url_key];
	if(!cache) {
		cache = std::make_shared<info_cache_t>();
		cache->url_key = url_key;
		info_queue.emplace(vnx::get_wall_time_micros(), cache);
	}
	return cache;
}

std::shared_ptr<SearchEngine::word_cache_t> SearchEngine::get_word_cache(uint32_t word_id)
{
	auto& cache = word_cache[word_id];
	if(!cache) {
		cache = std::make_shared<word_cache_t>();
		cache->word_id = word_id;
		const int64_t rand_offset = (::rand() * int64_t(word_commit_interval)) / RAND_MAX;
		word_queue.emplace(vnx::get_wall_time_seconds() + rand_offset, word_id);
	}
	return cache;
}

void SearchEngine::delete_page_async(const std::string& url_key)
{
	page_info_async->get_value(Variant(url_key),
			std::bind(&SearchEngine::delete_page_callback, this, url_key, std::placeholders::_1));
}

void SearchEngine::delete_page_callback(const std::string& url_key,
										std::shared_ptr<const keyvalue::Entry> entry)
{
	auto info = std::dynamic_pointer_cast<const PageInfo>(entry->value);
	
	if(!info || !info->is_deleted || info->word_version) {
		auto cached = get_info_cache(url_key);
		cached->is_deleted = 1;
	}
	if(info && info->link_version)
	{
		for(const auto& child_key : info->links)
		{
			auto child_cache = get_info_cache(child_key);
			child_cache->rem_reverse_links.push_back(url_key);
		}
		auto cached = get_info_cache(url_key);
		cached->is_deleted = 1;
		cached->schedule_time_us = vnx::get_wall_time_micros();
	}
}

void SearchEngine::redirect_callback(	const std::string& org_url_key,
										const std::string& new_url_key,
										std::shared_ptr<const keyvalue::Entry> entry)
{
	auto org_page_info = std::dynamic_pointer_cast<const PageInfo>(entry->value);
	if(org_page_info && !org_page_info->is_deleted)
	{
		auto p_new_cache = get_info_cache(new_url_key);
		
		for(const auto& link : org_page_info->reverse_links)
		{
			auto cached = get_info_cache(link.url_key);
			cached->add_links.push_back(new_url_key);
			cached->rem_links.push_back(org_url_key);
			p_new_cache->add_reverse_links.push_back(link);
		}
		p_new_cache->schedule_time_us = vnx::get_wall_time_micros();
		
		delete_page_async(org_url_key);
	}
}

int64_t SearchEngine::get_rank_update_interval(float rank_value) const
{
	return rank_update_interval * 60 * std::max(1000. / std::max(rank_value, 1.f), 1.);
}

void SearchEngine::handle(std::shared_ptr<const keyvalue::SyncUpdate> entry)
{
	if(auto info = std::dynamic_pointer_cast<const PageInfo>(entry->value))
	{
		if(info->id && !info->is_deleted)
		{
			if(page_ranking.size() < page_ranking_size
				|| info->reverse_domains.size() > std::prev(page_ranking.end())->first)
			{
				for(auto iter = page_ranking.begin(); iter != page_ranking.end(); ++iter) {
					if(iter->second == info->id) {
						page_ranking.erase(iter);
						break;
					}
				}
				if(page_ranking.size() >= page_ranking_size) {
					page_ranking.erase(std::prev(page_ranking.end()));
				}
				page_ranking.emplace(info->reverse_domains.size(), info->id);
			}
			if(auto* page = find_page(info->id))
			{
				page->rank_value = info->rank_value;
				page->array_version = info->array_version;
				page->reverse_links = info->reverse_links.size();
				page->reverse_domains = info->reverse_domains.size();
				return;
			}
			std::unique_lock lock(index_mutex);
			
			const std::string url_key = entry->key.to_string_value();
			const stx::pstring p_url_key = url_key;
			const Url::Url parsed(url_key);
			page_map[p_url_key] = info->id;
			
			auto& page = page_index[info->id];
			page.id = info->id;
			page.url_key = p_url_key;
			page.rank_value = info->rank_value;
			page.index_version = info->index_version;
			page.link_version = info->link_version;
			page.word_version = info->word_version;
			page.array_version = info->array_version;
			page.reverse_links = info->reverse_links.size();
			page.reverse_domains = info->reverse_domains.size();
			{
				auto& domain = get_domain(parsed.host());
				page.domain_id = domain.id;
				domain.pages.push_back(page.id);
			}
			{
				int64_t next_update = info->last_updated;
				if(!next_update) {
					next_update = vnx::get_wall_time_seconds();
				}
				next_update += get_rank_update_interval(info->rank_value);
				page.next_rank_update = next_update;
				rank_update_queue.emplace(next_update, info->id);
			}
			next_page_id = std::max(next_page_id, info->id + 1);
		}
		return;
	}
	
	if(auto word_context = std::dynamic_pointer_cast<const WordContext>(entry->value))
	{
		std::unique_lock lock(index_mutex);
		
		const auto key = entry->key.to_string_value();
		const auto word_id = word_context->id;
		word_map[key] = word_id;
		
		word_t& word = word_index[word_id];
		word.id = word_id;
		word.num_pages = word_context->pages.size();
		word.value = key;
		
		if(word.num_pages >= word_suggest_threshold) {
			word_suggest_map[key] = word_id;
		}
		next_word_id = std::max(next_word_id, word_context->id + 1);
		return;
	}
	
	if(auto url_index = std::dynamic_pointer_cast<const UrlIndex>(entry->value))
	{
		const auto org_url_key = entry->key.to_string_value();
		if(auto* page = find_page_url(org_url_key))
		{
			page->scheme = url_index->scheme;
			page->first_seen = url_index->first_seen;
			page->last_modified = url_index->last_modified;
		}
		if(!url_index->redirect.empty())
		{
			const auto new_url_key = get_url_key(url_index->redirect);
			if(new_url_key != org_url_key)
			{
				page_info_async->get_value(Variant(org_url_key),
						std::bind(&SearchEngine::redirect_callback, this, org_url_key, new_url_key, std::placeholders::_1));
			}
		}
		return;
	}
	
	if(entry->collection == "page_index")
	{
		auto job = std::make_shared<page_update_job_t>();
		job->url_key = entry->key.to_string_value();
		job->index_version = entry->version;
		
		const auto* page = find_page_url(job->url_key);
		job->update_info = !page || job->index_version != page->index_version;
		job->update_links = !page || job->index_version != page->link_version || update_page_info;
		job->update_words = !page || job->index_version != page->word_version || update_word_context;
		
		if(job->update_info || job->update_links || job->update_words) {
			load_queue.emplace(job);
		}
		return;
	}
	
	if(entry->collection == "page_content")
	{
		const auto url_key = entry->key.to_string_value();
		const auto* page = find_page_url(url_key);
		
		if(!page || entry->version != page->array_version || update_word_array)
		{
			if(!is_robots_txt(Url::Url(url_key)))
			{
				auto job = std::make_shared<word_process_job_t>();
				job->url_key = url_key;
				job->content_version = entry->version;
				load_queue_2.emplace(job);
			}
		}
		return;
	}
}

void SearchEngine::handle(std::shared_ptr<const keyvalue::SyncInfo> value)
{
	if(value->code == keyvalue::SyncInfo::END)
	{
		init_sync_count++;
		if(value->collection == "page_info")
		{
			log(INFO).out << "Loaded " << page_index.size() << " page infos.";
		}
		if(init_sync_count == 1)
		{
			word_context_async->sync_all_private(word_context_stream->get_src_mac());
			log(INFO).out << "Starting WordContext sync ...";
		}
		if(init_sync_count == 2)
		{
			subscribe(input_url_index, 100);
			url_index_async->sync_all_private(url_index_stream->get_src_mac());
			log(INFO).out << "Starting UrlIndex sync ...";
		}
		if(init_sync_count == 3)
		{
			subscribe(input_page_index, 100);
			subscribe(input_page_content, 100);
			page_index_async->sync_all_keys_private(page_index_stream->get_src_mac());
			page_content_async->sync_all_keys_private(page_content_stream->get_src_mac());
			log(INFO).out << "Starting PageIndex / PageContent sync ...";
		}
		if(init_sync_count == 5)
		{
			is_initialized = true;
			log(INFO).out << "Initialized with " << domain_index.size() << " domains, "
					<< page_index.size() << " pages and " << word_index.size() << " words.";
		}
	}
}

void SearchEngine::update_page_callback_0(	std::shared_ptr<page_update_job_t> job,
											std::shared_ptr<const keyvalue::Entry> entry)
{
	auto url_index = std::dynamic_pointer_cast<const UrlIndex>(entry->value);
	if(url_index) {
		if(!url_index->redirect.empty())
		{
			const Url::Url parsed_redir(url_index->redirect);
			if(job->url_key != get_url_key(parsed_redir))
			{
				delete_page_async(job->url_key);
				return;
			} else {
				job->scheme = parsed_redir.scheme();
			}
		} else {
			job->scheme = url_index->scheme;
		}
	} else {
		job->scheme = "http";
	}
	job->url_index = url_index;
	
	page_index_async->get_value(Variant(job->url_key),
			std::bind(&SearchEngine::update_page_callback_1, this, job, std::placeholders::_1));
}

void SearchEngine::update_page_callback_1(	std::shared_ptr<page_update_job_t> job,
											std::shared_ptr<const keyvalue::Entry> entry)
{
	auto index = std::dynamic_pointer_cast<const PageIndex>(entry->value);
	if(!index) {
		delete_page_async(job->url_key);
		return;
	}
	job->index = index;
	
	std::vector<Variant> link_keys;
	if(job->update_links) {
		for(const auto& link : job->index->links) {
			try {
				const Url::Url parsed(link.url);
				if(std::find(protocols.begin(), protocols.end(), parsed.scheme()) == protocols.end()) {
					continue;
				}
				const auto url_key = get_url_key(parsed);
				
				parsed_link_t new_link;
				new_link.page_link_t::operator=(link);
				new_link.url_key = url_key;
				job->links.push_back(new_link);
				link_keys.push_back(Variant(url_key));
			} catch(...) {
				// ignore
			}
		}
	}
	url_index_async->get_values(link_keys,
			std::bind(&SearchEngine::update_page_callback_2, this, job, std::placeholders::_1));
}

void SearchEngine::update_page_callback_2(	std::shared_ptr<page_update_job_t> job,
											std::vector<std::shared_ptr<const keyvalue::Entry>> entries)
{
	for(auto entry : entries) {
		if(auto url_index = std::dynamic_pointer_cast<const UrlIndex>(entry->value)) {
			try {
				if(!url_index->redirect.empty()) {
					const auto org_url_key = entry->key.to_string_value();
					const auto new_url_key = get_url_key(url_index->redirect);
					if(new_url_key != org_url_key) {
						job->redirects[org_url_key] = new_url_key;
					}
				}
			} catch(...) {
				// ignore
			}
		}
	}
	if(job->update_words) {
		update_threads->add_task(std::bind(&SearchEngine::word_collect_task, this, job));
	} else {
		update_page_callback_3(job);
	}
}

void SearchEngine::update_page_callback_3(	std::shared_ptr<page_update_job_t> job)
{
	page_info_async->get_value(Variant(job->url_key),
			std::bind(&SearchEngine::update_page_callback_4, this, job, std::placeholders::_1));
}

void SearchEngine::update_page_callback_4(	std::shared_ptr<page_update_job_t> job,
											std::shared_ptr<const keyvalue::Entry> entry)
{
	job->info = std::dynamic_pointer_cast<const PageInfo>(entry->value);
	update_page(job);
}

void SearchEngine::update_page(std::shared_ptr<page_update_job_t> job)
{
	std::unique_lock lock(index_mutex);
	
	const auto info = job->info;
	const auto index = job->index;
	const auto url_index = job->url_index;
	const auto url_key = job->url_key;
	const Url::Url parsed(url_key);
	const auto domain = parsed.host();
	
	uint32_t page_id = 0;
	if(info && info->id) {
		page_id = info->id;
	} else {
		page_id = next_page_id++;
	}
	for(const auto& word : job->new_words) {
		const auto id = add_word(word);
		job->words[id]++;
	}
	
	auto& page = page_index[page_id];
	if(!page.id) {
		page.id = page_id;
		{
			const stx::pstring p_url_key = url_key;
			page.url_key = p_url_key;
			page_map[p_url_key] = page_id;
		}
		{
			auto& r_domain = get_domain(domain);
			page.domain_id = r_domain.id;
			r_domain.pages.push_back(page.id);
		}
	}
	page.index_version = job->index_version;
	page.link_version = job->index_version;
	page.word_version = job->index_version;
	page.scheme = job->scheme;
	page.last_modified = index->last_modified;
	
	if(info) {
		page.rank_value = info->rank_value;
		page.array_version = info->array_version;
		page.reverse_links = info->reverse_links.size();
		page.reverse_domains = info->reverse_domains.size();
	}
	if(url_index) {
		page.first_seen = url_index->first_seen;
	}
	auto p_info_cache = get_info_cache(url_key);
	
	if(job->update_info)
	{
		p_info_cache->page_id = page_id;
		p_info_cache->is_deleted = 0;
		p_info_cache->index_version = job->index_version;
	}
	if(job->update_links && !p_info_cache->link_version)
	{
		std::map<std::string, page_link_t> new_links;
		p_info_cache->version = 1;
		p_info_cache->link_version = job->index_version;
		
		for(const auto& link : job->links)
		{
			if(link.url_key == url_key) {
				continue;	// ignore self links
			}
			const auto iter = job->redirects.find(link.url_key);
			if(iter != job->redirects.end()) {
				new_links[iter->second] = link;
			} else {
				new_links[link.url_key] = link;
			}
		}
		if(info) {
			// check for new or removed links
			for(const auto& link_key : info->links)
			{
				const auto iter = new_links.find(link_key);
				if(iter != new_links.end()) {
					// link already exists
					if(info->version > 0) {
						new_links.erase(iter);
					}
				} else {
					// link was removed
					auto cached = get_info_cache(link_key);
					cached->rem_reverse_links.push_back(url_key);
					p_info_cache->rem_links.push_back(link_key);
				}
			}
		}
		for(const auto& entry : new_links)
		{
			reverse_link_t reverse_link;
			reverse_link.url_key = url_key;
			for(const auto& word : entry.second.words) {
				reverse_link.words.push_back(add_word(word));
			}
			// add new link
			const auto& link_key = entry.first;
			auto cached = get_info_cache(link_key);
			cached->add_reverse_links.emplace_back(std::move(reverse_link));
			p_info_cache->add_links.push_back(link_key);
		}
		// make sure our link cache is saved last
		p_info_cache->schedule_time_us = vnx::get_wall_time_micros();
	}
	if(job->update_words)
	{
		auto new_job = std::make_shared<rank_update_job_t>();
		new_job->url_key = url_key;
		new_job->page_id = page_id;
		for(const auto& entry : job->words) {
			if(entry.second > 0) {
				new_job->update_words.push_back(entry.first);
			} else if(entry.second < 0) {
				new_job->rem_words.push_back(entry.first);
			}
		}
		if(!info || job->index_version != info->word_version) {
			new_job->word_version = job->index_version;
		}
		if(info && p_info_cache->add_reverse_links.empty()) {
			new_job->info = info;
			update_page_rank(new_job);
		} else {
			p_info_cache->rank_update_job = new_job;
		}
	}
}

void SearchEngine::update_page_rank(std::shared_ptr<rank_update_job_t> job)
{
	std::vector<std::string> url_keys;
	for(const auto& link : job->info->reverse_links) {
		url_keys.push_back(link.url_key);
	}
	search_async->get_page_ranks(url_keys, reset_rank_values,
			std::bind(&SearchEngine::update_page_rank_callback, this, job, std::placeholders::_1));
}

void SearchEngine::update_page_rank_callback(	std::shared_ptr<rank_update_job_t> job,
												std::vector<float> rank_values)
{
	const auto info = job->info;
	const auto page_id = job->page_id;
	float rank_value = info->reverse_domains.size();
	std::unordered_map<uint32_t, float> word_rank;
	
	for(size_t i = 0; i < rank_values.size(); ++i)
	{
		const auto value = rank_values[i];
		const auto& link = info->reverse_links[i];
		for(const auto word_id : link.words) {
			auto& word_value = word_rank[word_id];
			word_value = std::max(word_value, value);
		}
		rank_value = std::max(rank_value, value * rank_decay);
	}
	auto p_info_cache = get_info_cache(job->url_key);
	p_info_cache->rank_value = rank_value;
	
	if(auto* page = find_page(page_id)) {
		page->next_rank_update = vnx::get_wall_time_seconds() + get_rank_update_interval(rank_value);
		rank_update_queue.emplace(page->next_rank_update, page->id);
	} else {
		return;
	}
	if(page_cache.count(page_id)) {
		log(DEBUG) << "Previous rank update not yet finished for: " << job->url_key << " (rank_value = " << rank_value << ")";
		return;
	}
	auto p_page_cache = std::make_shared<page_cache_t>();
	p_page_cache->url_key = job->url_key;
	p_page_cache->word_version = job->word_version;
	
	for(const auto word_id : job->rem_words)
	{
		auto cached = get_word_cache(word_id);
		cached->update_pages.emplace_back(page_id, -1);
		p_page_cache->num_pending++;
	}
	for(const auto word_id : job->update_words)
	{
		auto value = rank_value;
		{
			auto iter = word_rank.find(word_id);
			if(iter != word_rank.end()) {
				value += iter->second;
			}
		}
		auto cached = get_word_cache(word_id);
		cached->update_pages.emplace_back(page_id, value);
		p_page_cache->num_pending++;
	}
	if(!p_page_cache->num_pending) {
		p_info_cache->word_version = job->word_version;
		p_info_cache->words.clear();
	} else {
		page_cache[page_id] = p_page_cache;
	}
}

void SearchEngine::check_queues()
{
	if(!is_initialized) {
		return;
	}
	check_info_queue();
	check_word_queue();
	check_load_queue();
}

void SearchEngine::check_load_queue()
{
	while((!load_queue.empty() || !load_queue_2.empty())
			&& url_index_async->vnx_get_num_pending() < max_num_pending
			&& page_content_async->vnx_get_num_pending() < max_num_pending
			&& update_threads->get_num_pending() < max_num_pending
			&& info_cache.size() < 1.1 * max_info_cache
			&& page_cache.size() < 1.1 * max_page_cache
			&& word_cache.size() < 1.1 * max_word_cache)
	{
		if(!load_queue.empty()) {
			const auto job = load_queue.front();
			url_index_async->get_value(Variant(job->url_key),
					std::bind(&SearchEngine::update_page_callback_0, this, job, std::placeholders::_1));
			load_queue.pop();
		}
		if(!load_queue_2.empty()) {
			const auto job = load_queue_2.front();
			page_content_async->get_value(Variant(job->url_key),
					std::bind(&SearchEngine::word_process_callback_0, this, job, std::placeholders::_1));
			load_queue_2.pop();
		}
	}
}

void SearchEngine::check_info_queue()
{
	const auto now = vnx::get_wall_time_micros();
	while(!info_queue.empty()
			&& page_info_async->vnx_get_num_pending() < max_num_pending
			&& update_threads->get_num_pending() < max_num_pending)
	{
		const auto iter = info_queue.begin();
		if(now - iter->first > int64_t(info_commit_interval) * 1000000 || info_cache.size() > max_info_cache)
		{
			const auto cache = iter->second;
			if(cache->schedule_time_us) {
				info_queue.emplace(cache->schedule_time_us, cache);
				cache->schedule_time_us = 0;
			} else {
				page_info_async->get_value_locked(Variant(cache->url_key), lock_timeout * 1000,
						std::bind(&SearchEngine::info_update_callback_0, this, cache, std::placeholders::_1));
				info_cache.erase(cache->url_key);
			}
			info_queue.erase(iter);
		} else {
			break;
		}
	}
	while(!rank_update_queue.empty()
			&& load_queue.empty()
			&& info_cache.size() < 0.9 * max_info_cache
			&& page_cache.size() < 0.9 * max_page_cache
			&& word_cache.size() < 0.9 * max_word_cache
			&& page_info_async->vnx_get_num_pending() < max_num_pending)
	{
		const auto iter = rank_update_queue.begin();
		if(now > iter->first * 1000000) {
			const auto page_id = iter->second;
			if(auto* page = find_page(page_id)) {
				if(iter->first == page->next_rank_update) {
					const auto url_key = page->url_key.str();
					page_info_async->get_value(Variant(url_key),
						[this, page_id, url_key](std::shared_ptr<const keyvalue::Entry> entry) {
							if(auto info = std::dynamic_pointer_cast<const PageInfo>(entry->value)) {
								auto job = std::make_shared<rank_update_job_t>();
								job->url_key = url_key;
								job->page_id = page_id;
								job->update_words = info->words;
								job->info = info;
								update_page_rank(job);
							}
						});
				}
			}
			rank_update_queue.erase(iter);
		}
		else {
			break;
		}
	}
}

void SearchEngine::check_word_queue()
{
	const auto now = vnx::get_wall_time_seconds();
	while(!word_queue.empty()
			&& word_context_async->vnx_get_num_pending() < max_num_pending
			&& update_threads->get_num_pending() < max_num_pending)
	{
		const auto iter = word_queue.begin();
		if(now - iter->first >= word_commit_interval
			|| page_cache.size() > max_page_cache
			|| word_cache.size() > max_word_cache)
		{
			const auto word_id = iter->second;
			const auto iter2 = word_cache.find(word_id);
			if(iter2 != word_cache.end())
			{
				if(auto* word = find_word(word_id)) {
					auto job = std::make_shared<word_update_job_t>();
					job->word = word->value.str();
					job->cached = iter2->second;
					word_context_async->get_value(Variant(job->word),
							std::bind(&SearchEngine::word_update_callback, this, job, std::placeholders::_1));
				} else {
					log(WARN) << "Unknown word_id: " << word_id;
				}
				word_cache.erase(iter2);
			}
			word_queue.erase(iter);
		} else {
			break;
		}
	}
}

void SearchEngine::info_update_callback_0(	std::shared_ptr<info_cache_t> cached,
											std::shared_ptr<const keyvalue::Entry> entry)
{
	auto info = std::dynamic_pointer_cast<const PageInfo>(entry->value);
	
	auto job = std::make_shared<info_update_job_t>();
	job->cached = cached;
	job->info = info;
	
	if(cached->is_deleted > 0)
	{
		uint32_t page_id = 0;
		if(auto* page = find_page_url(cached->url_key))
		{
			page_id = page->id;
			{
				std::unique_lock lock(index_mutex);
				page_map.erase(page->url_key);
				page_index.erase(page->id);
			}
		}
		cached->rank_update_job = nullptr;
		
		auto words = cached->words;
		if(info) {
			words = unique(concat(words, info->words));
		}
		for(const auto word_id : words)
		{
			const auto cache = get_word_cache(word_id);
			cache->update_pages.emplace_back(page_id, -1);
		}
		word_array_async->delete_value(Variant(cached->url_key));
	}
	update_threads->add_task(std::bind(&SearchEngine::info_update_task, this, job));
}

void SearchEngine::info_update_callback(std::shared_ptr<info_update_job_t> job)
{
	auto cached = job->cached;
	page_info_async->store_value_delay(Variant(cached->url_key), job->result,
			(cached->link_version || cached->is_deleted > 0) ? commit_delay * 1000 : 0);
	
	if(auto new_job = cached->rank_update_job) {
		new_job->info = job->result;
		update_page_rank(new_job);
	}
	if(cached->word_version) {
		page_update_counter++;
	}
}

void SearchEngine::word_update_callback(std::shared_ptr<word_update_job_t> job,
										std::shared_ptr<const keyvalue::Entry> entry)
{
	job->context = std::dynamic_pointer_cast<const WordContext>(entry->value);
	update_threads->add_task(std::bind(&SearchEngine::word_update_task, this, job));
}

void SearchEngine::word_update_finished(std::shared_ptr<word_update_job_t> job)
{
	word_context_async->store_value(Variant(job->word), job->result,
		[this, job]() {
			const auto word_id = job->cached->word_id;
			for(const auto& entry : job->cached->update_pages)
			{
				const auto page_id = entry.first;
				const auto iter = page_cache.find(page_id);
				if(iter != page_cache.end())
				{
					const auto& p_page_cache = iter->second;
					if(entry.second >= 0) {
						p_page_cache->words.push_back(word_id);
					}
					if(--p_page_cache->num_pending == 0)
					{
						if(const auto word_version = p_page_cache->word_version)
						{
							auto cached = get_info_cache(p_page_cache->url_key);
							cached->word_version = word_version;
							cached->words = std::move(p_page_cache->words);
						}
						page_cache.erase(iter);
					}
				}
			}
			const auto iter = word_index.find(word_id);
			if(iter != word_index.end()) {
				auto& word = iter->second;
				word.num_pages = job->result->pages.size();
				if(word.num_pages >= word_suggest_threshold) {
					word_suggest_map[word.value.str()] = word_id;
				}
			}
			word_update_counter++;
		});
}

void SearchEngine::word_process_callback_0(	std::shared_ptr<word_process_job_t> job,
											std::shared_ptr<const keyvalue::Entry> entry)
{
	if(auto content = std::dynamic_pointer_cast<const PageContent>(entry->value))
	{
		job->content = content;
		job->content_version = entry->version;
		update_threads->add_task(std::bind(&SearchEngine::word_process_task, this, job));
	}
}

void SearchEngine::word_process_callback_1(std::shared_ptr<word_process_job_t> job)
{
	if(job->num_new_words)
	{
		std::unique_lock lock(index_mutex);
		
		auto& list = job->word_array->list;
		for(size_t i = 0; i < list.size(); ++i) {
			if(list[i].word_id == 0) {
				list[i].word_id = add_word(job->word_list[i]);
			}
		}
	}
	word_array_async->store_value(Variant(job->url_key), job->word_array,
		[this, job]() {
			auto cached = get_info_cache(job->url_key);
			cached->array_version = job->content_version;
		});
}

void SearchEngine::print_stats()
{
	log(INFO).out << (60000 * word_update_counter) / stats_interval_ms << " words/min, "
			<< (60000 * page_update_counter) / stats_interval_ms << " pages/min, "
			<< domain_index.size() << " domains, "
			<< load_queue.size() << " + " << load_queue_2.size() << " / " << info_queue.size() << " / "
			<< page_cache.size() << " / " << page_index.size() << " pages, "
			<< word_cache.size() << " / " << word_index.size() << " words";
	
	word_update_counter = 0;
	page_update_counter = 0;
}

static
void write_info_callback(void* file, const char* data)
{
	fwrite(data, 1, strlen(data), (FILE*)file);
}

void SearchEngine::write_info()
{
	FILE* file = fopen("engine_malloc_info.txt", "w");
	malloc_stats_print(&write_info_callback, file, 0);
	fclose(file);
}

void SearchEngine::info_update_task(std::shared_ptr<info_update_job_t> job) noexcept
{
	if(job->info) {
		job->result = vnx::clone(job->info);
		job->info = nullptr;
	} else {
		job->result = PageInfo::create();
	}
	auto info = job->result;
	auto cache = job->cached;
	
	if(cache->is_deleted <= 0)
	{
		if(cache->page_id) {
			info->id = cache->page_id;
		}
		if(cache->version) {
			info->version = cache->version;
		}
		if(cache->rank_value >= 0) {
			info->rank_value = cache->rank_value;
			info->last_updated = vnx::get_wall_time_seconds();
		}
		if(cache->index_version) {
			info->index_version = cache->index_version;
		}
		if(cache->link_version) {
			info->link_version = cache->link_version;
		}
		if(cache->word_version) {
			info->word_version = cache->word_version;
			info->words = cache->words;
		}
		if(cache->array_version) {
			info->array_version = cache->array_version;
		}
		info->is_deleted = false;
	}
	else {
		info->id = 0;
		info->is_deleted = true;
		info->index_version = 0;
		info->link_version = 0;
		info->word_version = 0;
		info->array_version = 0;
		info->words.clear();
		info->links.clear();
		
		cache->add_links.clear();
		cache->rem_links.clear();
	}
	
	if(!cache->rem_links.empty() || !cache->add_links.empty())
	{
		std::set<std::string> links(info->links.begin(), info->links.end());
		for(const auto& link_key : cache->rem_links) {
			links.erase(link_key);
		}
		for(const auto& link_key : cache->add_links) {
			links.insert(link_key);
		}
		info->links = std::vector<std::string>(links.begin(), links.end());
	}
	if(!cache->rem_reverse_links.empty() || !cache->add_reverse_links.empty())
	{
		std::map<std::string, reverse_link_t> links;
		for(auto& link : info->reverse_links) {
			links[link.url_key] = std::move(link);
		}
		for(const auto& url_key : cache->rem_reverse_links) {
			links.erase(url_key);
		}
		for(const auto& link : cache->add_reverse_links) {
			links[link.url_key] = link;
		}
		links.erase("");
		info->reverse_links.clear();
		info->reverse_links.reserve(links.size());
		for(auto& entry : links) {
			info->reverse_links.emplace_back(std::move(entry.second));
		}
		info->reverse_domains.clear();
		for(const auto& link : info->reverse_links) {
			info->reverse_domains[Url::Url(link.url_key).host()]++;
		}
		info->rank_value = fmaxf(info->rank_value, info->reverse_domains.size());
	}
	add_task(std::bind(&SearchEngine::info_update_callback, this, job));
}

void SearchEngine::word_collect_task(std::shared_ptr<page_update_job_t> job) noexcept
{
	std::shared_lock lock(index_mutex);
	
	for(const auto& entry : job->index->words)
	{
		const auto iter = word_map.find(entry.first);
		if(iter != word_map.end()) {
			job->words[iter->second]++;
		} else {
			job->new_words.push_back(entry.first);
		}
	}
	if(job->info) {
		for(const auto word_id : job->info->words) {
			if(word_index.count(word_id)) {
				job->words[word_id]--;
			}
		}
	}
	
	add_task(std::bind(&SearchEngine::update_page_callback_3, this, job));
}

void SearchEngine::word_process_task(std::shared_ptr<word_process_job_t> job) noexcept
{
	try {
		job->word_list = parse_text(job->content->text, &job->word_positions);
	} catch(const std::exception& ex) {
		log(WARN) << ex.what();
	}
	std::shared_lock lock(index_mutex);
	
	auto array = WordArray::create();
	array->last_update = vnx::get_time_seconds();
	array->list.resize(job->word_list.size());
	
	for(size_t i = 0; i < job->word_list.size(); ++i)
	{
		auto& entry = array->list[i];
		const auto iter = word_map.find(job->word_list[i]);
		if(iter != word_map.end()) {
			entry.word_id = iter->second;
		} else {
			job->num_new_words++;
		}
		const auto& position = job->word_positions[i];
		entry.offset = position.first;
		entry.size = position.second - position.first;
	}
	job->word_array = array;
	
	add_task(std::bind(&SearchEngine::word_process_callback_1, this, job));
}

void SearchEngine::word_update_task(std::shared_ptr<word_update_job_t> job) noexcept
{
	auto cached = job->cached;
	auto context = job->context;
	const auto time = vnx::get_time_seconds();
	
	if(context) {
		auto delta = cached->update_pages;
		{
			size_t i = 0;
			std::shared_lock lock(index_mutex);
			for(const auto& entry : context->pages) {
				if(!page_index.count(entry.first)) {
					delta.emplace_back(entry.first, -1);
				}
				if(i++ % 65535 == 0) {
					lock.unlock();
					lock.lock();
				}
			}
		}
		job->result = context->apply(delta, time);
	}
	else {
		auto context = WordContext::create();
		context->id = cached->word_id;
		job->result = context->apply(cached->update_pages, time);
	}
	
	add_task(std::bind(&SearchEngine::word_update_finished, this, job));
}


} // search
} // vnx
