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
	
	set_timer_millis(stats_interval_ms, std::bind(&SearchEngine::print_stats, this));
	
	page_info_async->sync_all(input_page_info_sync);
	word_context_async->sync_all_keys(input_word_context_sync);
	
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
								const std::function<void(const std::shared_ptr<const SearchResult>&)>& _callback,
								const vnx::request_id_t& _request_id) const
{
	auto request = std::make_shared<query_t>();
	request->words = words;
	request->limit = limit;
	request->offset = offset;
	request->flags = flags;
	request->callback = _callback;
	{
		std::lock_guard lock(query_mutex);
		query_queue.push(request);
	}
	query_condition.notify_one();
}

std::shared_ptr<const DomainIndex> SearchEngine::get_domain_info(	const std::string& host,
																	const int32_t& limit,
																	const uint32_t& offset) const
{
	std::shared_lock lock(index_mutex);
	
	auto iter = domain_map.find(host);
	if(iter != domain_map.end()) {
		auto iter2 = domain_index.find(iter->second);
		if(iter2 != domain_index.end()) {
			const auto& domain = iter2->second;
			
			std::vector<std::pair<size_t, const page_t*>> sorted;
			for(const auto page_id : domain.pages) {
				auto iter3 = page_index.find(page_id);
				if(iter3 != page_index.end()) {
					const auto& page = iter3->second;
					sorted.emplace_back(page.reverse_links.size(), &page);
				}
			}
			std::sort(sorted.begin(), sorted.end(), std::greater<std::pair<size_t, const page_t*>>());
			
			auto result = DomainIndex::create();
			result->host = host;
			result->num_pages = sorted.size();
			for(uint32_t i = 0; i < uint32_t(limit) && offset + i < sorted.size(); ++i) {
				result->pages.push_back(sorted[offset + i].second->url_key);
			}
			return result;
		}
	}
	return 0;
}

std::vector<DomainIndex> SearchEngine::get_domain_list(const int32_t& limit, const uint32_t& offset) const
{
	std::shared_lock lock(index_mutex);
	
	std::vector<std::pair<size_t, const domain_t*>> sorted;
	for(const auto& entry : domain_index) {
		const auto& domain = entry.second;
		sorted.emplace_back(domain.pages.size(), &domain);
	}
	std::sort(sorted.begin(), sorted.end(), std::greater<std::pair<size_t, const domain_t*>>());
	
	std::vector<DomainIndex> result;
	for(uint32_t i = 0; i < uint32_t(limit) && offset + i < sorted.size(); ++i) {
		const auto* domain = sorted[offset + i].second;
		DomainIndex index;
		index.host = domain->host;
		index.num_pages = domain->pages.size();
		result.push_back(index);
	}
	return result;
}

std::vector<std::string> SearchEngine::reverse_lookup(const std::string& url_key) const
{
	std::shared_lock lock(index_mutex);
	
	std::vector<std::string> result;
	const auto page_id = find_url_id(url_key);
	if(page_id) {
		auto iter = page_index.find(page_id);
		if(iter != page_index.end()) {
			const auto& page = iter->second;
			for(const auto link_id : page.reverse_links) {
				auto iter2 = page_index.find(link_id);
				if(iter2 != page_index.end()) {
					result.push_back(iter2->second.url_key);
				}
			}
		}
	}
	return result;
}

std::vector<std::string> SearchEngine::reverse_domain_lookup(const std::string& url_key) const
{
	std::shared_lock lock(index_mutex);
	
	std::vector<std::string> result;
	const auto page_id = find_url_id(url_key);
	if(page_id) {
		auto iter = page_index.find(page_id);
		if(iter != page_index.end()) {
			const auto& page = iter->second;
			for(const auto domain_id : page.reverse_domains) {
				auto iter2 = domain_index.find(domain_id);
				if(iter2 != domain_index.end()) {
					result.push_back(iter2->second.host);
				}
			}
		}
	}
	return result;
}

