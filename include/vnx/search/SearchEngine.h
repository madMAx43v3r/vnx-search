/*
 * SearchEngine.h
 *
 *  Created on: Apr 10, 2020
 *      Author: mad
 */

#ifndef INCLUDE_VNX_SEARCH_SEARCHENGINE_H_
#define INCLUDE_VNX_SEARCH_SEARCHENGINE_H_

#include <vnx/search/SearchEngineBase.hxx>
#include <vnx/search/SearchEngineAsyncClient.hxx>
#include <vnx/search/WordContext.hxx>
#include <vnx/search/WordArray.hxx>
#include <vnx/search/PageInfo.hxx>
#include <vnx/search/PageIndex.hxx>
#include <vnx/search/PageContent.hxx>

#include <vnx/keyvalue/Server.h>
#include <vnx/keyvalue/ServerClient.hxx>
#include <vnx/keyvalue/StorageClient.hxx>
#include <vnx/keyvalue/ServerAsyncClient.hxx>

#include <stx/sstring.h>
#include <stx/fstring.h>
#include <stx/pstring.h>

#include <atomic>
#include <shared_mutex>


namespace vnx {
namespace search {

class SearchEngine : public SearchEngineBase {
public:
	TopicPtr input_page_info;
	TopicPtr input_url_index_sync;
	TopicPtr input_page_index_sync;
	TopicPtr input_page_content_sync;
	TopicPtr input_page_info_sync;
	TopicPtr input_word_context_sync;
	
	SearchEngine(const std::string& _vnx_name);
	
protected:
	struct word_t {
		uint32_t id = 0;
		uint32_t num_pages = 0;
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
		uint32_t reverse_links = 0;
		uint32_t reverse_domains = 0;
		float rank_value = 0;
		uint64_t index_version = 0;
		uint64_t link_version = 0;
		uint64_t word_version = 0;
		uint64_t array_version = 0;
		int64_t first_seen = 0;
		int64_t last_modified = 0;
		stx::fstring<8> scheme = std::string("http");
		stx::pstring url_key;
		std::string get_url() const { return scheme.str() + ":" + url_key.str(); }
	};
	
	struct page_cache_t {
		int32_t words_pending = 0;
		uint64_t word_version = 0;
		std::string url_key;
		std::set<uint32_t> words;
	};
	
	struct word_cache_t {
		uint32_t word_id = 0;
		std::string word;
		std::vector<uint32_t> rem_pages;
		std::vector<uint32_t> add_pages;
	};
	
	struct link_cache_t {
		std::string url_key;
		bool is_page_update = false;		// if version should be updated
		uint64_t link_version = 0;
		int64_t schedule_time_us = 0;
		std::vector<std::string> rem_links;
		std::vector<std::string> add_links;
		std::vector<std::string> rem_reverse_links;
		std::vector<std::string> add_reverse_links;
	};
	
	struct page_update_job_t {
		bool update_info = false;
		bool update_links = false;
		bool update_words = false;
		uint64_t index_version = 0;
		std::string url_key;
		std::string scheme;
		std::shared_ptr<const PageInfo> info;
		std::shared_ptr<const PageIndex> index;
		std::shared_ptr<const UrlIndex> url_index;
		std::vector<std::string> links;
		std::map<std::string, std::string> redirects;
		std::map<uint32_t, int> words;
		std::vector<std::string> new_words;
	};
	
	struct link_update_job_t {
		std::shared_ptr<link_cache_t> cached;
		std::shared_ptr<const PageInfo> info;
		std::shared_ptr<PageInfo> result;
	};
	
	struct word_update_job_t {
		size_t num_pages = 0;
		std::shared_ptr<word_cache_t> cached;
		std::shared_ptr<const WordContext> context;
		std::shared_ptr<WordContext> result;
	};
	
	struct word_process_job_t {
		std::string url_key;
		uint64_t content_version = 0;
		size_t num_new_words = 0;
		std::vector<std::string> word_list;
		std::vector<std::pair<uint32_t, uint32_t>> word_positions;
		std::shared_ptr<const PageContent> content;
		std::shared_ptr<WordArray> word_array;
	};
	
