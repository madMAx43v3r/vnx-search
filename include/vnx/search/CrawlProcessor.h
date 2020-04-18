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
#include <vnx/search/PageContent.hxx>
#include <vnx/search/CrawlFrontendAsyncClient.hxx>

#include <vnx/keyvalue/ServerClient.hxx>
#include <vnx/keyvalue/ServerAsyncClient.hxx>

namespace Url {
	class Url;
}

namespace googlebot {
	class RobotsMatcher;
}


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
	
	enum robots_txt_state_e {
		ROBOTS_TXT_UNKNOWN,
		ROBOTS_TXT_PENDING,
		ROBOTS_TXT_MISSING,
		ROBOTS_TXT_TIMEOUT,
		ROBOTS_TXT_FOUND
	};
	
	struct domain_t {
		std::string host;
		std::multimap<int, std::string> queue;
		std::shared_ptr<const PageContent> robots_txt;
		int64_t num_fetched = 0;
		int64_t num_errors = 0;
		int64_t num_disallowed = 0;
		int64_t last_fetch_us = 0;			// [usec]
		int64_t robot_start_time = 0;		// [sec]
		int num_pending = 0;
		robots_txt_state_e robots_state = ROBOTS_TXT_UNKNOWN;
		bool is_blacklisted = false;
	};
	
	void main() override;
	
	std::shared_ptr<const CrawlStats> get_stats(const int32_t& limit) const override;
	
	void handle(std::shared_ptr<const TextResponse> value) override;
	
	void handle(std::shared_ptr<const keyvalue::KeyValuePair> value) override;
	
private:
	domain_t& get_domain(const std::string& host);
	
	bool filter_url(const Url::Url& parsed);
	
	bool enqueue(const std::string& url, int depth, int64_t load_time = 0);
	
	void check_queue();
	
	void update_queue();
	
	void check_url(const std::string& url, int depth, std::shared_ptr<const Value> index);
	
	void check_all_urls();
	
	void check_page_callback(const std::string& url_key, std::shared_ptr<const Value> url_index, std::shared_ptr<const PageIndex> page_index);
	
	void check_page(const std::string& url_key, int depth, std::shared_ptr<const PageIndex> index);
	
	url_t url_fetch_done(const std::string& url_key);
	
	void url_fetch_callback(const std::string& url, std::shared_ptr<const UrlIndex> index);
	
	void url_update_callback(const std::string& url_key, std::shared_ptr<UrlIndex> fetched, std::shared_ptr<const Value> previous);
	
	void robots_txt_callback(const std::string& url_key, robots_txt_state_e missing_state, std::shared_ptr<const Value> value);
	
	void url_fetch_error(uint64_t request_id, const std::exception& ex);
	
	void url_index_error(uint64_t request_id, const std::exception& ex);
	
	void page_index_error(uint64_t request_id, const std::exception& ex);
	
	void page_content_error(uint64_t request_id, const std::exception& ex);
	
	void publish_stats();
	
	void print_stats();
	
private:
	TopicPtr url_sync_topic;
	std::map<uint64_t, std::string> pending_urls;
	std::multimap<int64_t, std::string> waiting;
	std::multimap<std::pair<int, int64_t>, domain_t*> queue;
	
	std::unordered_map<std::string, url_t> url_map;
	std::unordered_map<std::string, domain_t> domain_map;
	
	std::shared_ptr<keyvalue::ServerAsyncClient> url_index_async;
	std::shared_ptr<keyvalue::ServerAsyncClient> page_index_async;
	std::shared_ptr<keyvalue::ServerAsyncClient> page_content_async;
	std::shared_ptr<CrawlFrontendAsyncClient> crawl_frontend_async;
	
	std::shared_ptr<googlebot::RobotsMatcher> matcher;
	
	int64_t fetch_counter = 0;
	int64_t error_counter = 0;
	int64_t reload_counter = 0;
	int64_t pending_robots_txt = 0;
	int64_t missing_robots_txt = 0;
	int64_t timed_out_robots_txt = 0;
	int64_t found_robots_txt = 0;
	double average_depth = 0;
	
};


} // search
} // vnx

#endif /* INCLUDE_VNX_SEARCH_CRAWLPROCESSOR_H_ */
