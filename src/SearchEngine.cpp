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
#include <cmath>
#include <chrono>
#include <algorithm>


namespace vnx {
namespace search {

SearchEngine::SearchEngine(const std::string& _vnx_name)
	:	SearchEngineBase(_vnx_name)
{
	input_url_index_sync = vnx_name + ".url_index.sync";
	input_page_info_sync = vnx_name + ".page_info.sync";
	input_page_index_sync = vnx_name + ".page_index.sync";
	input_word_context_sync = vnx_name + ".word_context.sync";
	
	protocols.push_back("http");
	protocols.push_back("https");
}

void SearchEngine::init()
{
	vnx::open_pipe(vnx_name, this, 1000);
}

void SearchEngine::main()
{
	subscribe(input_url_index_sync, 100);
	subscribe(input_page_info_sync, 100);
	subscribe(input_page_index_sync, 100);
	subscribe(input_word_context_sync, 100);
	
	protocols = get_unique(protocols);
	
	module_page_info = new keyvalue::Server("PageInfo");
	module_page_info->collection = "page_info";
	module_page_info.start();
	
	module_word_context = new keyvalue::Server("WordContext");
	module_word_context->collection = "word_context";
	module_word_context.start();
	
	page_info_async = std::make_shared<keyvalue::ServerAsyncClient>("PageInfo");
	word_context_async = std::make_shared<keyvalue::ServerAsyncClient>("WordContext");
	url_index_async = std::make_shared<keyvalue::ServerAsyncClient>(url_index_server);
	page_index_sync = std::make_shared<keyvalue::ServerClient>(page_index_server);
	
	add_async_client(page_info_async);
	add_async_client(word_context_async);
	add_async_client(url_index_async);
	
	set_timer_millis(100, std::bind(&SearchEngine::link_update, this));
	set_timer_millis(stats_interval_ms, std::bind(&SearchEngine::print_stats, this));
	
	page_info_async->sync_all(input_page_info_sync);
	word_context_async->sync_all(input_word_context_sync);
	
	query_threads.resize(num_query_threads);
	for(int i = 0; i < num_query_threads; ++i) {
		query_threads[i] = std::thread(&SearchEngine::query_loop, this);
	}
	
	update_threads.resize(num_update_threads);
	for(int i = 0; i < num_update_threads; ++i) {
		update_threads[i] = std::thread(&SearchEngine::update_loop, this);
	}
	
	Super::main();
	
	query_condition.notify_all();
	for(auto& thread : query_threads) {
		if(thread.joinable()) {
			thread.join();
		}
	}
	
	update_condition.notify_all();
	for(auto& thread : update_threads) {
		if(thread.joinable()) {
			thread.join();
		}
	}
}

void SearchEngine::query_async(	const std::vector<std::string>& words,
								const int32_t& limit, const uint32_t& offset,
								const std::vector<search_flags_e>& flags,
								const vnx::request_id_t& req_id) const
{
	auto request = std::make_shared<query_t>();
	request->words = words;
	request->limit = limit;
	request->offset = offset;
	request->flags = flags;
	request->req_id = req_id;
	{
		std::lock_guard lock(query_mutex);
		query_queue.push(request);
	}
	query_condition.notify_one();
}

Object SearchEngine::get_domain_info(	const std::string& host,
										const int32_t& limit,
										const uint32_t& offset) const
{
	Object result;
	const auto iter = domain_map.find(host);
	if(iter != domain_map.end()) {
		const auto* domain = find_domain(iter->second);
		if(domain) {
			std::vector<std::pair<size_t, const page_t*>> sorted;
			for(const auto page_id : domain->pages) {
				const auto* page = find_page(page_id);
				if(page) {
					sorted.emplace_back(page->reverse_domains.size(), page);
				}
			}
			std::sort(sorted.begin(), sorted.end(), std::greater<std::pair<size_t, const page_t*>>());
			
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
	return result;
}

void SearchEngine::get_page_info_async(const std::string& url_key, const request_id_t& req_id) const
{
	page_info_async->get_value(Variant(url_key),
			std::bind(&SearchEngine::get_page_info_callback, this, url_key, std::placeholders::_1, req_id));
}

void SearchEngine::get_page_info_callback(	const std::string& url_key,
											std::shared_ptr<const Value> value,
											const request_id_t& req_id) const
{
	Object result;
	const auto page_id = find_url_id(url_key);
	const auto* page = find_page(page_id);
	if(page) {
		result["url"] = page->get_url();
		result["is_loaded"] = page->is_loaded;
		result["first_seen"] = page->first_seen;
		result["last_modified"] = page->last_modified;
		{
			const auto* domain = find_domain(page->domain_id);
			if(domain) {
				result["domain"] = domain->host.str();
			}
		}
		std::vector<std::string> reverse_domains;
		for(const auto domain_id : page->reverse_domains) {
			const auto* domain = find_domain(domain_id);
			if(domain) {
				reverse_domains.push_back(domain->host.str());
			}
		}
		result["reverse_domains"] = reverse_domains;
	}
	const auto page_info = std::dynamic_pointer_cast<const PageInfo>(value);
	if(page_info) {
		result["title"] = page_info->title;
		std::vector<std::string> links;
		std::vector<std::string> reverse_links;
		for(const auto link_id : page_info->links) {
			const auto* child = find_page(link_id);
			if(child) {
				links.push_back(child->get_url());
			}
		}
		for(const auto link_id : page_info->reverse_links) {
			const auto* parent = find_page(link_id);
			if(parent) {
				reverse_links.push_back(parent->get_url());
			}
		}
		result["links"] = links;
		result["reverse_links"] = reverse_links;
	}
	get_page_info_async_return(req_id, result);
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
			std::bind(&SearchEngine::reverse_lookup_callback, this, url_key, std::placeholders::_1, req_id));
}

void SearchEngine::reverse_lookup_callback(	const std::string& url_key,
											std::shared_ptr<const Value> value,
											const request_id_t& req_id) const
{
	std::vector<std::pair<std::string, size_t>> sorted;
	const auto page_info = std::dynamic_pointer_cast<const PageInfo>(value);
	if(page_info) {
		for(const auto link_id : page_info->reverse_links) {
			const auto* parent = find_page(link_id);
			if(parent) {
				sorted.emplace_back(parent->url_key, parent->reverse_domains.size());
			}
		}
	}
	std::sort(sorted.begin(), sorted.end(),
			[] (const std::pair<std::string, size_t>& A, const std::pair<std::string, size_t>& B) -> bool {
				return A.second > B.second;
			});
	std::vector<std::string> result;
	for(const auto& entry : sorted) {
		result.push_back(entry.first);
	}
	reverse_lookup_async_return(req_id, result);
}

void SearchEngine::reverse_domain_lookup_async(	const std::string& url_key,
												const request_id_t& req_id) const
{
	page_info_async->get_value(Variant(url_key),
			std::bind(&SearchEngine::reverse_domain_lookup_callback, this, url_key, std::placeholders::_1, req_id));
}

void SearchEngine::reverse_domain_lookup_callback(	const std::string& url_key,
													std::shared_ptr<const Value> value,
													const request_id_t& req_id) const
{
	std::map<std::string, uint32_t> tmp;
	const auto page_info = std::dynamic_pointer_cast<const PageInfo>(value);
	if(page_info) {
		for(const auto link_id : page_info->reverse_links) {
			const auto* parent = find_page(link_id);
			if(parent) {
				const auto* domain = find_domain(parent->domain_id);
				if(domain) {
					tmp[domain->host.str()]++;
				}
			}
		}
	}
	std::vector<std::pair<std::string, uint32_t>> sorted(tmp.begin(), tmp.end());
	std::sort(sorted.begin(), sorted.end(),
			[] (const std::pair<std::string, uint32_t>& A, const std::pair<std::string, uint32_t>& B) -> bool {
				return A.second > B.second;
			});
	reverse_domain_lookup_async_return(req_id, sorted);
}

std::vector<std::string> SearchEngine::suggest_words(const std::string& prefix, const int32_t& limit) const
{
	std::shared_lock lock(index_mutex);
	
	std::vector<std::string> result;
	for(auto it = word_map.lower_bound(prefix); it != word_map.end() && result.size() < size_t(limit); ++it) {
		result.push_back(it->first.str());
	}
	return result;
}

std::vector<std::string> SearchEngine::suggest_domains(const std::string& prefix, const int32_t& limit) const
{
	std::shared_lock lock(index_mutex);
	
	std::vector<std::string> result;
	for(auto it = domain_map.lower_bound(prefix); it != domain_map.end() && result.size() < size_t(limit); ++it) {
		result.push_back(it->first.str());
	}
	return result;
}

uint32_t SearchEngine::find_url_id(const std::string& url_key) const
{
	auto iter = url_map.find(url_key);
	if(iter != url_map.end()) {
		return iter->second;
	}
	return 0;
}

uint32_t SearchEngine::get_url_id(const std::string& url_key)
{
	uint32_t id = find_url_id(url_key);
	if(id == 0) {
		if(free_url_ids.empty()) {
			id = next_url_id++;
		} else {
			id = free_url_ids.back();
			free_url_ids.pop_back();
		}
		url_map[url_key] = id;
	}
	return id;
}

SearchEngine::page_t* SearchEngine::find_page(uint32_t url_id)
{
	const auto iter = page_index.find(url_id);
	if(iter != page_index.end()) {
		return &iter->second;
	}
	return 0;
}

const SearchEngine::page_t* SearchEngine::find_page(uint32_t url_id) const
{
	const auto iter = page_index.find(url_id);
	if(iter != page_index.end()) {
		return &iter->second;
	}
	return 0;
}

template<typename T>
SearchEngine::word_t& SearchEngine::get_word(const T& word)
{
	const auto iter = word_map.find(word);
	if(iter != word_map.end()) {
		return word_index[iter->second];
	}
	else {
		const auto id = next_word_id++;
		word_map[word] = id;
		
		auto& entry = word_index[id];
		entry.id = id;
		entry.value = word;
		return entry;
	}
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
		link_queue.emplace(vnx::get_wall_time_micros(), url_key);
	}
	return cache;
}

std::shared_ptr<SearchEngine::link_cache_t> SearchEngine::get_link_cache(uint32_t page_id)
{
	const auto* page = find_page(page_id);
	if(page) {
		return get_link_cache(page->url_key);
	}
	return 0;
}

std::shared_ptr<SearchEngine::word_cache_t> SearchEngine::get_word_cache(uint32_t word_id)
{
	// update_mutex needs to be unique locked
	auto& cache = word_cache[word_id];
	if(!cache) {
		cache = std::make_shared<word_cache_t>();
		cache->word_id = word_id;
		word_queue.emplace(vnx::get_wall_time_micros(), word_id);
	}
	return cache;
}

void SearchEngine::delete_page_async(const std::string& url_key)
{
	page_info_async->get_value(Variant(url_key),
			std::bind(&SearchEngine::delete_page_callback, this, url_key, std::placeholders::_1));
}

void SearchEngine::delete_page_callback(const std::string& url_key,
										std::shared_ptr<const Value> value)
{
	const auto page_info = std::dynamic_pointer_cast<const PageInfo>(value);
	if(!page_info || page_info->id == 0) {
		return;
	}
	if(page_info->link_version)
	{
		const auto p_link_cache = get_link_cache(page_info->id);
		if(p_link_cache)
		{
			p_link_cache->version = 0;
			p_link_cache->is_update = true;
			p_link_cache->add_links.clear();
			p_link_cache->rem_links = page_info->links;
			
			for(const auto child_id : page_info->links)
			{
				const auto cache = get_link_cache(child_id);
				if(cache) {
					cache->rem_reverse_links.push_back(page_info->id);
				}
			}
			for(const auto parent_id : page_info->reverse_links)
			{
				const auto cache = get_link_cache(parent_id);
				if(cache) {
					cache->rem_links.push_back(page_info->id);
				}
			}
			p_link_cache->schedule_time_us = vnx::get_wall_time_micros();
		}
	}
	if(page_info->word_version)
	{
		std::lock_guard lock(update_mutex);
		
		auto& r_page_cache = page_cache[page_info->id];
		r_page_cache.version = 0;
		r_page_cache.words.clear();
		
		for(const auto word_id : page_info->words)
		{
			const auto cache = get_word_cache(word_id);
			if(cache) {
				cache->rem_pages.push_back(page_info->id);
				r_page_cache.words_pending++;
			}
		}
	}
	{
		std::unique_lock lock(index_mutex);
		page_index.erase(page_info->id);
	}
}

void SearchEngine::redirect_callback(	const std::string& org_url_key,
										const std::string& new_url_key,
										std::shared_ptr<const Value> value)
{
	std::unique_lock lock(index_mutex);
	
	const auto page_info = std::dynamic_pointer_cast<const PageInfo>(value);
	const auto new_page_id = get_url_id(new_url_key);
	
	if(page_info)
	{
		auto* new_page = find_page(new_page_id);
		const auto p_new_cache = get_link_cache(new_url_key);
		
		for(const auto parent_id : page_info->reverse_links)
		{
			if(new_page) {
				auto* parent = find_page(parent_id);
				if(parent && parent->is_loaded) {
					unique_push_back(new_page->reverse_domains, parent->domain_id);
				}
			}
			const auto cache = get_link_cache(parent_id);
			if(cache) {
				cache->add_links.push_back(new_page_id);
			}
			p_new_cache->add_reverse_links.push_back(parent_id);
		}
		p_new_cache->schedule_time_us = vnx::get_wall_time_micros();
		
		delete_page_async(org_url_key);
	}
	redirects[org_url_key] = new_page_id;
}

void SearchEngine::handle(std::shared_ptr<const keyvalue::KeyValuePair> pair)
{
	auto info = std::dynamic_pointer_cast<const PageInfo>(pair->value);
	if(info) {
		if(info->id)
		{
			std::unique_lock lock(index_mutex);
			
			const auto url_key = pair->key.to_string_value();
			url_map[url_key] = info->id;
			next_url_id = std::max(next_url_id, info->id + 1);
			
			auto& page = page_index[info->id];
			page.id = info->id;
			page.url_key = url_key;
			page.scheme = info->scheme;
		}
		return;
	}
	
	auto url_index = std::dynamic_pointer_cast<const UrlIndex>(pair->value);
	if(url_index) {
		if(url_index->fetch_count == 0) {
			return;
		}
		std::unique_lock lock(index_mutex);
		
		bool is_redirect = false;
		const auto org_url_key = pair->key.to_string_value();
		
		if(!url_index->redirect.empty())
		{
			const auto new_url_key = get_url_key(url_index->redirect);
			
			if(new_url_key != org_url_key)
			{
				page_info_async->get_value(Variant(org_url_key),
						std::bind(&SearchEngine::redirect_callback, this, org_url_key, new_url_key, std::placeholders::_1));
				
				is_redirect = true;
			}
		}
		if(!is_redirect)
		{
			const auto page_id = find_url_id(org_url_key);
			if(page_id) {
				auto* page = find_page(page_id);
				if(page) {
					page->scheme = url_index->scheme;
					page->first_seen = url_index->first_seen;
					page->last_modified = url_index->last_modified;
				}
			}
			redirects.erase(org_url_key);
		}
		return;
	}
	
	auto word_context = std::dynamic_pointer_cast<const WordContext>(pair->value);
	if(word_context)
	{
		std::unique_lock lock(index_mutex);
		
		const stx::sstring key = pair->key.to_string_value();
		word_map[key] = word_context->id;
		
		word_t& word = word_index[word_context->id];
		word.id = word_context->id;
		word.num_pages = word_context->pages.size();
		word.value = key;
		next_word_id = std::max(next_word_id, word_context->id + 1);
		return;
	}
	
	if(pair->collection == "page_index")
	{
		const auto url_key = pair->key.to_string_value();
		const auto page_id = find_url_id(url_key);
		const auto iter = page_index.find(page_id);
		
		auto index = std::dynamic_pointer_cast<const PageIndex>(pair->value);
		if(index) {
			bool do_load = true;
			if(iter != page_index.end()) {
				auto& page = iter->second;
				page.is_deleted = false;
				// make sure we don't overwrite with an older version
				do_load = !page.is_loaded || pair->version > page.version;
			}
			if(do_load) {
				url_index_async->get_value(Variant(url_key),
						std::bind(&SearchEngine::update_page_callback_1, this, url_key, pair->version, index, std::placeholders::_1));
			}
		} else {
			if(iter != page_index.end()) {
				delete_page_async(url_key);
			}
		}
		return;
	}
}

void SearchEngine::handle(std::shared_ptr<const keyvalue::SyncInfo> value)
{
	std::unique_lock lock(index_mutex);
	
	if(value->code == keyvalue::SyncInfo::END)
	{
		init_sync_count++;
		if(value->collection == "page_info")
		{
			std::vector<bool> is_used(next_url_id);
			for(const auto& entry : url_map) {
				is_used[entry.second] = true;
			}
			for(uint32_t i = next_url_id - 1; i >= 1; --i) {
				if(!is_used[i]) {
					free_url_ids.push_back(i);
				}
			}
			log(INFO).out << "Found " << free_url_ids.size() << " free IDs";
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
			page_index_sync->sync_all(input_page_index_sync);
			log(INFO).out << "Starting PageIndex sync ...";
		}
		if(init_sync_count == 4)
		{
			size_t num_purged = 0;
			for(const auto& entry : page_index) {
				const auto& page = entry.second;
				if(page.is_deleted) {
					delete_page_async(page.url_key);
					num_purged++;
				}
			}
			log(INFO).out << "Purged " << num_purged << " deleted pages.";
			
			is_initialized = true;
			
			log(INFO).out << "Initialized with " << url_map.size() << " urls, " << domain_map.size() << " domains, "
					<< page_index.size() << " pages and " << word_map.size() << " words.";
		}
	}
}

void SearchEngine::update_page_callback_1(	const std::string& url_key,
											const uint64_t version,
											std::shared_ptr<const PageIndex> index,
											std::shared_ptr<const Value> value)
{
	auto url_index = std::dynamic_pointer_cast<const UrlIndex>(value);
	if(url_index) {
		if(url_index->depth >= 0) {
			page_info_async->get_value_locked(Variant(url_key), lock_timeout * 1000,
					std::bind(&SearchEngine::update_page_callback_2, this, url_key, version, index, url_index, std::placeholders::_1));
		}
	}
}

void SearchEngine::update_page_callback_2(	const std::string& url_key,
											const uint64_t version,
											std::shared_ptr<const PageIndex> index,
											std::shared_ptr<const UrlIndex> url_index,
											std::pair<Variant, std::shared_ptr<const Value>> pair)
{
	auto page_info = std::dynamic_pointer_cast<const PageInfo>(pair.second);
	update_page(url_key, version, index, page_info, url_index);
}

void SearchEngine::update_page(	const std::string& url_key,
								const uint64_t version,
								std::shared_ptr<const PageIndex> index,
								std::shared_ptr<const PageInfo> page_info,
								std::shared_ptr<const UrlIndex> url_index)
{
	std::unique_lock lock(index_mutex);
	
	const Url::Url parsed_url_key(url_key);
	const auto page_id = get_url_id(url_key);
	
	auto& page = page_index[page_id];
	page.id = page_id;
	page.is_deleted = false;
	page.version = version;
	page.scheme = url_index->scheme;
	page.url_key = url_key;
	page.first_seen = url_index->first_seen;
	page.last_modified = url_index->last_modified;
	
	if(!page.domain_id) {
		auto& domain = get_domain(parsed_url_key.host());
		page.domain_id = domain.id;
		domain.pages.push_back(page.id);
	}
	
	if(!page.is_loaded)
	{
		// populate reverse_domains
		std::vector<uint32_t> links;
		if(page_info) {
			append(links, page_info->links);
		}
		{
			const auto iter = link_cache.find(url_key);
			if(iter != link_cache.end()) {
				append(links, iter->second->add_links);
			}
		}
		for(const auto child_id : links)
		{
			auto* child = find_page(child_id);
			if(child) {
				unique_push_back(child->reverse_domains, page.domain_id);
			}
		}
	}
	page.is_loaded = true;
	
	if(!page_info || version > page_info->version)
	{
		// initialize or update page_info
		auto copy = page_info ? vnx::clone(page_info) : PageInfo::create();
		copy->id = page_id;
		copy->version = version;
		copy->scheme = url_index->scheme;
		copy->title = index->title;
		page_info_async->store_value(Variant(url_key), copy);
	}
	else {
		page_info_async->unlock(Variant(url_key));
	}
	
	if(!page_info || version > page_info->link_version)
	{
		std::unordered_set<uint32_t> new_links;
		const auto p_link_cache = get_link_cache(url_key);
		p_link_cache->version = version;
		p_link_cache->is_update = true;
		
		for(const auto& link_url : index->links) {
			try {
				const Url::Url parsed_link(link_url);
				const auto link_url_key = get_url_key(parsed_link);
				
				if(std::find(protocols.begin(), protocols.end(), parsed_link.scheme()) == protocols.end()) {
					continue;
				}
				uint32_t link_id = 0;
				{
					const auto iter = redirects.find(link_url_key);
					if(iter != redirects.end()) {
						link_id = iter->second;
					} else {
						link_id = find_url_id(link_url_key);
					}
				}
				if(link_id) {
					if(link_id != page.id) {
						// link to existing page
						new_links.insert(link_id);
					}
				} else {
					// link to not yet loaded page
					const auto cached = get_link_cache(link_url_key);
					if(cached) {
						cached->add_reverse_links.push_back(page.id);
					}
				}
			} catch(...) {
				// ignore bad links
			}
		}
		if(page_info)
		{
			// check for new or removed links
			for(const auto link_id : page_info->links)
			{
				const auto iter = new_links.find(link_id);
				if(iter != new_links.end()) {
					// link already exists
					new_links.erase(iter);
				} else {
					// link was removed
					const auto cached = get_link_cache(link_id);
					if(cached) {
						cached->rem_reverse_links.push_back(page.id);
					}
					p_link_cache->rem_links.push_back(link_id);
				}
			}
		}
		for(const auto link_id : new_links)
		{
			// make new links
			const auto cached = get_link_cache(link_id);
			if(cached) {
				cached->add_reverse_links.push_back(page.id);
			}
			auto* child = find_page(link_id);
			if(child) {
				unique_push_back(child->reverse_domains, page.domain_id);
			}
			p_link_cache->add_links.push_back(link_id);
		}
		if(!page_info || page_info->id == 0)
		{
			// new page was loaded
			auto reverse_links = p_link_cache->add_reverse_links;
			if(page_info) {
				append(reverse_links, page_info->reverse_links);
			}
			for(const auto parent_id : reverse_links)
			{
				auto* parent = find_page(parent_id);
				if(parent && parent->is_loaded) {
					unique_push_back(page.reverse_domains, parent->domain_id);
				}
				const auto cached = get_link_cache(parent_id);
				if(cached) {
					cached->add_links.push_back(page.id);
				}
			}
		}
		// make sure our link cache is saved last
		p_link_cache->schedule_time_us = vnx::get_wall_time_micros();
	}
	
	// no other thread may try to lock index_mutex while holding a lock on update_mutex
	std::lock_guard lock_1(update_mutex);
	
	// update word index if version is greater and previous update has finished
	if((!page_info || version > page_info->word_version) && !page_cache.count(page.id))
	{
		struct word_info_t {
			short mode = 0;
			uint16_t count = 0;
		};
		
		std::unordered_map<uint32_t, word_info_t> words;
		const float inv_word_count = 1.f / index->word_count;
		
		for(const auto& entry : index->words)
		{
			const auto word_id = get_word(entry.first).id;
			auto& info = words[word_id];
			info.mode++;
			info.count = entry.second;
		}
		if(page_info) {
			for(const auto word_id : page_info->words) {
				if(word_index.count(word_id)) {
					words[word_id].mode--;
				}
			}
		}
		
		page_cache_t& r_page_cache = page_cache[page_id];
		r_page_cache.version = version;
		
		for(const auto& entry : words)
		{
			const auto word_id = entry.first;
			const auto p_word_cache = get_word_cache(word_id);
			if(entry.second.mode >= 0) {
				p_word_cache->add_pages.emplace_back(page_id, entry.second.count * inv_word_count);
				r_page_cache.words_pending++;
			}
			if(entry.second.mode <= 0) {
				p_word_cache->rem_pages.emplace_back(page_id);
				r_page_cache.words_pending++;
			}
		}
	}
}

void SearchEngine::link_update()
{
	const auto now_us = vnx::get_wall_time_micros();
	for(auto iter = link_queue.begin(); iter != link_queue.end();)
	{
		if((now_us - iter->first) / 1000000 >= link_commit_interval)
		{
			const auto cache = link_cache[iter->second];
			if(cache->schedule_time_us) {
				link_queue.emplace(cache->schedule_time_us, iter->second);
				cache->schedule_time_us = 0;
			} else {
				const auto url_key = iter->second;
				page_info_async->get_value_locked(Variant(url_key), lock_timeout * 1000,
						std::bind(&SearchEngine::link_commit, this, url_key, std::placeholders::_1));
			}
			iter = link_queue.erase(iter);
		} else {
			iter++;
		}
	}
}

void SearchEngine::link_commit(	const std::string& url_key,
								std::pair<Variant, std::shared_ptr<const Value>> pair)
{
	const auto iter = link_cache.find(url_key);
	if(iter == link_cache.end()) {
		page_info_async->unlock(pair.first);
		return;
	}
	const auto cache = iter->second;
	
	auto page_info = std::dynamic_pointer_cast<PageInfo>(vnx::clone(pair.second));
	if(!page_info) {
		page_info = PageInfo::create();
	}
	if(cache->is_update) {
		page_info->link_version = cache->version;
	}
	for(const auto link_id : cache->rem_links) {
		remove(page_info->links, link_id);
	}
	for(const auto link_id : cache->add_links) {
		unique_push_back(page_info->links, link_id);
	}
	for(const auto page_id : cache->rem_reverse_links) {
		remove(page_info->reverse_links, page_id);
	}
	for(const auto page_id : cache->add_reverse_links) {
		unique_push_back(page_info->reverse_links, page_id);
	}
	link_cache.erase(iter);
	
	page_info_async->store_value(pair.first, page_info);
	page_update_counter++;
}

void SearchEngine::word_update_finished(const uint32_t& page_id)
{
	const auto* page = find_page(page_id);
	if(page) {
		const Variant url_key(page->url_key);
		page_info_async->get_value_locked(url_key, lock_timeout * 1000,
			std::bind(&SearchEngine::word_update_callback, this, page_id, std::placeholders::_1));
	}
}

void SearchEngine::word_update_callback(const uint32_t& page_id,
										std::pair<Variant, std::shared_ptr<const Value>> pair)
{
	std::lock_guard lock(update_mutex);
	
	bool is_fail = true;
	const auto iter = page_cache.find(page_id);
	if(iter != page_cache.end())
	{
		const auto& r_page_cache = iter->second;
		if(r_page_cache.words_pending == 0)
		{
			const auto info = std::dynamic_pointer_cast<PageInfo>(vnx::clone(pair.second));
			if(info) {
				info->word_version = r_page_cache.version;
				info->words = r_page_cache.words;
				page_info_async->store_value(pair.first, info);
				page_update_counter++;
				is_fail = false;
			}
			page_cache.erase(iter);
		}
	}
	if(is_fail) {
		page_info_async->unlock(pair.first);
	}
}

void SearchEngine::print_stats()
{
	log(INFO).out << (60000 * word_update_counter) / stats_interval_ms << " words/min, "
			<< (60000 * page_update_counter) / stats_interval_ms << " pages/min, "
			<< (60000 * query_counter) / stats_interval_ms << " query/min, "
			<< domain_index.size() << " domains, "
			<< link_cache.size() << " / " << page_index.size() << " pages, "
			<< word_cache.size() << " / " << word_map.size() << " words, "
			<< redirects.size() << " redirects";
	
	word_update_counter = 0;
	page_update_counter = 0;
	query_counter = 0;
}

static
bool has_flag(const std::vector<search_flags_e>& flags, search_flags_e flag)
{
	return std::find(flags.begin(), flags.end(), flag) != flags.end();
}

void SearchEngine::query_loop() const noexcept
{
	keyvalue::ServerClient page_info_sync("PageInfo");
	keyvalue::ServerClient word_context_sync("WordContext");
	
	while(vnx_do_run())
	{
		std::shared_ptr<query_t> request;
		{
			std::unique_lock<std::mutex> lock(query_mutex);
			while(vnx_do_run() && query_queue.empty()) {
				query_condition.wait(lock);
			}
			if(vnx_do_run()) {
				request = query_queue.front();
				query_queue.pop();
			} else {
				break;
			}
		}
		
		auto result = SearchResult::create();
		result->is_fail = true;
		
		std::vector<std::shared_ptr<const WordContext>> context;
		try {
			const auto time_begin = vnx::get_wall_time_micros();
			const auto words = get_unique(request->words);
			const std::vector<Variant> keys(words.begin(), words.end());
			const auto values = word_context_sync.get_values(keys);
			int i = 0;
			for(auto value : values) {
				auto word_context = std::dynamic_pointer_cast<const WordContext>(value);
				if(word_context) {
					context.push_back(word_context);
					result->words.push_back(words.at(i));
				}
				i++;
			}
			result->load_time_us = vnx::get_wall_time_micros() - time_begin;
		}
		catch(const std::exception& ex) {
			result->error_msg = ex.what();
			query_async_return(request->req_id, result);
			continue;
		}
		const auto time_begin = vnx::get_wall_time_micros();
		
		const uint32_t num_words = context.size();
		if(num_words == 0) {
			query_async_return(request->req_id, result);
			continue;
		}
		
		size_t num_found = 0;
		std::vector<std::pair<uint32_t, uint32_t>> found(max_query_pages);
		
		if(num_words > 1)
		{
			std::unordered_map<uint32_t, std::pair<uint32_t, uint32_t>> page_hits;
			std::vector<std::vector<std::pair<uint32_t, uint16_t>>::const_iterator> iter(num_words);
			std::vector<std::vector<std::pair<uint32_t, uint16_t>>::const_iterator> end(num_words);
			for(uint32_t i = 0; i < num_words; ++i) {
				iter[i] = context[i]->pages.begin();
				end[i] = context[i]->pages.end();
			}
			uint32_t k = 0;
			uint32_t num_iter = num_words;
			
			while(num_iter > 0 && num_found < found.size())
			{
				for(int i = 0; iter[k] != end[k] && i < 10; ++iter[k], ++i)
				{
					const auto page_id = iter[k]->first;
					const uint32_t weight = iter[k]->second;
					auto& entry = page_hits[page_id];
					if(entry.first == 0) {
						entry.second = weight;
					} else {
						entry.second = std::min(weight, entry.second);
					}
					if(++entry.first == num_words) {
						const auto index = num_found++;
						if(index < found.size()) {
							found[index] = std::make_pair(page_id, entry.second);
						}
					}
				}
				if(iter[k] == end[k]) {
					iter.erase(iter.begin() + k);
					end.erase(end.begin() + k);
					num_iter--;
				} else {
					k++;
				}
				if(k >= num_iter) {
					k = 0;
				}
			}
		}
		else {
			const auto& list = context[0]->pages;
			num_found = std::min(found.size(), list.size());
			for(size_t i = 0; i < num_found; ++i) {
				found[i] = list[i];
			}
		}
		
		struct result_t : result_item_t {
			uint32_t domain_id = 0;
		};
		
		std::vector<result_t> results;
		{
			std::shared_lock lock(index_mutex);
			
			for(size_t i = 0; i < std::min(num_found, found.size()); ++i)
			{
				const auto& entry = found[i];
				const auto* page = find_page(entry.first);
				if(page && page->is_loaded)
				{
					results.push_back(result_t());
					result_t& item = results.back();
					item.url = page->get_url();
					item.domain_id = page->domain_id;
					item.last_modified = page->last_modified;
					item.score = entry.second * page->reverse_domains.size();
				}
			}
		}
		
		std::vector<std::pair<int64_t, const result_t*>> sorted;
		
		if(has_flag(request->flags, search_flags_e::GROUP_BY_DOMAIN))
		{
			std::unordered_map<uint32_t, std::pair<int64_t, const result_t*>> best_of;
			
			for(const auto& item : results) {
				auto& current = best_of[item.domain_id];
				if(!current.second || item.score > current.first) {
					current.first = item.score;
					current.second = &item;
				}
			}
			for(const auto& entry : best_of) {
				sorted.emplace_back(entry.second.first, entry.second.second);
			}
		}
		else {
			for(const auto& result : results) {
				sorted.emplace_back(result.score, &result);
			}
		}
		
		std::sort(sorted.begin(), sorted.end(), std::greater<std::pair<int64_t, const result_t*>>());
		
		const auto time_mid = vnx::get_wall_time_micros();
		result->compute_time_us = time_mid - time_begin;
		
		result->items.clear();
		try {
			std::vector<Variant> url_keys;
			for(uint32_t i = 0; i < uint32_t(request->limit) && request->offset + i < sorted.size(); ++i)
			{
				const auto* item = sorted[request->offset + i].second;
				url_keys.push_back(Variant(get_url_key(item->url)));
				result->items.push_back(*item);
			}
			const auto values = page_info_sync.get_values(url_keys);
			for(size_t i = 0; i < values.size(); ++i) {
				auto page_info = std::dynamic_pointer_cast<const PageInfo>(values[i]);
				if(page_info) {
					result->items[i].title = page_info->title;
				}
			}
		}
		catch(const std::exception& ex) {
			log(WARN).out << "query(): " << ex.what();
		}
		const auto time_end = vnx::get_wall_time_micros();
		
		result->num_results_total = sorted.size();
		result->load_time_us += time_end - time_mid;
		result->is_fail = false;
		query_async_return(request->req_id, result);
		
		query_counter++;
	}
}

void SearchEngine::update_loop() noexcept
{
	keyvalue::ServerClient word_context_sync("WordContext");
	
	bool do_wait = false;
	
	while(vnx_do_run())
	{
		uint32_t word_id = 0;
		std::shared_ptr<word_cache_t> p_word_cache;
		{
			std::unique_lock<std::mutex> lock(update_mutex);
			if(vnx_do_run() && do_wait) {
				update_condition.wait_for(lock, std::chrono::milliseconds(1000));
			}
			do_wait = true;
			
			if(vnx_do_run()) {
				if(word_queue.empty()) {
					continue;
				}
				const auto& entry = word_queue.front();
				const auto delta = (vnx::get_wall_time_micros() - entry.first) / 1000000;
				
				if(delta > word_commit_interval || word_cache.size() > max_word_cache)
				{
					word_id = entry.second;
					p_word_cache = word_cache[word_id];
					word_cache.erase(word_id);
					word_queue.pop();
				} else {
					continue;
				}
			} else {
				break;
			}
		}
		do_wait = false;
		
		std::string word;
		{
			std::shared_lock lock(index_mutex);
			
			const auto iter = word_index.find(word_id);
			if(iter != word_index.end()) {
				word = iter->second.value.str();
			} else {
				log(WARN).out << "update_loop(): invalid word id: " << word_id;
				continue;
			}
		}
		
		std::shared_ptr<const WordContext> context;
		try {
			const auto value = word_context_sync.get_value(Variant(word));
			context = std::dynamic_pointer_cast<const WordContext>(value);
		}
		catch(const std::exception& ex) {
			log(WARN).out << "update_loop(): " << ex.what();
			continue;
		}
		
		std::unordered_set<uint32_t> rem_pages;
		std::unordered_map<uint32_t, uint16_t> new_pages;
		
		for(const auto& entry : p_word_cache->add_pages)
		{
			const float tmp = powf(entry.second, word_power);
			new_pages.emplace(entry.first, std::max(std::min(int(tmp * (0xFFFF * word_cutoff)), 0xFFFF), 1));
		}
		for(const auto page_id : p_word_cache->rem_pages)
		{
			if(!new_pages.count(page_id)) {
				rem_pages.insert(page_id);
			}
		}
		if(context) {
			for(const auto& entry : context->pages)
			{
				const auto page_id = entry.first;
				if(!rem_pages.count(page_id) && !new_pages.count(page_id))
				{
					new_pages.emplace(entry);
				}
			}
		}
		
		std::vector<std::pair<uint64_t, std::pair<uint32_t, uint16_t>>> list;
		{
			std::shared_lock lock(index_mutex);
			
			for(const auto& entry : new_pages)
			{
				const auto* page = find_page(entry.first);
				if(page) {
					const auto weight = entry.second;
					list.emplace_back(uint64_t(weight) * page->reverse_domains.size(), entry);
				}
			}
			
			const auto iter = word_index.find(word_id);
			if(iter != word_index.end()) {
				iter->second.num_pages = list.size();
			}
		}
		std::sort(list.begin(), list.end(), std::greater<std::pair<uint64_t, std::pair<uint32_t, uint16_t>>>());
		
		try {
			auto value = WordContext::create();
			value->id = word_id;
			value->last_update = std::time(0);
			value->pages.reserve(list.size());
			for(const auto& entry : list) {
				value->pages.emplace_back(entry.second);
			}
			word_context_sync.store_value(Variant(word), value);
			
			std::unordered_map<uint32_t, int> page_mode;
			for(const auto& entry : p_word_cache->add_pages) {
				page_mode[entry.first]++;
			}
			for(const auto page_id : p_word_cache->rem_pages) {
				page_mode[page_id]--;
			}
			
			{
				std::lock_guard lock(update_mutex);
				
				for(const auto& entry : page_mode)
				{
					const auto page_id = entry.first;
					const auto iter = page_cache.find(page_id);
					if(iter != page_cache.end())
					{
						auto& r_page_cache = iter->second;
						if(entry.second >= 0) {
							r_page_cache.words.push_back(word_id);
							r_page_cache.words_pending--;
						}
						if(entry.second <= 0) {
							r_page_cache.words_pending--;
						}
						if(r_page_cache.words_pending == 0) {
							add_task(std::bind(&SearchEngine::word_update_finished, this, page_id));
						}
					}
				}
			}
			word_update_counter++;
		}
		catch(const std::exception& ex) {
			log(WARN).out << "update_loop(): " << ex.what();
			continue;
		}
	}
}


} // search
} // vnx