	void init() override;
	
	void main() override;
	
	void get_page_info_callback(const std::string& url_key,
								std::shared_ptr<const keyvalue::Entry> entry,
								const request_id_t& req_id) const;
	
	void reverse_lookup_callback(	const std::string& url_key,
									std::shared_ptr<const keyvalue::Entry> entry,
									const request_id_t& req_id) const;
	
	void reverse_domain_lookup_callback(const std::string& url_key,
										std::shared_ptr<const keyvalue::Entry> entry,
										const request_id_t& req_id) const;
	
	void get_domain_info_async(	const std::string& host,
								const int32_t& limit,
								const uint32_t& offset,
								const request_id_t& req_id) const;
	
	void get_page_info_async(const std::string& url_key, const request_id_t& req_id) const;
	
	void get_page_entries_async(const std::vector<uint32_t>& page_ids, const request_id_t& req_id) const;
	
	void get_page_ranks_async(	const std::vector<std::string>& url_keys,
								const vnx::bool_t& direct,
								const request_id_t& req_id) const;
	
	std::vector<Object> get_domain_list(const int32_t& limit, const uint32_t& offset) const;
	
	void get_page_ranking_async(const int32_t& limit, const uint32_t& offset,
								const request_id_t& req_id) const;
	
	void reverse_lookup_async(const std::string& url_key, const request_id_t& req_id) const;
	
	void reverse_domain_lookup_async(const std::string& url_key, const request_id_t& req_id) const;
	
	std::vector<std::string> suggest_words(const std::string& prefix, const int32_t& limit) const;
	
	std::vector<std::string> suggest_domains(const std::string& prefix, const int32_t& limit) const;
	
	void handle(std::shared_ptr<const keyvalue::SyncUpdate> pair) override;
	
	void handle(std::shared_ptr<const keyvalue::SyncInfo> value) override;
	
private:
	page_t* find_page(uint32_t page_id);
	const page_t* find_page(uint32_t page_id) const;
	
	template<typename T>
	page_t* find_page_url(const T& url_key);
	template<typename T>
	const page_t* find_page_url(const T& url_key) const;
	
	template<typename T>
	uint32_t find_word_id(const T& word) const;
	const word_t* find_word(uint32_t word_id) const;
	uint32_t add_word(const std::string& word);
	
	template<typename T>
	domain_t& get_domain(const T& host);
	const domain_t* find_domain(uint32_t domain_id) const;
	
	std::shared_ptr<link_cache_t> get_link_cache(const std::string& url_key);
	
	std::shared_ptr<word_cache_t> get_word_cache(uint32_t word_id);
	
	void delete_page_async(const std::string& url_key);
	
	void delete_page_callback(	const std::string& url_key,
								std::shared_ptr<const keyvalue::Entry> entry);
	
	void redirect_callback(	const std::string& org_url_key,
							const std::string& new_url_key,
							std::shared_ptr<const keyvalue::Entry> entry);
	
	void update_page_callback_0(std::shared_ptr<page_update_job_t> job,
								std::shared_ptr<const keyvalue::Entry> entry);
	
	void update_page_callback_1(std::shared_ptr<page_update_job_t> job,
								std::shared_ptr<const keyvalue::Entry> entry);
	
	void update_page_callback_2(std::shared_ptr<page_update_job_t> job,
								std::vector<std::shared_ptr<const keyvalue::Entry>> entries);
	
	void update_page_callback_3(std::shared_ptr<page_update_job_t> job);
	
	void update_page_callback_4(std::shared_ptr<page_update_job_t> job,
								std::shared_ptr<const keyvalue::Entry> entry);
	
	void update_page(std::shared_ptr<page_update_job_t> job);
	
	void word_process_callback_0(	std::shared_ptr<word_process_job_t> job,
									std::shared_ptr<const keyvalue::Entry> entry);
	
	void word_process_callback_1(	std::shared_ptr<word_process_job_t> job);
	
