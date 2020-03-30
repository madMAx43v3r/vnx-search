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
						const std::function<void()>& _callback,
						const vnx::request_id_t& _request_id) override;
	
	void register_parser(	const vnx::Hash64& address,
							const std::string& mime_type,
							const int32_t& num_threads) override;
	
	void handle(std::shared_ptr<const HttpResponse> value) override;
	
private:
	struct request_t {
		std::string url;
		std::string protocol;
		std::string host;
		std::string path;
		int port = -1;
		std::vector<std::string> accept_content;
		std::function<void()> callback;
	};
	
	void fetch_loop();
	
private:
	Hash64 unique_service;
	
	mutable std::mutex mutex;
	mutable std::condition_variable condition;
	
	mutable std::queue<std::shared_ptr<request_t>> work_queue;
	std::vector<std::thread> work_threads;
	
	std::map<std::string, std::map<Hash64, std::shared_ptr<ContentParserAsyncClient>>> parser_map;
	
	std::atomic<uint64_t> fetch_counter;
	std::atomic<uint64_t> general_fail_counter;
	std::atomic<uint64_t> invalid_protocol_counter;
	std::atomic<uint64_t> invalid_content_type_counter;
	std::atomic<uint64_t> invalid_reponse_size_counter;
	
	uint64_t num_bytes_fetched = 0;
	uint64_t num_bytes_parsed = 0;
	
};


} // search
} // vnx

#endif /* INCLUDE_VNX_SEARCH_CRAWLFRONTEND_H_ */
