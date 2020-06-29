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
#include <vnx/search/SearchEngine_query.hxx>

#include <vnx/keyvalue/Server.h>
#include <vnx/keyvalue/ServerClient.hxx>
#include <vnx/keyvalue/ServerAsyncClient.hxx>

#include <stx/cstring.h>
#include <stx/sstring.h>
#include <stx/fstring.h>
#include <stx/pstring.h>

#include <atomic>
#include <shared_mutex>


namespace vnx {
namespace search {

class SearchEngine : public SearchEngineBase {
public:
	TopicPtr input_url_index_sync;
	TopicPtr input_page_info_sync;
	TopicPtr input_page_index_sync;
	TopicPtr input_word_context_sync;
	
	SearchEngine(const std::string& _vnx_name);
	
protected:
	struct word_t {
		uint32_t id = 0;
		std::atomic<uint32_t> num_pages {0};
		stx::sstring value;
	};
	
	struct domain_t {
		uint32_t id = 0;
		stx::sstring host;
		std::vector<uint32_t> pages;
	};
	
	struct page_t {
		uint32_t id = 0;
		uint32_t domain_id = 0;
		uint64_t version = 0;
		uint64_t link_version = 0;
		uint64_t word_version = 0;
		int64_t first_seen = 0;
		int64_t last_modified = 0;
		stx::fstring<8> scheme;
		std::vector<uint32_t> reverse_domains;
	};
	
	struct page_cache_t {
		int32_t words_pending = 0;
		uint64_t word_version = 0;
		std::string url_key;
		std::vector<uint32_t> words;
	};
	
	struct word_cache_t {
		uint32_t word_id = 0;
		std::string word;
		std::vector<uint32_t> rem_pages;
		std::vector<std::pair<uint32_t, float>> add_pages;
	};
	
	struct link_cache_t {
		bool is_update = false;		// if version should be updated
		uint64_t version = 0;
		int64_t schedule_time_us = 0;
		std::vector<uint32_t> rem_links;
		std::vector<uint32_t> add_links;
		std::vector<uint32_t> rem_reverse_links;
		std::vector<uint32_t> add_reverse_links;
	};
	
	struct query_job_t : SearchEngine_query {
		request_id_t req_id;
	};
	
	struct page_update_job_t {
		
	};
	
	struct word_update_job_t {
		std::shared_ptr<word_cache_t> cached;
		std::shared_ptr<const WordContext> context;
		std::shared_ptr<WordContext> result;
	};
	
	void init() override;
	
	void main() override;
	
	void query_async(	const std::vector<std::string>& words,
						const int32_t& limit, const uint32_t& offset,
						const std::vector<search_flags_e>& flags,
						const request_id_t& req_id) const override;
	
	void get_page_info_callback(const std::string& url_key,
								std::shared_ptr<const Value> value,
								const request_id_t& req_id) const;
	
	void reverse_lookup_callback(	const std::string& url_key,
									std::shared_ptr<const Value> value,
									const request_id_t& req_id) const;
	
	void reverse_domain_lookup_callback(const std::string& url_key,
										std::shared_ptr<const Value> value,
										const request_id_t& req_id) const;
	
	Object get_domain_info(const std::string& host, const int32_t& limit, const uint32_t& offset) const;
	
	void get_page_info_async(const std::string& url_key, const request_id_t& req_id) const;
	
	std::vector<Object> get_domain_list(const int32_t& limit, const uint32_t& offset) const;
	
	void reverse_lookup_async(const std::string& url_key, const request_id_t& req_id) const;
	
	void reverse_domain_lookup_async(const std::string& url_key, const request_id_t& req_id) const;
	
	std::vector<std::string> suggest_words(const std::string& prefix, const int32_t& limit) const;
	
	std::vector<std::string> suggest_domains(const std::string& prefix, const int32_t& limit) const;
	
	void handle(std::shared_ptr<const keyvalue::KeyValuePair> pair) override;
	
	void handle(std::shared_ptr<const keyvalue::SyncInfo> value) override;
	
private:
	page_t* find_page(uint32_t page_id);
	const page_t* find_page(uint32_t page_id) const;
	
	template<typename T>
	word_t& get_word(const T& word);
	const word_t* find_word(uint32_t word_id) const;
	
	template<typename T>
	domain_t& get_domain(const T& host);
	const domain_t* find_domain(uint32_t domain_id) const;
	
