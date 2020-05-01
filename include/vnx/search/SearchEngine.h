/*
 * SearchEngine.h
 *
 *  Created on: Apr 10, 2020
 *      Author: mad
 */

#ifndef INCLUDE_VNX_SEARCH_SEARCHENGINE_H_
#define INCLUDE_VNX_SEARCH_SEARCHENGINE_H_

#include <vnx/search/SearchEngineBase.hxx>
#include <vnx/search/WordContext.hxx>
#include <vnx/search/PageInfo.hxx>

#include <vnx/keyvalue/Server.h>
#include <vnx/keyvalue/ServerClient.hxx>
#include <vnx/keyvalue/ServerAsyncClient.hxx>

#include <atomic>
#include <shared_mutex>


namespace vnx {
namespace search {

class SearchEngine : public SearchEngineBase {
public:
	TopicPtr input_page_index_sync;
	
	SearchEngine(const std::string& _vnx_name);
	
protected:
	struct word_t {
		int64_t queue_time_us = 0;
		std::vector<std::pair<uint32_t, float>> add_pages;
	};
	
	struct domain_t {
		uint32_t id = 0;
		uint32_t num_pages = 0;
		std::string host;
	};
	
	struct page_t {
		uint32_t id = 0;
		uint32_t domain_id = 0;
		uint32_t index_version = 0;
		int depth = -1;
		bool is_loaded = false;
		uint64_t version = 0;
		std::string scheme;
		std::string url_key;
		std::string title;
		int64_t first_seen = 0;
		int64_t last_modified = 0;
		std::vector<uint32_t> links;
		std::vector<uint32_t> reverse_links;
		std::vector<uint32_t> reverse_domains;
		std::atomic<size_t> num_pending {0};
	};
	
	struct query_t {
		std::vector<std::string> words;
		int32_t limit = 0;
		int64_t offset = 0;
		std::vector<search_flags_e> flags;
		std::function<void(const std::shared_ptr<const SearchResult>&)> callback;
	};
	
	void init() override;
	
	void main() override;
	
	void query_async(	const std::vector<std::string>& words,
						const int32_t& limit, const int64_t& offset,
						const std::vector<search_flags_e>& flags,
						const std::function<void(const std::shared_ptr<const SearchResult>&)>& _callback,
						const vnx::request_id_t& _request_id) const override;
	
	std::vector<std::string> suggest_words(const std::string& prefix, const int32_t& limit) const;
	
	std::vector<std::string> suggest_domains(const std::string& prefix, const int32_t& limit) const;
	
	void handle(std::shared_ptr<const keyvalue::KeyValuePair> pair) override;
	
	void handle(std::shared_ptr<const keyvalue::SyncInfo> value) override;
	
private:
	uint32_t get_url_id(const std::string& url_key);
	
	SearchEngine::domain_t& get_domain(const std::string& host);
	
	void url_index_callback(	const std::string& url_key,
								const uint64_t version,
								std::shared_ptr<const PageIndex> page_index_,
								std::shared_ptr<const Value> url_index_);
	
	void update_queue_timer();
	
	void print_stats();
	
	void query_loop() const noexcept;
	
	void update_loop() noexcept;
	
private:
	TopicPtr input_page_info_sync;
	
	Handle<keyvalue::Server> module_page_info;
	Handle<keyvalue::Server> module_word_context;
	
	std::vector<std::thread> query_threads;
	std::vector<std::thread> update_threads;
	
	std::shared_ptr<keyvalue::ServerAsyncClient> url_index_async;
	std::shared_ptr<keyvalue::ServerAsyncClient> page_info_async;
	std::shared_ptr<keyvalue::ServerAsyncClient> word_context_async;
	std::shared_ptr<keyvalue::ServerClient> page_index_sync;
	
	mutable std::mutex parallel_mutex;
	mutable std::shared_mutex index_mutex;
	
	std::unordered_map<std::string, uint32_t> url_map;
	std::map<std::string, uint32_t> domain_map;
	std::unordered_map<uint32_t, domain_t> domain_index;
	std::unordered_map<uint32_t, page_t> page_index;
	std::unordered_multimap<uint32_t, uint32_t> open_links;
	
	std::set<std::string> word_set;
	std::unordered_map<std::string, std::shared_ptr<word_t>> word_cache;
	
	mutable std::mutex query_mutex;
	mutable std::condition_variable query_condition;
	mutable std::queue<std::shared_ptr<query_t>> query_queue;
	
	mutable std::mutex update_mutex;
	mutable std::condition_variable update_condition;
	mutable std::multimap<int64_t, std::string> update_queue;
	
	bool is_initialized = false;
	int init_sync_count = 0;
	uint32_t next_url_id = 1;
	uint32_t next_domain_id = 1;
	
	mutable std::atomic<int64_t> query_counter {0};
	mutable std::atomic<int64_t> page_update_counter {0};
	mutable std::atomic<int64_t> word_update_counter {0};
	
};


} // search
} // vnx

#endif /* INCLUDE_VNX_SEARCH_SEARCHENGINE_H_ */
