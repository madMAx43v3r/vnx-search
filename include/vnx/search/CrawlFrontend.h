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

namespace httplib {
	class Client;
	class SSLClient;
} // httplib


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
	
private:
	struct request_t {
		std::string url;
		std::string path;
		std::shared_ptr<httplib::Client> client;
		std::vector<std::string> accept_content;
		std::function<void(const std::shared_ptr<const UrlIndex>&)> callback;
	};
	
	void parse_callback(std::shared_ptr<const TextResponse> value);
	
	void print_stats();
	
	void fetch_loop();
	
private:
	Hash64 unique_service;
	std::vector<std::thread> work_threads;
	
	mutable std::mutex mutex;
	mutable std::condition_variable condition;
	
	mutable std::queue<std::shared_ptr<request_t>> work_queue;
	
	mutable std::map<std::string, std::vector<std::shared_ptr<httplib::Client>>> http_clients;
	mutable std::map<std::string, std::vector<std::shared_ptr<httplib::SSLClient>>> https_clients;
	
	std::map<std::string, std::map<Hash64, std::shared_ptr<ContentParserAsyncClient>>> parser_map;
	
	mutable std::atomic<uint64_t> fetch_counter;
	mutable std::atomic<uint64_t> general_fail_counter;
	mutable std::atomic<uint64_t> invalid_protocol_counter;
	mutable std::atomic<uint64_t> invalid_content_type_counter;
	mutable std::atomic<uint64_t> invalid_response_size_counter;
	
	mutable uint64_t num_bytes_fetched = 0;
	mutable uint64_t num_bytes_parsed = 0;
	
	uint64_t last_fetch_count = 0;
	uint64_t last_error_count = 0;
	
};


} // search
} // vnx

#endif /* INCLUDE_VNX_SEARCH_CRAWLFRONTEND_H_ */
