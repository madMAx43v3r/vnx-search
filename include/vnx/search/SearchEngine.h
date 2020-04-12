/*
 * SearchEngine.h
 *
 *  Created on: Apr 10, 2020
 *      Author: mad
 */

#ifndef INCLUDE_VNX_SEARCH_SEARCHENGINE_H_
#define INCLUDE_VNX_SEARCH_SEARCHENGINE_H_

#include <vnx/search/SearchEngineBase.hxx>
#include <vnx/keyvalue/ServerClient.hxx>

#include <atomic>


namespace vnx {
namespace search {

class SearchEngine : public SearchEngineBase {
public:
	SearchEngine(const std::string& _vnx_name);
	
protected:
	struct word_t {
		uint32_t id = 0;
		std::vector<uint32_t> pages;
	};
	
	struct page_t {
		uint32_t id = 0;
		uint32_t domain_id = 0;
		int64_t last_modified = 0;
		int64_t last_fetched = 0;
		int64_t first_fetched = 0;
		std::string title;
		std::vector<uint32_t> links;
		std::vector<uint32_t> words;
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
	uint32_t get_url_id(const std::string& url);
	
	uint32_t get_domain_id(const std::string& url);
	
	uint32_t get_word_id(const std::string& word);
	
	void work_loop();
	
	void update_func(std::vector<std::shared_ptr<const keyvalue::KeyValuePair>> values);
	
private:
	std::shared_ptr<keyvalue::ServerClient> page_index_client;
	std::shared_ptr<keyvalue::ServerClient> page_content_client;
	
	std::vector<std::thread> work_threads;
	
	mutable std::mutex work_mutex;
	mutable std::condition_variable work_condition;
	mutable std::queue<std::shared_ptr<query_t>> work_queue;
	
	mutable std::mutex index_mutex;
	std::unordered_map<std::string, uint32_t> url_map;
	std::unordered_map<uint32_t, std::string> url_reverse_map;
	std::map<std::string, uint32_t> domain_map;
	std::unordered_map<uint32_t, std::string> domain_reverse_map;
	std::map<std::string, uint32_t> word_map;
	std::unordered_map<uint32_t, std::string> word_reverse_map;
	std::unordered_map<uint32_t, std::shared_ptr<word_t>> word_index;
	std::unordered_map<uint32_t, std::shared_ptr<page_t>> page_index;
	
	std::vector<std::shared_ptr<const keyvalue::KeyValuePair>> update_buffer;
	
	bool is_initialized = false;
	uint32_t next_url_id = 1;
	uint32_t next_domain_id = 1;
	uint32_t next_word_id = 1;
	
	mutable std::atomic<int64_t> query_counter;
	
	// below work_loop() private
	std::mutex parallel_mutex;
	
};


} // search
} // vnx

#endif /* INCLUDE_VNX_SEARCH_SEARCHENGINE_H_ */
