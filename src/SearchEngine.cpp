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
	input_url_index_sync = vnx_name + ".url_index.sync_" + std::to_string(vnx::rand64());
	input_page_index_sync = vnx_name + ".page_index.sync_" + std::to_string(vnx::rand64());
	input_page_content_sync = vnx_name + ".page_content.sync_" + std::to_string(vnx::rand64());
	input_page_info_sync = vnx_name + ".page_info.sync_" + std::to_string(vnx::rand64());
	input_word_context_sync = vnx_name + ".word_context.sync_" + std::to_string(vnx::rand64());
	
	protocols.push_back("http");
	protocols.push_back("https");
}

void SearchEngine::init()
{
	private_addr = Hash64::rand();
	vnx::open_pipe(private_addr, this, UNLIMITED);
	vnx::open_pipe(vnx_name, this, 100);
}

void SearchEngine::main()
{
	subscribe(input_page_info, 100);
	subscribe(input_url_index_sync, 100, 1000);
	subscribe(input_page_index_sync, 100, 1000);
	subscribe(input_page_content_sync, 100, 1000);
	subscribe(input_page_info_sync, 100, 1000);
	subscribe(input_word_context_sync, 100, 1000);
	
	protocols = get_unique(protocols);
	
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
	
	search_async = std::make_shared<SearchEngineAsyncClient>(private_addr);
	page_info_async = std::make_shared<keyvalue::ServerAsyncClient>("PageInfo");
	word_context_async = std::make_shared<keyvalue::ServerAsyncClient>("WordContext");
	word_array_async = std::make_shared<keyvalue::ServerAsyncClient>("WordArray");
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
	
	page_info_async->sync_all(input_page_info_sync);
	
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
		const auto* domain = find_domain(iter->second);
		if(domain) {
			std::vector<std::pair<std::pair<uint32_t, uint32_t>, const page_t*>> sorted;
			for(const auto page_id : domain->pages) {
				const auto* page = find_page(page_id);
				if(page) {
					sorted.emplace_back(std::make_pair(page->reverse_domains, page->reverse_links), page);
				}
			}
			std::sort(sorted.begin(), sorted.end(),
					std::greater<std::pair<std::pair<uint32_t, uint32_t>, const page_t*>>());
			
			std::vector<std::string> pages;
			for(uint32_t i = 0; i < uint32_t(limit) && offset + i < sorted.size(); ++i) {
				const auto* page = sorted[offset + i].second;
				pages.push_back(page->get_url());
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
			std::bind(&SearchEngine::get_page_info_callback, this, url_key, std::placeholders::_1, req_id),
			[this, req_id](const std::exception& ex) {
				vnx_async_return(req_id, vnx::InternalError::from_what(ex.what()));
			});
}

void SearchEngine::get_page_info_callback(	const std::string& url_key,
											std::shared_ptr<const keyvalue::Entry> entry,
											const request_id_t& req_id) const
{
	Object result;
	auto info = std::dynamic_pointer_cast<const PageInfo>(entry->value);
	const auto* page = info ? find_page(info->id) : nullptr;
	if(info) {
		result["is_deleted"] = info->is_deleted;
		result["rank_value"] = info->rank_value;
		result["words"] = info->words.size();
		result["links"] = info->links.size();
		result["reverse_links"] = info->reverse_links.size();
		result["reverse_domains"] = info->reverse_domains.size();
	}
	if(page) {
		result["url"] = page->get_url();
		result["rank_value"] = page->rank_value;
		result["last_modified"] = page->last_modified;
		result["first_seen"] = page->first_seen;
		const auto* domain = find_domain(page->domain_id);
		if(domain) {
			result["domain"] = domain->host.str();
		}
	}
	get_page_info_async_return(req_id, result);
}

void SearchEngine::get_page_entries_async(const std::vector<uint32_t>& page_ids, const vnx::request_id_t& req_id) const
{
	std::vector<page_entry_t> result;
	for(auto page_id : page_ids) {
		const auto* page = find_page(page_id);
		if(page) {
			page_entry_t entry;
			entry.id = page->id;
			entry.rank_value = page->rank_value;
			entry.url = page->get_url();
			result.emplace_back(std::move(entry));
		}
	}
	get_page_entries_async_return(req_id, result);
}

void SearchEngine::get_page_ranks_async(const std::vector<std::string>& url_keys,
										const vnx::bool_t& direct,
										const request_id_t& req_id) const
{
	update_threads->add_task(std::bind(&SearchEngine::page_rank_task, this, url_keys, direct, req_id));
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
	for(uint32_t i = 0; i < uint32_t(limit) && offset + i < sorted.size(); ++i) {
		const auto* domain = sorted[offset + i].second;
		Object info;
		info["host"] = domain->host.str();
		info["num_pages"] = domain->pages.size();
		result.push_back(info);
	}
	return result;
}

void SearchEngine::reverse_lookup_async(const std::string& url_key,
										const request_id_t& req_id) const
{
	page_info_async->get_value(Variant(url_key),
			std::bind(&SearchEngine::reverse_lookup_callback, this, url_key, std::placeholders::_1, req_id),
			[this, req_id](const std::exception& ex) {
				vnx_async_return(req_id, vnx::InternalError::from_what(ex.what()));
			});
}

void SearchEngine::reverse_lookup_callback(	const std::string& url_key,
											std::shared_ptr<const keyvalue::Entry> entry,
											const request_id_t& req_id) const
{
	std::vector<std::pair<float, std::string>> sorted;
	auto info = std::dynamic_pointer_cast<const PageInfo>(entry->value);
	if(info) {
		for(const auto& link_key : info->reverse_links) {
			const auto* parent = find_page_url(link_key);
			if(parent) {
				sorted.emplace_back(parent->rank_value, parent->url_key.str());
			}
		}
	}
	std::sort(sorted.begin(), sorted.end(), std::greater<std::pair<float, std::string>>());
	
	std::vector<std::string> result;
	for(auto& entry : sorted) {
		result.emplace_back(std::move(entry.second));
	}
	reverse_lookup_async_return(req_id, result);
}

void SearchEngine::reverse_domain_lookup_async(	const std::string& url_key,
												const request_id_t& req_id) const
{
	page_info_async->get_value(Variant(url_key),
			std::bind(&SearchEngine::reverse_domain_lookup_callback, this, url_key, std::placeholders::_1, req_id),
			[this, req_id](const std::exception& ex) {
				vnx_async_return(req_id, vnx::InternalError::from_what(ex.what()));
			});
}

void SearchEngine::reverse_domain_lookup_callback(	const std::string& url_key,
													std::shared_ptr<const keyvalue::Entry> entry,
													const request_id_t& req_id) const
{
	std::vector<std::pair<std::string, uint32_t>> sorted;
	
	auto info = std::dynamic_pointer_cast<const PageInfo>(entry->value);
	if(info) {
		for(const auto& entry : info->reverse_domains) {
			sorted.emplace_back(entry);
		}
	}
	std::sort(sorted.begin(), sorted.end(),
			[] (const std::pair<std::string, uint32_t>& A, const std::pair<std::string, uint32_t>& B) -> bool {
				return A.second > B.second;
			});
	reverse_domain_lookup_async_return(req_id, sorted);
}

std::vector<std::string> SearchEngine::suggest_words(const std::string& prefix, const int32_t& limit) const
{
	std::vector<std::string> result;
	for(auto it = word_map.lower_bound(prefix); it != word_map.end() && result.size() < size_t(limit); ++it) {
		result.push_back(it->first.str());
	}
	return result;
}

std::vector<std::string> SearchEngine::suggest_domains(const std::string& prefix, const int32_t& limit) const
{
	std::vector<std::string> result;
	for(auto it = domain_map.lower_bound(prefix); it != domain_map.end() && result.size() < size_t(limit); ++it) {
		result.push_back(it->first.str());
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

std::shared_ptr<SearchEngine::link_cache_t> SearchEngine::get_link_cache(const std::string& url_key)
{
	auto& cache = link_cache[url_key];
	if(!cache) {
		cache = std::make_shared<link_cache_t>();
		cache->url_key = url_key;
		link_queue.emplace(vnx::get_wall_time_micros(), cache);
	}
	return cache;
}

std::shared_ptr<SearchEngine::word_cache_t> SearchEngine::get_word_cache(uint32_t word_id)
{
	auto& cache = word_cache[word_id];
	if(!cache) {
		cache = std::make_shared<word_cache_t>();
		cache->word_id = word_id;
		{
			auto* word = find_word(word_id);
			if(word) {
				cache->word = word->value.str();
			} else {
				throw std::logic_error("invalid word_id: " + std::to_string(word_id));
			}
		}
		const int64_t rand_offset = (::rand() * int64_t(word_commit_interval)) / RAND_MAX;
		word_queue.emplace(vnx::get_wall_time_seconds() + rand_offset, word_id);
	}
	return cache;
}

void SearchEngine::delete_page_async(const std::string& url_key)
{
	page_info_async->get_value_locked(Variant(url_key), lock_timeout * 1000,
			std::bind(&SearchEngine::delete_page_callback, this, url_key, std::placeholders::_1));
}

void SearchEngine::delete_page_callback(const std::string& url_key,
										std::shared_ptr<const keyvalue::Entry> entry)
{
	const auto info = std::dynamic_pointer_cast<const PageInfo>(entry->value);
	if(!info) {
		page_info_async->unlock(entry->key);
		return;
	}
	if(info->link_version)
	{
		const auto p_link_cache = get_link_cache(url_key);
		if(p_link_cache)
		{
			p_link_cache->link_version = 0;
			p_link_cache->is_page_update = true;
			p_link_cache->add_links.clear();
			p_link_cache->rem_links = info->links;
			
			for(const auto& child_key : info->links)
			{
				const auto cache = get_link_cache(child_key);
				if(cache) {
					cache->rem_reverse_links.push_back(url_key);
				}
			}
			p_link_cache->schedule_time_us = vnx::get_wall_time_micros();
		}
	}
	if(info->word_version)
	{
		auto& p_page_cache = page_cache[info->id];
		if(!p_page_cache) {
			p_page_cache = std::make_shared<page_cache_t>();
			p_page_cache->word_version = 0;
			p_page_cache->url_key = url_key;
			
			for(const auto word_id : info->words)
			{
				try {
					const auto cache = get_word_cache(word_id);
					cache->rem_pages.push_back(info->id);
					p_page_cache->words_pending++;
				} catch(...) {
					// ignore
				}
			}
			if(p_page_cache->words_pending == 0) {
				page_word_update_finished(info->id);
			}
		}
	}
	if(info->id)
	{
		std::unique_lock lock(index_mutex);
		page_index.erase(info->id);
		page_map.erase(url_key);
	}
	if(!info->is_deleted)
	{
		auto copy = vnx::clone(info);
		copy->is_deleted = true;
		copy->array_version = 0;
		page_info_async->store_value(entry->key, copy);
		word_array_async->delete_value(entry->key);
	} else {
		page_info_async->unlock(entry->key);
	}
}

void SearchEngine::redirect_callback(	const std::string& org_url_key,
										const std::string& new_url_key,
										std::shared_ptr<const keyvalue::Entry> entry)
{
	auto org_page_info = std::dynamic_pointer_cast<const PageInfo>(entry->value);
	
	if(org_page_info && !org_page_info->is_deleted)
	{
		auto p_new_cache = get_link_cache(new_url_key);
		
		for(const auto& parent_key : org_page_info->reverse_links)
		{
			auto cache = get_link_cache(parent_key);
			if(cache) {
				cache->add_links.push_back(new_url_key);
				cache->rem_links.push_back(org_url_key);
			}
			p_new_cache->add_reverse_links.push_back(parent_key);
		}
		p_new_cache->schedule_time_us = vnx::get_wall_time_micros();
		
		delete_page_async(org_url_key);
	}
}

void SearchEngine::handle(std::shared_ptr<const keyvalue::SyncUpdate> entry)
{
	auto info = std::dynamic_pointer_cast<const PageInfo>(entry->value);
	if(info) {
		if(info->id && !info->is_deleted)
		{
			{
				auto* page = find_page(info->id);
				if(page) {
					for(auto word_id : info->words) {
						try {
							get_word_cache(word_id);	// update words with new rank_value
						} catch(...) {
							// ignore
						}
					}
					page->rank_value = info->rank_value;
					page->array_version = info->array_version;
					page->reverse_links = info->reverse_links.size();
					page->reverse_domains = info->reverse_domains.size();
					return;
				}
			}
			std::unique_lock lock(index_mutex);
			
			auto& page = page_index[info->id];
			if(!page.id)
			{
				const stx::pstring url_key = entry->key.to_string_value();
				const Url::Url parsed(url_key.str());
				page_map[url_key] = info->id;
				
				page.id = info->id;
				page.url_key = url_key;
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
				next_page_id = std::max(next_page_id, info->id + 1);
			}
		}
		return;
	}
	
	auto url_index = std::dynamic_pointer_cast<const UrlIndex>(entry->value);
	if(url_index)
	{
		const auto org_url_key = entry->key.to_string_value();
		auto* page = find_page_url(org_url_key);
		if(page) {
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
	
	auto word_context = std::dynamic_pointer_cast<const WordContext>(entry->value);
	if(word_context)
	{
		std::unique_lock lock(index_mutex);
		
		const stx::sstring key = entry->key.to_string_value();
		word_map[key] = word_context->id;
		
		word_t& word = word_index[word_context->id];
		word.id = word_context->id;
		word.num_pages = word_context->pages.size();
		word.value = key;
		next_word_id = std::max(next_word_id, word_context->id + 1);
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
			word_context_async->sync_all(input_word_context_sync);
			log(INFO).out << "Starting WordContext sync ...";
		}
		if(init_sync_count == 2)
		{
			subscribe(input_url_index, 100);
			url_index_async->sync_all(input_url_index_sync);
			log(INFO).out << "Starting UrlIndex sync ...";
		}
		if(init_sync_count == 3)
		{
			subscribe(input_page_index, 100);
			subscribe(input_page_content, 100);
			page_index_async->sync_all_keys(input_page_index_sync);
			page_content_async->sync_all_keys(input_page_content_sync);
			log(INFO).out << "Starting PageIndex / PageContent sync ...";
		}
		if(init_sync_count == 5)
		{
			is_initialized = true;
			
			log(INFO).out << "Initialized with " << domain_map.size() << " domains, "
					<< page_index.size() << " pages and " << word_map.size() << " words.";
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
		for(const auto& link_url : job->index->links) {
			try {
				const Url::Url parsed(link_url);
				if(std::find(protocols.begin(), protocols.end(), parsed.scheme()) == protocols.end()) {
					continue;
				}
				const auto url_key = get_url_key(parsed);
				job->links.push_back(url_key);
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
		auto url_index = std::dynamic_pointer_cast<const UrlIndex>(entry->value);
		if(url_index) {
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
	page_info_async->get_value_locked(Variant(job->url_key), lock_timeout * 1000,
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
	
	auto& page = page_index[page_id];
	if(!page.id) {
		page.id = page_id;
		const stx::pstring p_url_key = url_key;
		page.url_key = p_url_key;
		page_map[p_url_key] = page_id;
	}
	page.index_version = job->index_version;
	page.link_version = job->index_version;
	page.word_version = job->index_version;
	page.scheme = job->scheme;
	page.last_modified = index->last_modified;
	
	if(!page.domain_id) {
		auto& r_domain = get_domain(domain);
		page.domain_id = r_domain.id;
		r_domain.pages.push_back(page.id);
	}
	if(info) {
		page.rank_value = info->rank_value;
		page.array_version = info->array_version;
		page.reverse_links = info->reverse_links.size();
		page.reverse_domains = info->reverse_domains.size();
	}
	if(url_index) {
		page.first_seen = url_index->first_seen;
	}
	
	if(job->update_info)
	{
		// initialize or update page_info
		auto copy = info ? vnx::clone(info) : PageInfo::create();
		copy->id = page_id;
		copy->is_deleted = false;
		copy->index_version = job->index_version;
		page_info_async->store_value(Variant(url_key), copy);
	} else {
		page_info_async->unlock(Variant(url_key));
	}
	
	if(job->update_links)
	{
		std::set<std::string> new_links;
		auto p_link_cache = get_link_cache(url_key);
		p_link_cache->link_version = job->index_version;
		p_link_cache->is_page_update = true;
		
		for(const auto& link_key : job->links)
		{
			if(link_key != url_key) {
				const auto iter = job->redirects.find(link_key);
				if(iter != job->redirects.end()) {
					new_links.emplace(iter->second);
				} else {
					new_links.emplace(link_key);
				}
			}
		}
		if(info) {
			// check for new or removed links
			for(const auto& link_key : info->links)
			{
				const auto iter = new_links.find(link_key);
				if(iter != new_links.end()) {
					// link already exists
					new_links.erase(iter);
				} else {
					// link was removed
					auto cached = get_link_cache(link_key);
					if(cached) {
						cached->rem_reverse_links.push_back(url_key);
					}
					p_link_cache->rem_links.push_back(link_key);
				}
			}
		}
		for(const auto& link_key : new_links)
		{
			// make new links
			auto cached = get_link_cache(link_key);
			if(cached) {
				cached->add_reverse_links.push_back(url_key);
			}
			p_link_cache->add_links.push_back(link_key);
		}
		// make sure our link cache is saved last
		p_link_cache->schedule_time_us = vnx::get_wall_time_micros();
	}
	
	// update word index if version is greater and previous update has finished
	if(job->update_words && !page_cache.count(page_id))
	{
		for(const auto& word : job->new_words)
		{
			const auto id = add_word(word);
			job->words[id]++;
		}
		
		auto& p_page_cache = page_cache[page_id];
		if(!p_page_cache) {
			p_page_cache = std::make_shared<page_cache_t>();
		}
		p_page_cache->word_version = job->index_version;
		p_page_cache->url_key = url_key;
		
		for(const auto& entry : job->words) {
			try {
				const auto word_id = entry.first;
				const auto p_word_cache = get_word_cache(word_id);
				if(entry.second >= 0) {
					p_word_cache->add_pages.push_back(page_id);
					p_page_cache->words_pending++;
				}
				if(entry.second <= 0) {
					p_word_cache->rem_pages.push_back(page_id);
					p_page_cache->words_pending++;
				}
			} catch(const std::exception& ex) {
				log(WARN) << "update_page(): " << ex.what();
			}
		}
		if(p_page_cache->words_pending == 0) {
			page_word_update_finished(page_id);
		}
	}
}

void SearchEngine::check_queues()
{
	check_link_queue();
	check_word_queue();
	check_load_queue();
}

void SearchEngine::check_load_queue()
{
	while((!load_queue.empty() || !load_queue_2.empty())
			&& url_index_async->vnx_get_num_pending() < max_num_pending
			&& page_content_async->vnx_get_num_pending() < max_num_pending
			&& update_threads->get_num_pending() < max_num_pending
			&& link_cache.size() <= 1.1 * max_link_cache
			&& word_cache.size() <= 1.1 * max_word_cache)
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

void SearchEngine::check_link_queue()
{
	const auto now = vnx::get_wall_time_micros();
	while(!link_queue.empty()
			&& page_info_async->vnx_get_num_pending() < max_num_pending
			&& update_threads->get_num_pending() < max_num_pending)
	{
		const auto iter = link_queue.begin();
		if(now - iter->first >= int64_t(link_commit_interval) * 1000000
				|| link_cache.size() > max_link_cache)
		{
			const auto cache = iter->second;
			if(cache->schedule_time_us) {
				link_queue.emplace(cache->schedule_time_us, cache);
				cache->schedule_time_us = 0;
			} else {
				page_info_async->get_value_locked(Variant(cache->url_key), lock_timeout * 1000,
						std::bind(&SearchEngine::link_update_callback_0, this, cache, std::placeholders::_1));
				link_cache.erase(cache->url_key);
			}
			link_queue.erase(iter);
		} else {
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
		if(now - iter->first >= word_commit_interval || word_cache.size() > max_word_cache)
		{
			const auto word_id = iter->second;
			const auto iter2 = word_cache.find(word_id);
			if(iter2 != word_cache.end())
			{
				auto job = std::make_shared<word_update_job_t>();
				job->cached = iter2->second;
				word_context_async->get_value(Variant(job->cached->word),
						std::bind(&SearchEngine::word_update_callback, this, job, std::placeholders::_1));
				word_cache.erase(iter2);
			}
			word_queue.erase(iter);
		} else {
			break;
		}
	}
}

void SearchEngine::link_update_callback_0(	std::shared_ptr<link_cache_t> cached,
											std::shared_ptr<const keyvalue::Entry> entry)
{
	auto job = std::make_shared<link_update_job_t>();
	job->cached = cached;
	job->info = std::dynamic_pointer_cast<const PageInfo>(entry->value);
	update_threads->add_task(std::bind(&SearchEngine::link_update_task, this, job));
}

void SearchEngine::link_update_callback_1(std::shared_ptr<link_update_job_t> job)
{
	search_async->get_page_ranks(job->result->reverse_links, reset_rank_values,
			std::bind(&SearchEngine::link_update_callback_2, this, job, std::placeholders::_1));
}

void SearchEngine::link_update_callback_2(	std::shared_ptr<link_update_job_t> job,
											std::vector<float> rank_values)
{
	auto cached = job->cached;
	auto result = job->result;
	result->rank_value = result->reverse_domains.size();
	for(auto value : rank_values) {
		result->rank_value = fmaxf(result->rank_value, value * rank_decay);
	}
	page_info_async->store_value_delay(Variant(cached->url_key), result, cached->is_page_update ? commit_delay * 1000 : 0);
	page_update_counter++;
}

void SearchEngine::word_update_callback(std::shared_ptr<word_update_job_t> job,
										std::shared_ptr<const keyvalue::Entry> entry)
{
	job->context = std::dynamic_pointer_cast<const WordContext>(entry->value);
	update_threads->add_task(std::bind(&SearchEngine::word_update_task, this, job));
}

void SearchEngine::word_update_finished(std::shared_ptr<word_update_job_t> job)
{
	word_context_async->store_value(Variant(job->cached->word), job->result,
		[this, job]() {
			word_update_counter++;
			
			for(const auto& page_id : job->cached->add_pages)
			{
				const auto iter = page_cache.find(page_id);
				if(iter != page_cache.end())
				{
					auto p_page_cache = iter->second;
					p_page_cache->words.insert(job->cached->word_id);
					if(--(p_page_cache->words_pending) == 0) {
						page_word_update_finished(page_id);
					}
				}
			}
			for(const auto page_id : job->cached->rem_pages)
			{
				const auto iter = page_cache.find(page_id);
				if(iter != page_cache.end())
				{
					auto p_page_cache = iter->second;
					if(--(p_page_cache->words_pending) == 0) {
						page_word_update_finished(page_id);
					}
				}
			}
			const auto iter = word_index.find(job->cached->word_id);
			if(iter != word_index.end()) {
				iter->second.num_pages = job->num_pages;
			}
		});
}

void SearchEngine::page_word_update_finished(uint32_t page_id)
{
	const auto iter = page_cache.find(page_id);
	if(iter != page_cache.end())
	{
		const auto cached = iter->second;
		if(cached->words_pending == 0)
		{
			page_info_async->get_value_locked(Variant(cached->url_key), lock_timeout * 1000,
					std::bind(&SearchEngine::page_word_update_callback, this, cached, std::placeholders::_1));
			page_cache.erase(iter);
		}
	}
}

void SearchEngine::page_word_update_callback(	std::shared_ptr<page_cache_t> cached,
												std::shared_ptr<const keyvalue::Entry> entry)
{
	const auto info = std::dynamic_pointer_cast<PageInfo>(vnx::clone(entry->value));
	if(info) {
		info->word_version = cached->word_version;
		info->words = std::vector<uint32_t>(cached->words.begin(), cached->words.end());
		page_info_async->store_value_delay(entry->key, info, commit_delay * 1000);
		page_update_counter++;
	} else {
		page_info_async->unlock(entry->key);
	}
}

void SearchEngine::word_process_callback_0(	std::shared_ptr<word_process_job_t> job,
											std::shared_ptr<const keyvalue::Entry> entry)
{
	auto content = std::dynamic_pointer_cast<const PageContent>(entry->value);
	if(content) {
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
			const auto* page = find_page_url(job->url_key);
			if(!page || page->array_version != job->content_version)
			{
				page_info_async->get_value_locked(Variant(job->url_key), lock_timeout * 1000,
						std::bind(&SearchEngine::word_process_callback_2, this, job, std::placeholders::_1));
			}
		});
}

void SearchEngine::word_process_callback_2(	std::shared_ptr<word_process_job_t> job,
											std::shared_ptr<const keyvalue::Entry> entry)
{
	auto info = std::dynamic_pointer_cast<const PageInfo>(entry->value);
	if(!info || !info->is_deleted)
	{
		auto copy = info ? vnx::clone(info) : PageInfo::create();
		copy->array_version = job->content_version;
		page_info_async->store_value_delay(entry->key, copy, commit_delay * 1000);
		page_update_counter++;
	} else {
		page_info_async->unlock(entry->key);
	}
}

void SearchEngine::print_stats()
{
	log(INFO).out << (60000 * word_update_counter) / stats_interval_ms << " words/min, "
			<< (60000 * page_update_counter) / stats_interval_ms << " pages/min, "
			<< domain_index.size() << " domains, "
			<< load_queue.size() << " / " << load_queue_2.size() << " / " << link_queue.size() << " / "
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

void SearchEngine::page_rank_task(const std::vector<std::string>& url_keys, const bool direct, const request_id_t& req_id) const noexcept
{
	std::shared_lock lock(index_mutex);
	
	std::vector<float> result;
	result.resize(url_keys.size());
	for(size_t i = 0; i < url_keys.size(); ++i)
	{
		const auto* page = find_page_url(url_keys[i]);
		if(page) {
			result[i] = direct ? page->reverse_domains : page->rank_value;
		}
		if(i % 8192 == 8191) {
			lock.unlock();
			lock.lock();
		}
	}
	get_page_ranks_async_return(req_id, result);
}

void SearchEngine::link_update_task(std::shared_ptr<link_update_job_t> job) noexcept
{
	auto cache = job->cached;
	auto info = vnx::clone(job->info);
	if(!info) {
		info = PageInfo::create();
	}
	if(cache->is_page_update) {
		info->link_version = cache->link_version;
	}
	for(const auto& link_key : cache->rem_links) {
		remove(info->links, link_key);
	}
	for(const auto& link_key : cache->add_links) {
		unique_push_back(info->links, link_key);
	}
	for(const auto& parent_key : cache->rem_reverse_links) {
		remove(info->reverse_links, parent_key);
	}
	for(const auto& parent_key : cache->add_reverse_links) {
		unique_push_back(info->reverse_links, parent_key);
	}
	info->reverse_domains.clear();
	for(const auto& link_key : info->reverse_links) {
		info->reverse_domains[Url::Url(link_key).host()]++;
	}
	job->result = info;
	
	add_task(std::bind(&SearchEngine::link_update_callback_1, this, job));
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
	array->last_update = std::time(0);
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
	const auto word_id = job->cached->word_id;
	
	std::unordered_set<uint32_t> rem_pages;
	std::unordered_set<uint32_t> new_pages;
	
	for(const auto page_id : job->cached->add_pages) {
		new_pages.insert(page_id);
	}
	for(const auto page_id : job->cached->rem_pages) {
		if(!new_pages.count(page_id)) {
			rem_pages.insert(page_id);
		}
	}
	if(job->context) {
		for(const auto page_id : job->context->pages) {
			if(!rem_pages.count(page_id)) {
				new_pages.insert(page_id);
			}
		}
	}
	
	std::vector<std::pair<float, uint32_t>> list;
	{
		std::shared_lock lock(index_mutex);
		
		size_t i = 0;
		for(const auto page_id : new_pages)
		{
			const auto* page = find_page(page_id);
			if(page) {
				list.emplace_back(page->rank_value, page_id);
			}
			if(i++ % 8192 == 8191) {
				lock.unlock();
				lock.lock();
			}
		}
		job->num_pages = list.size();
	}
	std::sort(list.begin(), list.end(), std::greater<std::pair<float, uint32_t>>());
	
	auto result = WordContext::create();
	result->id = word_id;
	result->last_update = std::time(0);
	result->pages.reserve(list.size());
	for(const auto& entry : list) {
		result->pages.emplace_back(entry.second);
	}
	job->result = result;
	
	add_task(std::bind(&SearchEngine::word_update_finished, this, job));
}


} // search
} // vnx
