/*
 * CrawlProcessor.h
 *
 *  Created on: Apr 4, 2020
 *      Author: mad
 */

#ifndef INCLUDE_VNX_SEARCH_CRAWLPROCESSOR_H_
#define INCLUDE_VNX_SEARCH_CRAWLPROCESSOR_H_

#include <vnx/search/CrawlProcessorBase.hxx>
#include <vnx/search/UrlIndex.hxx>
#include <vnx/search/PageIndex.hxx>
#include <vnx/search/CrawlFrontendAsyncClient.hxx>

#include <vnx/keyvalue/ServerClient.hxx>
#include <vnx/keyvalue/ServerAsyncClient.hxx>


namespace vnx {
namespace search {

class CrawlProcessor : public CrawlProcessorBase {
public:
	CrawlProcessor(const std::string& _vnx_name);
	
protected:
	struct url_t {
		std::string domain;
		uint64_t request_id = -1;
		int depth = 0;
		bool is_reload = false;
	};
	
	struct domain_t {
		std::multimap<int, std::string> queue;
		int64_t num_fetched = 0;
		int64_t num_errors = 0;
		int64_t last_fetch_us = 0;			// [usec]
		int num_pending = 0;
	};
	
	void main() override;
	
	void handle(std::shared_ptr<const keyvalue::KeyValuePair> value) override;
	
private:
	bool enqueue(const std::string& url, int depth, int64_t load_time = 0);
	
	void check_queue();
	
	void check_url(const std::string& url, int depth, std::shared_ptr<const Value> index);
	
	void check_all_urls();
	
	void check_page_callback(const std::string& url, std::shared_ptr<const Value> url_index, std::shared_ptr<const PageIndex> page_index);
	
	void check_page(const std::string& url, int depth, std::shared_ptr<const PageIndex> index);
	
	url_t url_fetch_done(const std::string& url);
	
	void url_fetch_callback(const std::string& url, std::shared_ptr<const UrlIndex> index);
	
	void url_update_callback(const std::string& url, std::shared_ptr<UrlIndex> fetched, std::shared_ptr<const Value> previous);
	
	void url_fetch_error(uint64_t request_id, const std::exception& ex);
	
	void url_index_error(uint64_t request_id, const std::exception& ex);
	
	void publish_stats();
	
	void print_stats();
	
private:
	TopicPtr url_sync_topic;
	std::multimap<int64_t, std::string> waiting;
	
	std::unordered_map<std::string, url_t> url_map;
	std::unordered_map<std::string, domain_t> domain_map;
	
	std::map<uint64_t, std::string> pending_urls;
	
	std::shared_ptr<keyvalue::ServerClient> url_index;
	std::shared_ptr<keyvalue::ServerAsyncClient> url_index_async;
	std::shared_ptr<CrawlFrontendAsyncClient> crawl_frontend_async;
	
	uint64_t fetch_counter = 0;
	uint64_t error_counter = 0;
	uint64_t reload_counter = 0;
	uint64_t active_domains = 0;
	double average_depth = 0;
	
};


} // search
} // vnx

#endif /* INCLUDE_VNX_SEARCH_CRAWLPROCESSOR_H_ */