std::vector<std::string> SearchEngine::suggest_words(const std::string& prefix, const int32_t& limit) const
{
	std::shared_lock lock(index_mutex);
	
	std::vector<std::string> result;
	for(auto it = word_set.lower_bound(prefix); it != word_set.end() && result.size() < size_t(limit); ++it) {
		result.push_back(*it);
	}
	return result;
}

std::vector<std::string> SearchEngine::suggest_domains(const std::string& prefix, const int32_t& limit) const
{
	std::shared_lock lock(index_mutex);
	
	std::vector<std::string> result;
	for(auto it = domain_map.lower_bound(prefix); it != domain_map.end() && result.size() < size_t(limit); ++it) {
		result.push_back(it->first);
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
		if(free_ids.empty()) {
			id = next_url_id++;
		} else {
			id = free_ids.back();
			free_ids.pop_back();
		}
		url_map[url_key] = id;
	}
	return id;
}

SearchEngine::domain_t& SearchEngine::get_domain(const std::string& host)
{
	auto iter = domain_map.find(host);
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

void SearchEngine::delete_page(const page_t& page)
{
	for(const auto parent_id : page.reverse_links)
	{
		const auto iter = page_index.find(parent_id);
		if(iter != page_index.end()) {
			auto& parent = iter->second;
			std::remove(parent.links.begin(), parent.links.end(), page.id);
		}
		open_links.emplace(page.id, parent_id);
	}
}

void SearchEngine::handle(std::shared_ptr<const keyvalue::KeyValuePair> pair)
{
	auto info = std::dynamic_pointer_cast<const PageInfo>(pair->value);
	if(info) {
		std::unique_lock lock(index_mutex);
		
		const uint32_t page_id = pair->key;
		url_map[info->url_key] = page_id;
		next_url_id = std::max(next_url_id, page_id + 1);
		
		auto& page = page_index[page_id];
		page.id = page_id;
		page.version = info->version;
		page.url_key = info->url_key;
		return;
	}
	
	auto url_index = std::dynamic_pointer_cast<const UrlIndex>(pair->value);
	if(url_index) {
		bool is_redirect = false;
		
		if(!url_index->redirect.empty())
		{
			const auto org_url_key = pair->key.to_string_value();
			const auto new_url_key = get_url_key(url_index->redirect);
			
			if(new_url_key != org_url_key)
			{
				std::unique_lock lock(index_mutex);
				
				const auto org_page_id = get_url_id(org_url_key);
				const auto new_page_id = get_url_id(new_url_key);
				const auto new_iter = page_index.find(new_page_id);
				const auto org_iter = page_index.find(org_page_id);
				
				if(org_iter != page_index.end())
				{
					auto& page = org_iter->second;
					if(new_iter != page_index.end()) {
						// both old and new pages exist
						auto& new_page = new_iter->second;
						for(const auto parent_id : page.reverse_links)
						{
							const auto parent_iter = page_index.find(parent_id);
							if(parent_iter != page_index.end()) {
								auto& parent = parent_iter->second;
								unique_push_back(parent.links, new_page_id);
								unique_push_back(new_page.reverse_links, parent.id);
								unique_push_back(new_page.reverse_domains, parent.domain_id);
							}
						}
					} else {
						// new page doesn't exist
						std::vector<std::pair<uint32_t, uint32_t>> new_links;
						const auto range = open_links.equal_range(new_page_id);
						
						for(const auto parent_id : page.reverse_links)
						{
							bool found = false;
							for(auto entry = range.first; entry != range.second; ++entry) {
								if(entry->second == parent_id) {
									found = true;
									break;
								}
							}
							if(!found) {
								new_links.emplace_back(new_page_id, parent_id);
							}
						}
						for(const auto& link : new_links) {
							open_links.emplace(link);
						}
					}
				}
				else if(new_iter != page_index.end())
				{
					// new page does exist
					auto& new_page = new_iter->second;
					const auto range = open_links.equal_range(org_page_id);
					
					for(auto entry = range.first; entry != range.second; ++entry)
					{
						const auto parent_iter = page_index.find(entry->second);
						if(parent_iter != page_index.end()) {
							auto& parent = parent_iter->second;
							unique_push_back(parent.links, new_page_id);
							unique_push_back(new_page.reverse_links, parent.id);
							unique_push_back(new_page.reverse_domains, parent.domain_id);
						}
					}
					open_links.erase(range.first, range.second);
				}
				else {
					// neither old or new page exists
					std::vector<std::pair<uint32_t, uint32_t>> new_links;
					const auto range = open_links.equal_range(org_page_id);
					
					for(auto entry = range.first; entry != range.second; ++entry) {
						new_links.emplace_back(new_page_id, entry->second);
					}
					open_links.erase(range.first, range.second);
					
					for(const auto& link : new_links) {
						open_links.emplace(link);			// emplace after erase
					}
				}
				redirects[org_page_id] = new_page_id;
				is_redirect = true;
			}
		}
		
		if(!is_redirect && url_index->fetch_count > 0)
		{
			std::unique_lock lock(index_mutex);
			
			const auto url_key = pair->key.to_string_value();
			const auto iter = url_map.find(url_key);
			if(iter != url_map.end()) {
				redirects.erase(iter->second);
			}
		}
		return;
	}
	
	if(pair->collection == "word_context")
	{
		std::unique_lock lock(index_mutex);
		
		const auto word = pair->key.to_string_value();
		word_set.insert(word);
		return;
	}
	
	if(pair->collection == "page_index")
	{
		std::unique_lock lock(index_mutex);
		
		const auto url_key = pair->key.to_string_value();
		const auto page_id = get_url_id(url_key);
		const auto iter = page_index.find(page_id);
		
		auto index = std::dynamic_pointer_cast<const PageIndex>(pair->value);
		if(index) {
			bool do_load = true;
			if(iter != page_index.end()) {
				auto& page = iter->second;
				page.is_deleted = false;
				do_load = pair->version >= page.version;	// make sure we don't overwrite an older version
			} else {
				auto info = PageInfo::create();
				info->url_key = url_key;
				page_info_async->store_value(page_id, info);	// make sure new page_id is stored
			}
			if(do_load) {
				url_index_async->get_value(url_key,
						std::bind(&SearchEngine::url_index_callback, this, url_key, pair->version, index, std::placeholders::_1));
			}
		} else {
			if(iter != page_index.end()) {
				delete_page(iter->second);
				page_index.erase(iter);
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
					free_ids.push_back(i);
				}
			}
			subscribe(input_url_index, 1000);
			url_index_async->sync_all(input_url_index_sync);
			log(INFO).out << "Found " << free_ids.size() << " free IDs";
			log(INFO).out << "Starting UrlIndex sync ...";
		}
		if(value->collection == "url_index")
		{
			subscribe(input_page_index, 1000);
			page_index_sync->sync_all(input_page_index_sync);
			log(INFO).out << "Starting PageIndex sync ...";
		}
		if(init_sync_count == 4)
		{
			const auto size_before = page_index.size();
			for(auto iter = page_index.begin(); iter != page_index.end();) {
				const auto& page = iter->second;
				if(page.is_deleted) {
					delete_page(page);
					iter = page_index.erase(iter);
				} else {
					iter++;
				}
			}
			log(INFO).out << "Purged " << (size_before - page_index.size()) << " deleted pages.";
			
			is_initialized = true;
			
			log(INFO).out << "Initialized with " << url_map.size() << " urls, " << domain_map.size() << " domains, "
					<< page_index.size() << " pages and " << word_set.size() << " words.";
		}
	}
}