	void word_process_callback_2(	std::shared_ptr<word_process_job_t> job,
									std::shared_ptr<const keyvalue::Entry> entry);
	
	void check_queues();
	void check_load_queue();
	void check_link_queue();
	void check_word_queue();
	
	void link_update_callback_0(std::shared_ptr<link_cache_t> cached,
								std::shared_ptr<const keyvalue::Entry> entry);
	
	void link_update_callback_1(std::shared_ptr<link_update_job_t> job);
	
	void link_update_callback_2(std::shared_ptr<link_update_job_t> job,
								std::vector<float> rank_values);
	
	void word_update_callback(	std::shared_ptr<word_update_job_t> job,
								std::shared_ptr<const keyvalue::Entry> entry);
	
	void word_update_finished(std::shared_ptr<word_update_job_t> job);
	
	void page_word_update_finished(uint32_t page_id);
	
	void page_word_update_callback(	std::shared_ptr<page_cache_t> cached,
									std::shared_ptr<const keyvalue::Entry> entry);
	
	void print_stats();
	
	void write_info();
	
	void page_rank_task(const std::vector<std::string>& url_keys,
						const bool direct,
						const request_id_t& req_id) const noexcept;
	
	void link_update_task(std::shared_ptr<link_update_job_t> job) noexcept;
	
	void word_collect_task(std::shared_ptr<page_update_job_t> job) noexcept;
	
	void word_process_task(std::shared_ptr<word_process_job_t> job) noexcept;
	
	void word_update_task(std::shared_ptr<word_update_job_t> job) noexcept;
	
private:
	Hash64 private_addr;
	Handle<keyvalue::Server> module_page_info;
	Handle<keyvalue::Server> module_word_context;
	Handle<keyvalue::Server> module_word_array;
	
	std::shared_ptr<ThreadPool> update_threads;
	
	std::shared_ptr<SearchEngineAsyncClient> search_async;
	std::shared_ptr<keyvalue::ServerAsyncClient> url_index_async;
	std::shared_ptr<keyvalue::ServerAsyncClient> page_info_async;
	std::shared_ptr<keyvalue::ServerAsyncClient> page_index_async;
	std::shared_ptr<keyvalue::ServerAsyncClient> page_content_async;
	std::shared_ptr<keyvalue::ServerAsyncClient> word_context_async;
	std::shared_ptr<keyvalue::ServerAsyncClient> word_array_async;
	
	// protected by index_mutex (only main thread may modify)
	std::map<stx::sstring, uint32_t, std::less<>> word_map;
	std::map<stx::sstring, uint32_t, std::less<>> domain_map;
	std::map<stx::pstring, uint32_t, std::less<>> page_map;
	std::unordered_map<uint32_t, domain_t> domain_index;
	std::unordered_map<uint32_t, page_t> page_index;
	std::unordered_map<uint32_t, word_t> word_index;
	
	// accessed by main thread only
	std::unordered_map<uint32_t, std::shared_ptr<page_cache_t>> page_cache;
	std::unordered_map<uint32_t, std::shared_ptr<word_cache_t>> word_cache;
	std::unordered_map<std::string, std::shared_ptr<link_cache_t>> link_cache;
	std::queue<std::shared_ptr<page_update_job_t>> load_queue;
	std::queue<std::shared_ptr<word_process_job_t>> load_queue_2;
	std::multimap<int64_t, std::shared_ptr<link_cache_t>> link_queue;
	std::multimap<int64_t, uint32_t> word_queue;
	
	std::multimap<uint32_t, uint32_t, std::greater<float>> page_ranking;
	
	mutable std::shared_mutex index_mutex;
	
	int init_sync_count = 0;
	uint32_t next_page_id = 1;
	uint32_t next_word_id = 1;
	uint32_t next_domain_id = 1;
	std::atomic_bool is_initialized {false};
	
	mutable std::atomic<int64_t> page_update_counter {0};
	mutable std::atomic<int64_t> word_update_counter {0};
	
};


} // search
} // vnx

#endif /* INCLUDE_VNX_SEARCH_SEARCHENGINE_H_ */
