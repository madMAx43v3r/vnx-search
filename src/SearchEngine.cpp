/*
 * SearchEngine.cpp
 *
 *  Created on: Apr 10, 2020
 *      Author: mad
 */

#include <vnx/search/SearchEngine.h>
#include <vnx/search/PageIndex.hxx>


namespace vnx {
namespace search {

SearchEngine::SearchEngine(const std::string& _vnx_name)
	:	SearchEngineBase(_vnx_name)
{
	input_page_index_sync = vnx_name + ".page_index.sync";
}

void SearchEngine::init()
{
	vnx::open_pipe(vnx_name, this, 1000);
}

void SearchEngine::main()
{
	subscribe(input_page_index_sync, 100);
	
	page_index_client = std::make_shared<keyvalue::ServerClient>(page_index_server);
	page_index_client->sync_all(input_page_index_sync);
	
	work_threads.resize(num_threads);
	for(int i = 0; i < num_threads; ++i) {
		work_threads[i] = std::thread(&SearchEngine::work_loop, this);
	}
	
	Super::main();
	
	work_condition.notify_all();
	for(auto& thread : work_threads) {
		if(thread.joinable()) {
			thread.join();
		}
	}
}

void SearchEngine::query_async(	const std::vector<std::string>& words,
								const int64_t& limit, const int64_t& offset,
								const std::function<void(const std::shared_ptr<const SearchResult>&)>& _callback,
								const vnx::request_id_t& _request_id) const
{
	auto request = std::make_shared<query_t>();
	request->words = words;
	request->limit = std::min(limit, int64_t(100));
	request->offset = offset;
	request->callback = _callback;
	{
		std::lock_guard<std::mutex> lock(work_mutex);
		work_queue.push(request);
	}
	work_condition.notify_one();
}

uint32_t SearchEngine::get_url_id(const std::string& url)
{
	uint32_t url_id = 0;
	{
		auto iter = url_map.find(url);
		if(iter != url_map.end()) {
			url_id = iter->second;
		} else {
			url_id = next_url_id++;
			url_map[url] = url_id;
			url_reverse_map[url_id] = url;
		}
	}
	return url_id;
}

uint32_t SearchEngine::get_word_id(const std::string& word)
{
	uint32_t word_id = 0;
	{
		auto iter = word_map.find(word);
		if(iter != word_map.end()) {
			word_id = iter->second;
		} else {
			word_id = next_word_id++;
			word_map[word] = word_id;
			word_reverse_map[word_id] = word;
		}
	}
	return word_id;
}

void SearchEngine::handle(std::shared_ptr<const keyvalue::KeyValuePair> pair)
{
	auto index = std::dynamic_pointer_cast<const PageIndex>(pair->value);
	if(index) {
		if(is_initialized) {
			update_buffer.push_back(pair);
		}
		else {
			std::lock_guard<std::mutex> lock(index_mutex);
			
			const auto url = pair->key.to_string_value();
			const auto page_id = get_url_id(url);
			
			auto& page = page_index[page_id];
			if(!page) {
				page = std::make_shared<page_t>();
			}
			page->id = page_id;
			page->title = index->title;
			page->last_modified = index->last_modified;
			
			for(const auto& link : index->links) {
				page->links.push_back(get_url_id(link));
			}
			
			for(const auto& word : index->words)
			{
				const auto word_id = get_word_id(word);
				auto& entry = word_index[word_id];
				if(!entry) {
					entry = std::make_shared<word_t>();
				}
				entry->pages.push_back(page_id);
				page->words.push_back(word_id);
			}
		}
	}
}

void SearchEngine::handle(std::shared_ptr<const keyvalue::SyncInfo> value)
{
	if(value->code == keyvalue::SyncInfo::END) {
		if(!is_initialized) {
			std::lock_guard<std::mutex> lock(index_mutex);
			is_initialized = true;
			subscribe(input_page_index, 1000);
			log(INFO).out << "Initialized with " << url_map.size() << " urls, "
					<< page_index.size() << " pages and " << word_index.size() << " words.";
		}
	}
}

void SearchEngine::work_loop()
{
	while(vnx_do_run())
	{
		std::shared_ptr<query_t> request;
		{
			std::unique_lock<std::mutex> lock(work_mutex);
			while(vnx_do_run() && work_queue.empty()) {
				work_condition.wait(lock);
			}
			if(vnx_do_run()) {
				request = work_queue.front();
				work_queue.pop();
			} else {
				break;
			}
		}
		
		auto result = SearchResult::create();
		
		uint32_t num_words = 0;
		std::unordered_map<uint32_t, uint32_t> page_hits;
		
		for(const auto& word : request->words) {
			std::shared_ptr<word_t> entry;
			{
				std::lock_guard<std::mutex> lock(index_mutex);
				auto id = word_map.find(word);
				if(id != word_map.end()) {
					auto iter = word_index.find(id->second);
					if(iter != word_index.end()) {
						entry = iter->second;
					}
				}
			}
			if(entry) {
				for(auto page : entry->pages) {
					page_hits[page]++;
				}
				result->words.push_back(word);
				num_words++;
			}
		}
		
		struct result_t {
			std::shared_ptr<const page_t> page;
			int64_t score = 1;
			int64_t add_score = 0;
		};
		
		std::unordered_map<uint32_t, result_t> pages;
		{
			std::lock_guard<std::mutex> lock(index_mutex);
			for(const auto& entry : page_hits) {
				if(entry.second >= num_words) {
					auto iter = page_index.find(entry.first);
					if(iter != page_index.end()) {
						pages[entry.first].page = iter->second;
					}
				}
			}
		}
		page_hits.clear();
		
		for(int iteration = 0; iteration < num_iterations; ++iteration)
		{
			for(const auto& entry : pages)
			{
				const auto& result = entry.second;
				for(auto link : result.page->links)
				{
					auto iter = pages.find(link);
					if(iter != pages.end()) {
						iter->second.add_score += result.score;
					}
				}
			}
			for(auto& entry : pages)
			{
				auto& result = entry.second;
				result.score += result.add_score;
				result.add_score = 0;
			}
		}
		
		std::multimap<int64_t, std::shared_ptr<const page_t>, std::greater<int64_t>> sorted;
		for(const auto& entry : pages) {
			sorted.emplace(entry.second.score, entry.second.page);
		}
		
		result->num_pages_total = sorted.size();
		{
			std::lock_guard<std::mutex> lock(index_mutex);
			int64_t offset = 0;
			for(const auto& entry : sorted)
			{
				if(offset++ < request->offset) {
					continue;
				}
				ResultItem item;
				item.title = entry.second->title;
				item.url = url_reverse_map[entry.second->id];
				item.score = entry.first;
				item.last_modified = entry.second->last_modified;
				result->items.push_back(item);
				
				if(result->items.size() >= request->limit) {
					break;
				}
			}
		}
		request->callback(result);
		
		query_counter++;
	}
}

void SearchEngine::update_func(std::vector<std::shared_ptr<const keyvalue::KeyValuePair>> values)
{
	// TODO
}


} // search
} // vnx