void SearchEngine::url_index_callback(	const std::string& url_key,
										const uint64_t version,
										std::shared_ptr<const PageIndex> index,
										std::shared_ptr<const Value> url_index_)
{
	std::unique_lock lock(index_mutex);
	
	auto url_index = std::dynamic_pointer_cast<const UrlIndex>(url_index_);
	if(!url_index) {
		return;
	}
	if(url_index->depth < 0) {
		return;
	}
	
	const Url::Url parsed_url_key(url_key);
	const auto page_id = get_url_id(url_key);
	
	auto& page = page_index[page_id];
	page.id = page_id;
	page.scheme = url_index->scheme;
	page.url_key = url_key;
	page.title = index->title;
	page.first_seen = url_index->first_seen;
	page.last_modified = url_index->last_modified;
	page.links.clear();
	
	if(!page.domain_id) {
		auto& domain = get_domain(parsed_url_key.host());
		page.domain_id = domain.id;
		domain.pages.push_back(page.id);
	}
	
	std::unordered_set<uint32_t> links;
	
	for(const auto& link_url : index->links) {
		try {
			const Url::Url parsed_link(link_url);
			
			if(std::find(protocols.begin(), protocols.end(), parsed_link.scheme()) == protocols.end()) {
				continue;
			}
			auto link_id = get_url_id(get_url_key(parsed_link));
			{
				const auto iter = redirects.find(link_id);
				if(iter != redirects.end()) {
					link_id = iter->second;
				}
			}
			if(link_id != page.id) {
				links.insert(link_id);
			}
		} catch(...) {
			// ignore bad links
		}
	}
	
	for(const auto link_id : links)
	{
		auto iter = page_index.find(link_id);
		if(iter != page_index.end()) {
			auto& child = iter->second;
			if(page.is_loaded) {
				unique_push_back(child.reverse_links, page.id);
			} else {
				child.reverse_links.push_back(page.id);
			}
			unique_push_back(child.reverse_domains, page.domain_id);
			page.links.push_back(link_id);
		} else {
			bool found = false;
			if(page.is_loaded) {
				const auto range = open_links.equal_range(link_id);
				for(auto entry = range.first; entry != range.second; ++entry) {
					if(entry->second == page.id) {
						found = true;
						break;
					}
				}
			}
			if(!found) {
				open_links.emplace(link_id, page.id);
			}
		}
	}
	{
		const auto range = open_links.equal_range(page.id);
		for(auto entry = range.first; entry != range.second; ++entry)
		{
			auto iter = page_index.find(entry->second);
			if(iter != page_index.end()) {
				auto& parent = iter->second;
				parent.links.push_back(page.id);
				page.reverse_links.push_back(parent.id);
				unique_push_back(page.reverse_domains, parent.domain_id);
			}
		}
		open_links.erase(range.first, range.second);
	}
	
	// update word index if version is greater and previous update has finished
	if(version > page.version && !page_cache.count(page.id))
	{
		const auto now_wall_us = vnx::get_wall_time_micros();
		const float inv_word_count = 1.f / index->word_count;
		
		for(const auto& entry : index->words)
		{
			const auto& word = entry.first;
			auto& cached = word_cache[word];
			if(!cached) {
				cached = std::make_shared<word_t>();
				cached->queue_time_us = now_wall_us;
				{
					std::lock_guard lock(update_mutex);
					update_queue.emplace(cached->queue_time_us, word);
				}
				word_set.insert(word);
			}
			cached->add_pages.emplace_back(page_id, entry.second * inv_word_count);
		}
		page_cache[page.id].num_pending += index->words.size();
		page.version = version;
	}
	page.is_loaded = true;
	page.is_deleted = false;
}

