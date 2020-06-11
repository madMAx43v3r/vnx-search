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
		bool is_loaded = false;
		bool is_deleted = true;
		uint64_t version = 0;
		int64_t first_seen = 0;
		int64_t last_modified = 0;
		stx::fstring<8> scheme;
		stx::cstring url_key;
		stx::cstring title;
		std::vector<uint32_t> links;
		std::vector<uint32_t> reverse_links;
		std::vector<uint32_t> reverse_domains;
		std::string get_url() const { return is_loaded ? scheme.str() + ":" + url_key.str() : url_key.str(); }
	};
	
	struct page_cache_t {
		uint64_t version = 0;
		ssize_t num_pending = 0;
		stx::cstring url_key;
		std::vector<uint32_t> words;
	};
	
	struct word_cache_t {
		uint32_t word_id;
		std::vector<uint32_t> rem_pages;
		std::vector<std::pair<uint32_t, float>> add_pages;
	};
	
	struct query_t : SearchEngine_query {
		request_id_t req_id;
	};
	
	void init() override;
	
	void main() override;
	
	void query_async(	const std::vector<std::string>& words,
						const int32_t& limit, const uint32_t& offset,
						const std::vector<search_flags_e>& flags,
						const request_id_t& req_id) const override;
	
	Object get_domain_info(const std::string& host, const int32_t& limit, const uint32_t& offset) const;
	
	Object get_page_info(const std::string& url_key) const;
	
	std::vector<Object> get_domain_list(const int32_t& limit, const uint32_t& offset) const;
	
	std::vector<std::string> reverse_lookup(const std::string& url_key) const;
	
	std::vector<std::string> reverse_domain_lookup(const std::string& url_key) const;
	
	std::vector<std::string> suggest_words(const std::string& prefix, const int32_t& limit) const;
	
	std::vector<std::string> suggest_domains(const std::string& prefix, const int32_t& limit) const;
	
	void handle(std::shared_ptr<const keyvalue::KeyValuePair> pair) override;
	
	void handle(std::shared_ptr<const keyvalue::SyncInfo> value) override;
	
private:
	uint32_t find_url_id(const stx::cstring& url_key) const;
	uint32_t get_url_id(const stx::cstring& url_key);
	
	page_t* find_page(uint32_t url_id);
	const page_t* find_page(uint32_t url_id) const;
	
	word_t& get_word(const stx::sstring& word);
	const word_t* find_word(uint32_t word_id) const;
	
	domain_t& get_domain(const stx::sstring& host);
	const domain_t* find_domain(uint32_t domain_id) const;
	
	void delete_page(const page_t& page);
	
	void url_index_callback(	const std::string& url_key,
								const uint64_t version,
								std::shared_ptr<const PageIndex> index,
								std::shared_ptr<const Value> value);
	
	void update_page(	const std::string& url_key,
						const uint64_t version,
						std::shared_ptr<const PageIndex> index,
						std::shared_ptr<const UrlIndex> url_index);
	
	void update_words(	const uint32_t page_id,
						std::shared_ptr<const PageIndex> index,
						std::shared_ptr<const Value> value);
	
	void print_stats();
	
	void query_loop() const noexcept;
	
	void update_loop() noexcept;
	
private:
	Handle<keyvalue::Server> module_page_info;
	Handle<keyvalue::Server> module_word_context;
	
	std::vector<std::thread> query_threads;
	std::vector<std::thread> update_threads;
	
	std::shared_ptr<keyvalue::ServerAsyncClient> url_index_async;
	std::shared_ptr<keyvalue::ServerAsyncClient> page_info_async;
	std::shared_ptr<keyvalue::ServerAsyncClient> word_context_async;
	std::shared_ptr<keyvalue::ServerClient> page_index_sync;
	
	mutable std::shared_mutex index_mutex;
	
	// protected by index_mutex
	std::vector<uint32_t> free_url_ids;
	std::map<stx::sstring, uint32_t> word_map;
	std::map<stx::sstring, uint32_t> domain_map;
	std::unordered_map<stx::cstring, uint32_t> url_map;
	std::unordered_map<uint32_t, word_t> word_index;
	std::unordered_map<uint32_t, domain_t> domain_index;
	std::unordered_map<uint32_t, page_t> page_index;
	std::unordered_map<uint32_t, uint32_t> redirects;
	std::unordered_multimap<uint32_t, uint32_t> open_links;
	
	// protected by update_mutex
	std::unordered_map<uint32_t, page_cache_t> page_cache;
	std::unordered_map<uint32_t, std::shared_ptr<word_cache_t>> word_cache;
	
	mutable std::mutex query_mutex;
	mutable std::condition_variable query_condition;
	mutable std::queue<std::shared_ptr<query_t>> query_queue;
	
	mutable std::mutex update_mutex;
	mutable std::condition_variable update_condition;
	mutable std::queue<std::pair<int64_t, std::shared_ptr<word_cache_t>>> update_queue;
	
	int init_sync_count = 0;
	uint32_t next_url_id = 1;
	uint32_t next_word_id = 1;
	uint32_t next_domain_id = 1;
	volatile bool is_initialized = false;
	
	mutable std::atomic<int64_t> query_counter {0};
	mutable std::atomic<int64_t> page_update_counter {0};
	mutable std::atomic<int64_t> word_update_counter {0};
	
};


} // search
} // vnx

#endif /* INCLUDE_VNX_SEARCH_SEARCHENGINE_H_ */
