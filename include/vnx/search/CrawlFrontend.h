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
#include <vnx/ThreadPool.h>

#include <url.h>

#include <atomic>


namespace vnx {
namespace search {

class CrawlFrontend : public CrawlFrontendBase {
public:
	struct request_t {
		std::string url;
		Url::Url parsed_url = Url::Url("");
		std::set<std::string> accept_content;
		vnx::request_id_t req_id;
		uint64_t parse_id = 0;
		bool is_fetch = false;
		UrlInfo info;
		std::shared_ptr<FetchResult> result;
	};
	
	CrawlFrontend(const std::string& _vnx_name);
	
protected:
	struct parser_t {
		Hash64 address;
		std::set<std::string> content_types;
		std::shared_ptr<ContentParserAsyncClient> client;
	};
	
	void init() override;
	
	void main() override;
	
	void load_async(const std::string& url,
					const vnx::request_id_t& req_id) const override;
	
	void fetch_async(	const std::string& url,
						const vnx::request_id_t& req_id) const override;
	
	void register_parser(	const vnx::Hash64& address,
							const std::vector<std::string>& mime_types,
							const int32_t& num_threads) override;
	
private:
	void fetch_callback(std::shared_ptr<const HttpResponse> response,
						std::shared_ptr<request_t> request);
	
	void parse_callback(std::shared_ptr<const TextResponse> response,
						std::shared_ptr<request_t> request);
	
	void parse_error(Hash64 address, std::shared_ptr<request_t> request, const vnx::exception& ex);
	
	void print_stats();
	
	void fetch_loop() noexcept;
	
	static size_t header_callback(char* buffer, size_t size, size_t len, void* userdata);
	
	static size_t write_callback(char* buf, size_t size, size_t len, void* userdata);
	
private:
	std::shared_ptr<ThreadPool> work_threads;
	
	mutable std::mutex work_mutex;
	mutable std::condition_variable work_condition;
	mutable std::queue<std::shared_ptr<request_t>> work_queue;
	
	std::map<Hash64, parser_t> parser_map;
	
	mutable std::atomic<uint64_t> fetch_counter {0};
	mutable std::atomic<uint64_t> redirect_counter {0};
	mutable std::atomic<uint64_t> server_fail_counter {0};
	mutable std::atomic<uint64_t> general_fail_counter {0};
	mutable std::atomic<uint64_t> connection_fail_counter {0};
	mutable std::atomic<uint64_t> empty_response_counter {0};
	mutable std::atomic<uint64_t> invalid_url_counter {0};
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