void SearchEngine::print_stats()
{
	std::shared_lock lock(index_mutex);
	
	log(INFO).out << (60000 * word_update_counter) / stats_interval_ms << " words/min, "
			<< (60000 * page_update_counter) / stats_interval_ms << " pages/min, "
			<< (60000 * query_counter) / stats_interval_ms << " query/min, "
			<< page_index.size() << " pages, "
			<< word_cache.size() << " / " << word_set.size() << " words, "
			<< open_links.size() << " open links, "
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
					result->words.push_back(words[i]);
				}
				i++;
			}
			result->load_time_us = vnx::get_wall_time_micros() - time_begin;
		}
		catch(...) {
			request->callback(result);
			continue;
		}
		const auto time_begin = vnx::get_wall_time_micros();
		
		const uint32_t num_words = context.size();
		if(num_words == 0) {
			request->callback(result);
			continue;
		}
		
		struct result_t {
			const page_t* page = 0;
			uint64_t score = 1;
		};
		
		std::vector<result_t> pages;
		pages.reserve(max_query_pages);
		
		if(num_words > 1)
		{
			std::atomic<uint32_t> num_found(0);
			std::vector<std::pair<uint32_t, uint32_t>> found(max_query_pages);
			{
				const uint32_t N = 8;
#pragma omp parallel for
				for(uint32_t t = 0; t < N; ++t)
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
					
					while(num_iter > 0 && num_found < max_query_pages)
					{
						for(int i = 0; iter[k] != end[k] && i < 256; ++iter[k], ++i)
						{
							const auto page_id = iter[k]->first;
							if(page_id % N == t) {
								auto& entry = page_hits[page_id];
								if(entry.first == 0) {
									entry.second = iter[k]->second;
								} else {
									entry.second = std::min(uint32_t(iter[k]->second), entry.second);
								}
								if(++entry.first == num_words) {
									const auto index = num_found++;
									if(index < max_query_pages) {
										found[index] = std::make_pair(page_id, entry.second);
									}
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
			}
			{
				std::shared_lock lock(index_mutex);
				
				for(size_t i = 0; i < std::min(size_t(num_found), found.size()); ++i) {
					const auto& entry = found[i];
					auto iter = page_index.find(entry.first);
					if(iter != page_index.end() && iter->second.is_loaded)
					{
						const auto& page = iter->second;
						result_t result;
						result.page = &page;
						result.score = entry.second;
						pages.push_back(result);
					}
				}
			}
		}
		else {
			std::shared_lock lock(index_mutex);
			
			for(const auto& entry : context[0]->pages) {
				if(pages.size() >= max_query_pages) {
					break;
				}
				auto iter = page_index.find(entry.first);
				if(iter != page_index.end() && iter->second.is_loaded)
				{
					const auto& page = iter->second;
					result_t result;
					result.page = &page;
					result.score = entry.second;
					pages.push_back(result);
				}
			}
		}
		
		for(auto& result : pages) {
			result.score = result.score * result.page->reverse_domains.size();
		}
		
		std::multimap<int64_t, const page_t*, std::greater<int64_t>> sorted;
		
		if(has_flag(request->flags, search_flags_e::GROUP_BY_DOMAIN))
		{
			std::unordered_map<uint32_t, std::pair<int64_t, const page_t*>> best_of;
			
			for(const auto& result : pages) {
				auto& current = best_of[result.page->domain_id];
				if(!current.second || result.score > current.first) {
					current.first = result.score;
					current.second = result.page;
				}
			}
			for(const auto& entry : best_of) {
				limited_emplace(sorted, entry.second.first, entry.second.second, request->offset + request->limit);
			}
		}
		else {
			for(const auto& result : pages) {
				limited_emplace(sorted, result.score, result.page, request->offset + request->limit);
			}
		}
		
		result->num_results_total = pages.size();
		{
			std::shared_lock lock(index_mutex);
			
			uint32_t offset = 0;
			for(const auto& entry : sorted)
			{
				if(offset++ < request->offset) {
					continue;
				}
				if(result->items.size() >= size_t(request->limit)) {
					break;
				}
				const auto* page = entry.second;
				
				result_item_t item;
				item.title = page->title;
				item.url = page->scheme + ":" + page->url_key;
				item.score = entry.first;
				item.last_modified = page->last_modified;
				result->items.push_back(item);
			}
		}
		
		result->is_fail = false;
		result->compute_time_us = vnx::get_wall_time_micros() - time_begin;
		request->callback(result);
		
		query_counter++;
	}
}

void SearchEngine::update_loop() noexcept
{
	keyvalue::ServerClient page_info_sync("PageInfo");
	keyvalue::ServerClient word_context_sync("WordContext");
	
	bool do_wait = false;
	
	while(vnx_do_run())
	{
		std::string word;
		{
			std::unique_lock<std::mutex> lock(update_mutex);
			if(vnx_do_run() && do_wait) {
				update_condition.wait_for(lock, std::chrono::milliseconds(1000));
			}
			do_wait = true;
			
			if(vnx_do_run()) {
				if(!is_initialized || update_queue.empty()) {
					continue;
				}
				const auto& entry = update_queue.front();
				if((vnx::get_wall_time_micros() - entry.first) / 1000000 > commit_interval) {
					word = entry.second;
					update_queue.pop();
				} else {
					continue;
				}
			} else {
				break;
			}
		}
		do_wait = false;
		
		std::shared_ptr<word_t> cached;
		{
			std::unique_lock lock(index_mutex);
			
			auto iter = word_cache.find(word);
			if(iter != word_cache.end()) {
				cached = iter->second;
				word_cache.erase(iter);
			}
		}
		if(!cached) {
			continue;
		}
		
		std::shared_ptr<const WordContext> context;
		try {
			const auto value = word_context_sync.get_value(word);
			context = std::dynamic_pointer_cast<const WordContext>(value);
		}
		catch(...) {
			continue;
		}
		
		std::unordered_map<uint32_t, uint16_t> new_pages;
		new_pages.reserve(cached->add_pages.size());
		
		for(const auto& entry : cached->add_pages)
		{
			const float tmp = powf(entry.second, word_power);
			new_pages.emplace(entry.first, std::max(std::min(int(tmp * (0xFFFF * 10)), 0xFFFF), 1));
		}
		
		std::vector<std::pair<uint64_t, std::pair<uint32_t, uint16_t>>> list;
		list.reserve((context ? context->pages.size() : 0) + new_pages.size());
		{
			std::shared_lock lock(index_mutex);
			
			if(context) {
				for(const auto& entry : context->pages) {
					auto iter = page_index.find(entry.first);
					if(iter != page_index.end()) {
						auto weight = entry.second;
						auto iter2 = new_pages.find(entry.first);
						if(iter2 != new_pages.end()) {
							weight = iter2->second;
							new_pages.erase(iter2);
						}
						const auto& page = iter->second;
						list.emplace_back(uint64_t(weight) * page.reverse_domains.size(),
											std::make_pair(entry.first, weight));
					}
				}
			}
			for(const auto& entry : new_pages) {
				auto iter = page_index.find(entry.first);
				if(iter != page_index.end()) {
					const auto& page = iter->second;
					const auto weight = entry.second;
					list.emplace_back(uint64_t(weight) * page.reverse_domains.size(), entry);
				}
			}
		}
		std::sort(list.begin(), list.end(), std::greater<std::pair<uint64_t, std::pair<uint32_t, uint16_t>>>());
		
		try {
			auto value = WordContext::create();
			value->last_update = std::time(0);
			value->pages.reserve(list.size());
			for(const auto& entry : list) {
				value->pages.emplace_back(entry.second);
			}
			word_context_sync.store_value(word, value);
			
			std::vector<uint32_t> finished_pages;
			std::vector<std::pair<uint32_t, std::shared_ptr<PageInfo>>> page_updates;
			{
				std::shared_lock lock(index_mutex);
				
				for(const auto& entry : cached->add_pages)
				{
					const auto iter = page_cache.find(entry.first);
					if(iter != page_cache.end()) {
						if(iter->second.num_pending-- <= 1)
						{
							const auto iter2 = page_index.find(entry.first);
							if(iter2 != page_index.end()) {
								const auto& page = iter2->second;
								auto info = PageInfo::create();
								info->version = page.version;
								info->url_key = page.url_key;
								page_updates.emplace_back(entry.first, info);
							}
							finished_pages.push_back(entry.first);
						}
					}
				}
			}
			if(!finished_pages.empty())
			{
				std::unique_lock lock(index_mutex);
				
				for(const auto page_id : finished_pages) {
					page_cache.erase(page_id);
				}
			}
			for(const auto& entry : page_updates) {
				page_info_sync.store_value(entry.first, entry.second);
				page_update_counter++;
			}
			word_update_counter++;
		}
		catch(...) {
			// ignore
		}
	}
}


} // search
} // vnx
