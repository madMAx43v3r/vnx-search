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
#include <algorithm>


namespace vnx {
namespace search {

SearchEngine::SearchEngine(const std::string& _vnx_name)
	:	SearchEngineBase(_vnx_name)
{
	input_page_info_sync = vnx_name + ".page_info.sync";
	input_page_index_sync = vnx_name + ".page_index.sync";
	
	protocols.push_back("http");
	protocols.push_back("https");
}

void SearchEngine::init()
{
	vnx::open_pipe(vnx_name, this, 1000);
}

void SearchEngine::main()
{
	subscribe(input_page_index, 1000);
	subscribe(input_page_info_sync, 1000);
	subscribe(input_page_index_sync, 1000);
	
	protocols = get_unique(protocols);
	
	module_page_info = new keyvalue::Server("PageInfo");
	module_page_info->collection = "page_info";
	module_page_info->num_read_threads = 1;
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
	set_timer_millis(update_interval_ms, std::bind(&SearchEngine::update_queue_timer, this));
	
	page_info_async->sync_all(input_page_info_sync);
	
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
								const int32_t& limit, const int64_t& offset,
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

uint32_t SearchEngine::get_url_id(const std::string& url_key)
{
	uint32_t id = 0;
	{
		auto iter = url_map.find(url_key);
		if(iter != url_map.end()) {
			id = iter->second;
		} else {
			id = next_url_id++;
			url_map[url_key] = id;
		}
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

void SearchEngine::handle(std::shared_ptr<const keyvalue::KeyValuePair> pair)
{
	auto info = std::dynamic_pointer_cast<const PageInfo>(pair->value);
	if(info) {
		if(info->depth >= 0 && info->depth <= max_depth)
		{
			std::unique_lock lock(index_mutex);
			
			const uint32_t page_id = pair->key;
			url_map[info->url_key] = page_id;
			next_url_id = std::max(next_url_id, page_id + 1);
			
			auto& page = page_index[page_id];
			page.id = page_id;
			page.version = info->version;
			page.url_key = info->url_key;
			page.depth = info->depth;
		}
		return;
	}
	
	auto index = std::dynamic_pointer_cast<const PageIndex>(pair->value);
	if(index) {
		const auto url_key = pair->key.to_string_value();
		url_index_async->get_value(url_key,
					std::bind(&SearchEngine::url_index_callback, this, url_key, pair->version, index, std::placeholders::_1));
	}
}

void SearchEngine::handle(std::shared_ptr<const keyvalue::SyncInfo> value)
{
	std::unique_lock lock(index_mutex);
	
	if(value->code == keyvalue::SyncInfo::END)
	{
		init_sync_count++;
		if(init_sync_count == 1)
		{
			page_index_sync->sync_all(input_page_index_sync);
			log(INFO).out << "Starting PageIndex sync ...";
		}
		if(init_sync_count == 2)
		{
			is_initialized = true;
			uint64_t num_links = 0;
			for(const auto& entry : page_index) {
				num_links += entry.second.links.size();
			}
			log(INFO).out << "Initialized with " << url_map.size() << " urls, " << domain_map.size() << " domains, "
					<< page_index.size() << " pages, " << num_links << " links and " << word_set.size() << " words.";
		}
	}
}

void SearchEngine::url_index_callback(	const std::string& url_key,
										const uint64_t version,
										std::shared_ptr<const PageIndex> page_index_,
										std::shared_ptr<const Value> url_index_)
{
	std::unique_lock lock(index_mutex);
	
	const auto now_wall_us = vnx::get_wall_time_micros();
	
	auto url_index = std::dynamic_pointer_cast<const UrlIndex>(url_index_);
	if(url_index) {
		if(url_index->depth < 0 || url_index->depth > max_depth) {
			return;
		}
		const auto page_id = get_url_id(url_key);
		
		auto& page = page_index[page_id];
		page.id = page_id;
		page.scheme = url_index->scheme;
		page.url_key = url_key;
		page.title = page_index_->title;
		page.depth = url_index->depth;
		page.first_seen = url_index->first_seen;
		page.last_modified = url_index->last_modified;
		
		if(!page.domain_id) {
			auto& domain = get_domain(Url::Url(url_key).host());
			page.domain_id = domain.id;
			domain.num_pages++;
		}
		
		page.links.clear();
		
		if(page.depth < max_depth)
		{
			for(const auto& link : page_index_->links) {
				try {
					const Url::Url parsed(link);
					if(std::find(protocols.begin(), protocols.end(), parsed.scheme()) != protocols.end())
					{
						const auto link_id = get_url_id(get_url_key(parsed));
						if(link_id == page.id) {
							continue;		// ignore self links
						}
						page.links.push_back(link_id);
						
						auto iter = page_index.find(link_id);
						if(iter != page_index.end()) {
							auto& child = iter->second;
							if(page.version) {
								unique_push_back(child.reverse_links, page.id);
							} else {
								child.reverse_links.push_back(page.id);
							}
							unique_push_back(child.reverse_domains, page.domain_id);
						} else {
							bool found = false;
							if(page.version) {
								auto range = reverse_links.equal_range(link_id);
								for(auto entry = range.first; entry != range.second; ++entry) {
									if(entry->second == page.id) {
										found = true;
										break;
									}
								}
							}
							if(!found) {
								reverse_links.emplace(link_id, page.id);
							}
						}
					}
				} catch(...) {
					// ignore bad links
				}
			}
		}
		{
			auto range = reverse_links.equal_range(page.id);
			for(auto entry = range.first; entry != range.second; ++entry)
			{
				auto iter = page_index.find(entry->second);
				if(iter != page_index.end()) {
					const auto& parent = iter->second;
					page.reverse_links.push_back(parent.id);
					unique_push_back(page.reverse_domains, parent.domain_id);
				}
			}
			reverse_links.erase(page.id);
		}
		
		if(version > page.version)
		{
			for(const auto& word : page_index_->words)
			{
				auto& entry = word_cache[word];
				if(!entry) {
					entry = std::make_shared<word_t>();
					entry->queue_time_us = now_wall_us;
				}
				entry->add_pages.push_back(page_id);
			}
			page.num_pending += page_index_->words.size();
			page.version = version;
		}
	}
}

void SearchEngine::update_queue_timer()
{
	std::multimap<int64_t, std::string> new_queue;
	{
		std::shared_lock lock(index_mutex);
		for(const auto& entry : word_cache) {
			limited_emplace(new_queue, entry.second->queue_time_us, entry.first, 10000);
		}
	}
	{
		std::lock_guard lock(update_mutex);
		update_queue = std::move(new_queue);
	}
	update_condition.notify_all();
}

void SearchEngine::print_stats()
{
	std::shared_lock lock(index_mutex);
	
	log(INFO).out << (60000 * word_update_counter) / stats_interval_ms << " words/min, "
			<< (60000 * page_update_counter) / stats_interval_ms << " pages/min, "
			<< (60000 * query_counter) / stats_interval_ms << " query/min, "
			<< page_index.size() << " pages, "
			<< word_cache.size() << " words cached, "
			<< reverse_links.size() << " open links";
	
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
			const std::vector<Variant> keys(request->words.begin(), request->words.end());
			const auto values = word_context_sync.get_values(keys);
			int i = 0;
			for(auto value : values) {
				auto word_context = std::dynamic_pointer_cast<const WordContext>(value);
				if(word_context) {
					context.push_back(word_context);
					result->words.push_back(request->words[i]);
				}
				i++;
			}
		}
		catch(...) {
			request->callback(result);
		}
		const uint32_t num_words = context.size();
		
		struct result_t {
			const page_t* page = 0;
			uint32_t weight = 1;
			std::atomic<uint32_t> score {0};
		};
		
		std::unordered_map<uint32_t, result_t> pages;
		
		if(num_words > 1)
		{
			std::unordered_map<uint32_t, uint32_t> page_hits;
			{
				std::vector<std::vector<uint32_t>::const_iterator> iter(num_words);
				std::vector<std::vector<uint32_t>::const_iterator> end(num_words);
				for(uint32_t i = 0; i < num_words; ++i) {
					iter[i] = context[i]->pages.begin();
					end[i] = context[i]->pages.end();
				}
				uint32_t k = 0;
				uint32_t num_iter = num_words;
				uint32_t num_hits = 0;
				
				while(num_iter > 0 && (max_query_pages < 0 || num_hits < max_query_pages))
				{
					if(iter[k] != end[k]) {
						if(++page_hits[*iter[k]] == num_words) {
							num_hits++;
						}
						iter[k]++;
					} else {
						iter.erase(iter.begin() + k);
						end.erase(end.begin() + k);
						num_iter--;
					}
					if(++k >= num_iter) {
						k = 0;
					}
				}
				pages.reserve(num_hits);
			}
			{
				std::shared_lock lock(index_mutex);
				
				for(const auto& entry : page_hits) {
					if(entry.second >= num_words) {
						auto iter = page_index.find(entry.first);
						if(iter != page_index.end()) {
							auto& result = pages[entry.first];
							result.page = &iter->second;
							result.weight = iter->second.reverse_domains.size();
						}
					}
				}
			}
		}
		else {
			std::shared_lock lock(index_mutex);
			
			pages.reserve(std::min(context[0]->pages.size(), size_t(max_query_pages)));
			
			for(const auto page_id : context[0]->pages) {
				if(pages.size() >= size_t(max_query_pages)) {
					break;
				}
				auto iter = page_index.find(page_id);
				if(iter != page_index.end()) {
					auto& result = pages[page_id];
					result.page = &iter->second;
					result.weight = iter->second.reverse_domains.size();
				}
			}
		}
		
		{
			std::lock_guard lock(parallel_mutex);
			
			const int N = omp_get_max_threads();
#pragma omp parallel for
			for(int k = 0; k < N; ++k)
			{
				for(auto entry = advance_until(pages.begin(), pages.end(), k); entry != pages.end();
						 entry = advance_until(entry, pages.end(), N))
				{
					const auto& result = entry->second;
					for(const auto link_id : result.page->links)
					{
						const auto iter = pages.find(link_id);
						if(iter != pages.end()) {
							iter->second.score += result.score;
						}
					}
				}
			}
		}
		
		std::multimap<int64_t, const page_t*, std::greater<int64_t>> sorted;
		
		if(has_flag(request->flags, search_flags_e::GROUP_BY_DOMAIN))
		{
			std::unordered_map<uint32_t, std::pair<int64_t, const page_t*>> best_of;
			
			for(const auto& entry : pages) {
				const auto& result = entry.second;
				auto& current = best_of[result.page->domain_id];
				if(result.score > current.first) {
					current.first = result.score;
					current.second = result.page;
				}
			}
			for(const auto& entry : best_of) {
				limited_emplace(sorted, entry.second.first, entry.second.second, request->offset + request->limit);
			}
		}
		else {
			for(const auto& entry : pages) {
				limited_emplace(sorted, entry.second.score, entry.second.page, request->offset + request->limit);
			}
		}
		
		result->num_results_total = pages.size();
		{
			std::shared_lock lock(index_mutex);
			
			int64_t offset = 0;
			for(const auto& entry : sorted)
			{
				if(offset++ < request->offset) {
					continue;
				}
				if(result->items.size() >= size_t(request->limit)) {
					break;
				}
				result_item_t item;
				item.title = entry.second->title;
				item.url = entry.second->scheme + ":" + entry.second->url_key;
				item.score = entry.first;
				item.last_modified = entry.second->last_modified;
				result->items.push_back(item);
			}
		}
		
		result->is_fail = false;
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
				update_condition.wait(lock);
			}
			do_wait = true;
			
			if(vnx_do_run()) {
				if(update_queue.empty()) {
					continue;
				}
				const auto iter = update_queue.begin();
				if((vnx::get_wall_time_micros() - iter->first) / 1000000 > commit_interval) {
					word = iter->second;
					update_queue.erase(iter);
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
		
		std::unordered_set<uint32_t> new_pages(cached->add_pages.begin(), cached->add_pages.end());
		
		if(context) {
			for(const auto page_id : context->pages) {
				const auto iter = new_pages.find(page_id);
				if(iter != new_pages.end()) {
					new_pages.erase(iter);
				}
			}
		}
		
		std::vector<std::pair<uint32_t, uint32_t>> list;
		list.reserve((context ? context->pages.size() : 0) + new_pages.size());
		{
			std::shared_lock lock(index_mutex);
			
			if(context) {
				for(const auto page_id : context->pages) {
					auto iter = page_index.find(page_id);
					if(iter != page_index.end()) {
						list.emplace_back(iter->second.reverse_links.size(), page_id);
					}
				}
			}
			for(const auto page_id : new_pages) {
				auto iter = page_index.find(page_id);
				if(iter != page_index.end()) {
					list.emplace_back(iter->second.reverse_links.size(), page_id);
				}
			}
		}
		std::sort(list.begin(), list.end(), std::greater<std::pair<uint32_t, uint32_t>>());
		
		try {
			auto value = WordContext::create();
			value->last_update = std::time(0);
			value->pages.reserve(list.size());
			for(const auto& entry : list) {
				value->pages.push_back(entry.second);
			}
			word_context_sync.store_value(word, value);
			
			std::vector<std::pair<uint32_t, std::shared_ptr<PageInfo>>> page_updates;
			{
				std::shared_lock lock(index_mutex);
				
				for(const auto page_id : cached->add_pages) {
					auto iter = page_index.find(page_id);
					if(iter != page_index.end()) {
						auto& page = iter->second;
						if(--(page.num_pending) == 0)
						{
							auto info = PageInfo::create();
							info->version = page.version;
							info->url_key = page.url_key;
							info->depth = page.depth;
							page_updates.emplace_back(page_id, info);
						}
					}
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