	std::shared_ptr<link_cache_t> get_link_cache(const std::string& url_key);
	std::shared_ptr<link_cache_t> get_link_cache(uint32_t page_id);
	
	std::shared_ptr<word_cache_t> get_word_cache(uint32_t word_id);
	
	void delete_page_async(const std::string& url_key);
	
	void delete_page_callback(	const std::string& url_key,
								std::pair<Variant, std::shared_ptr<const Value>> pair);
	
	void redirect_callback(	const std::string& org_url_key,
							const std::string& new_url_key,
							std::shared_ptr<const Value> value);
	
	void update_page_callback_0(const std::string& url_key,
								const uint64_t version,
								std::shared_ptr<const Value> value);
	
	void update_page_callback_1(const std::string& url_key,
								const uint64_t version,
								std::shared_ptr<const UrlIndex> url_index,
								std::shared_ptr<const Value> value);
	
	void update_page_callback_2(const std::string& url_key,
								const uint64_t version,
								std::shared_ptr<const PageIndex> index,
								std::shared_ptr<const UrlIndex> url_index,
								std::pair<Variant, std::shared_ptr<const Value>> pair);
	
	void update_page(	const std::string& url_key,
						const uint64_t version,
						std::shared_ptr<const PageIndex> index,
						std::shared_ptr<const PageInfo> page_info,
						std::shared_ptr<const UrlIndex> url_index);
	
	void check_load_queue();
	
	void check_link_queue();
	
	void check_word_queue();
	
	void link_update(	const std::string& url_key,
						std::pair<Variant, std::shared_ptr<const Value>> pair);
	
	void word_update_callback(std::shared_ptr<word_update_job_t> job, std::shared_ptr<const Value> value);
	
	void word_update_finished(std::shared_ptr<word_update_job_t> job);
	
	void page_word_update_finished(uint32_t page_id);
	
	void page_word_update_callback(	std::shared_ptr<page_cache_t> cached,
									std::pair<Variant, std::shared_ptr<const Value>> pair);
	
	void print_stats();
	
	void query_loop() const noexcept;
	
	void word_update_task(std::shared_ptr<word_update_job_t> job) const noexcept;
	
private:
	Handle<keyvalue::Server> module_page_info;
	Handle<keyvalue::Server> module_word_context;
	
	std::shared_ptr<ThreadPool> query_threads;
	std::shared_ptr<ThreadPool> word_update_threads;
	
	std::shared_ptr<keyvalue::ServerAsyncClient> url_index_async;
	std::shared_ptr<keyvalue::ServerAsyncClient> page_info_async;
	std::shared_ptr<keyvalue::ServerAsyncClient> page_index_async;
	std::shared_ptr<keyvalue::ServerAsyncClient> word_context_async;
	
	// protected by index_mutex (only main thread may modify)
	std::vector<uint32_t> free_page_ids;
	std::map<stx::sstring, uint32_t, std::less<>> word_map;
	std::map<stx::sstring, uint32_t, std::less<>> domain_map;
	std::unordered_map<uint32_t, word_t> word_index;
	std::unordered_map<uint32_t, domain_t> domain_index;
	std::unordered_map<uint32_t, page_t> page_index;
	
	// accessed by main thread only
	std::unordered_map<uint32_t, std::shared_ptr<page_cache_t>> page_cache;
	std::unordered_map<uint32_t, std::shared_ptr<word_cache_t>> word_cache;
	std::unordered_map<std::string, std::shared_ptr<link_cache_t>> link_cache;
	std::queue<std::pair<uint64_t, std::string>> load_queue;
	std::multimap<int64_t, std::string> link_queue;
	std::multimap<int64_t, uint32_t> word_queue;
	
	mutable std::shared_mutex index_mutex;
	
	int init_sync_count = 0;
	uint32_t next_page_id = 1;
	uint32_t next_word_id = 1;
	uint32_t next_domain_id = 1;
	std::atomic_bool is_initialized {false};
	
	mutable std::atomic<int64_t> query_counter {0};
	mutable std::atomic<int64_t> page_update_counter {0};
	mutable std::atomic<int64_t> word_update_counter {0};
	
};


} // search
} // vnx

#endif /* INCLUDE_VNX_SEARCH_SEARCHENGINE_H_ */
