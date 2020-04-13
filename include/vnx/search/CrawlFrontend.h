/*
 * CrawlFrontend.h
 *
 *  Created on: Mar 29, 2020
 *      Author: mad
 */

#ifndef INCLUDE_VNX_SEARCH_CRAWLFRONTEND_H_
#define INCLUDE_VNX_SEARCH_CRAWLFRONTEND_H_

#include <vnx/search/CrawlFrontendBase.hxx>
#include <vnx/search/ContentParserAsyncClient.hxx>

#include <atomic>


namespace vnx {
namespace search {

class CrawlFrontend : public CrawlFrontendBase {
public:
	CrawlFrontend(const std::string& _vnx_name);
	
protected:
	void init() override;
	
	void main() override;
	
	void fetch_async(	const std::string& url,
						const std::function<void(const std::shared_ptr<const UrlIndex>&)>& _callback,
						const vnx::request_id_t& _request_id) const override;
	
	void register_parser(	const vnx::Hash64& address,
							const std::vector<std::string>& mime_types,
							const int32_t& num_threads) override;
	
	void handle(std::shared_ptr<const HttpResponse> value) override;
	
public:
	struct request_t {
		std::string url;
		std::vector<std::string> accept_content;
		vnx::request_id_t request_id;
		std::function<void(const std::shared_ptr<const UrlIndex>&)> callback;
	};
	
private:
	struct parser_t {
		Hash64 address;
		std::set<std::string> content_types;
		std::shared_ptr<ContentParserAsyncClient> client;
	};
	
	void parse_callback(std::shared_ptr<const TextResponse> value);
	
	void parse_error(Hash64 address, uint64_t request_id, const std::exception& ex);
	
	void print_stats();
	
	void fetch_loop();
	
	static size_t header_callback(char* buffer, size_t size, size_t len, void* userdata);
	
	static size_t write_callback(char* buf, size_t size, size_t len, void* userdata);
	
private:
	Hash64 unique_service;
	std::vector<std::thread> work_threads;
	
	mutable std::mutex mutex;
	mutable std::condition_variable work_condition;
	
	mutable std::queue<std::shared_ptr<request_t>> work_queue;
	
	std::map<Hash64, parser_t> parser_map;
	
	mutable std::atomic<uint64_t> fetch_counter {0};
	mutable std::atomic<uint64_t> general_fail_counter {0};
	mutable std::atomic<uint64_t> invalid_protocol_counter {0};
	mutable std::atomic<uint64_t> invalid_content_type_counter {0};
	mutable std::atomic<uint64_t> invalid_response_size_counter {0};
	mutable std::atomic<uint64_t> parse_failed_counter {0};
	
	mutable uint64_t num_bytes_fetched = 0;
	mutable uint64_t num_bytes_parsed = 0;
	
	uint64_t last_fetch_count = 0;
	uint64_t last_num_bytes_fetched = 0;
	uint64_t last_num_bytes_parsed = 0;
	
};


} // search
} // vnx

#endif /* INCLUDE_VNX_SEARCH_CRAWLFRONTEND_H_ */
