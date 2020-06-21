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
#include <vnx/search/CrawlProcessorClient.hxx>
#include <vnx/search/CrawlFrontendAsyncClient.hxx>

#include <vnx/keyvalue/ServerClient.hxx>
#include <vnx/keyvalue/ServerAsyncClient.hxx>

#include <vnx/ThreadPool.h>

#include <regex>

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
	TopicPtr input_url_index_sync;
	TopicPtr input_page_index_sync;
	
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
	};
	
	void main() override;
	
	Object get_stats(const int32_t& limit) const override;
	
	void handle(std::shared_ptr<const TextResponse> value) override;
	
	void handle(std::shared_ptr<const keyvalue::KeyValuePair> value) override;
	
private:
	void process_page(	const std::string& url,
						std::shared_ptr<PageIndex> index,
						const std::string& content,
						const std::string& base_url,
						const std::vector<std::string>& links,
						const std::vector<std::string>& images,
						std::shared_ptr<const PageContent> robots_txt,
						bool is_reprocess);
	
	void page_process_callback(	const std::string& url_key,
								const std::shared_ptr<const PageIndex>& index,
								const bool& is_reprocess);
	
	void delete_url(const std::string& url_key);
	
	domain_t& get_domain(const std::string& host);
	
	domain_t* find_domain(const std::string& host);
	
	bool filter_url(const Url::Url& parsed) const;
	
	int enqueue(const std::string& url, int depth, int64_t load_time = 0);
	
	void check_queue();
	
	void check_url(const Url::Url& url, int depth, std::shared_ptr<const Value> index);
	
	void check_all_urls();
	
	void check_page_callback(	const std::string& url_key,
								std::shared_ptr<const Value> url_index,
								std::shared_ptr<const PageIndex> page_index);
	
	void reproc_page_callback(	const std::string& url_key,
								std::shared_ptr<const Value> page_content_,
								std::shared_ptr<const PageIndex> page_index_);
	
	void check_page(const std::string& url_key, int depth, std::shared_ptr<const PageIndex> index);
	
	void reproc_page(	const std::string& url_key,
						std::shared_ptr<const PageIndex> index,
						std::shared_ptr<const PageContent> content);
	
	url_t url_fetch_done(const std::string& url_key, bool is_fail);
	
	void url_fetch_callback(const std::string& url, std::shared_ptr<const FetchResult> result);
	
	void url_update(	const std::string& url_key,
						const std::string& new_scheme,
						const int new_depth,
						const UrlInfo& info);
	
	void url_update_callback(	const std::string& url_key,
								const std::string& new_scheme,
								const int new_depth,
								const UrlInfo& info,
								std::pair<Variant, std::shared_ptr<const Value>> pair);
	
	void robots_txt_callback(const std::string& url_key, robots_txt_state_e missing_state, std::shared_ptr<const Value> value);
	
	void url_fetch_error(uint64_t request_id, const std::exception& ex);
	
	void url_index_error(uint64_t request_id, const std::exception& ex);
	
	void page_index_error(uint64_t request_id, const std::exception& ex);
	
	void page_content_error(uint64_t request_id, const std::exception& ex);
	
	void publish_stats();
	
	void print_stats();
	
	void work_loop() const noexcept;
	
private:
	Hash64 private_addr;
	std::shared_ptr<ThreadPool> work_threads;
	std::map<uint64_t, std::string> pending_urls;
	std::multimap<int64_t, std::string> waiting;
	
	std::unordered_map<std::string, url_t> url_map;
	std::unordered_map<std::string, domain_t> domain_map;
	std::unordered_map<std::string, int64_t> pause_map;
	
	std::shared_ptr<keyvalue::ServerAsyncClient> url_index_async;
	std::shared_ptr<keyvalue::ServerAsyncClient> page_index_async;
	std::shared_ptr<keyvalue::ServerAsyncClient> page_content_async;
	std::shared_ptr<CrawlFrontendAsyncClient> crawl_frontend_async;
	
	std::shared_ptr<googlebot::RobotsMatcher> matcher;
	std::vector<std::regex> regex_blacklist_;
	
	int64_t fetch_counter = 0;
	int64_t error_counter = 0;
	int64_t reload_counter = 0;
	int64_t reproc_counter = 0;
	int64_t delete_counter = 0;
	int64_t pending_robots_txt = 0;
	int64_t missing_robots_txt = 0;
	int64_t timed_out_robots_txt = 0;
	int64_t found_robots_txt = 0;
	int64_t queue_block_count = 0;
	int64_t active_domains = 0;
	double average_depth = 0;
	
};


} // search
} // vnx

#endif /* INCLUDE_VNX_SEARCH_CRAWLPROCESSOR_H_ */
