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
#include <vnx/keyvalue/ServerClient.hxx>
#include <vnx/keyvalue/ServerAsyncClient.hxx>


namespace vnx {
namespace search {

class CrawlProcessor : public CrawlProcessorBase {
public:
	CrawlProcessor(const std::string& _vnx_name);
	
protected:
	struct url_t {
		int depth = 0;
	};
	
	struct domain_t {
		int num_pending = 0;
		int64_t last_fetch = 0;
	};
	
	void main() override;
	
	void handle(std::shared_ptr<const keyvalue::KeyValuePair> value) override;
	
private:
	void enqueue(const std::string& url, int depth, uint32_t load_time = 0);
	
	void check_queue();
	
	void check_url(const std::string& url, int depth, std::shared_ptr<const Value> index);
	
	void check_page_callback(const std::string& url, std::shared_ptr<const Value> url_index_, std::shared_ptr<const PageIndex> page_index_);
	
	void check_page(const std::string& url, int depth, std::shared_ptr<const PageIndex> index);
	
	void url_fetched(const std::string& url, int depth, uint64_t req_id, std::shared_ptr<const UrlIndex> index);
	
	void url_fetch_error(uint64_t req_id, const std::exception& ex);
	
	void url_index_error(uint64_t req_id, const std::exception& ex);
	
private:
	std::multimap<int, std::string> queue;
	std::multimap<uint32_t, std::string> waiting;
	
	std::map<std::string, url_t> url_map;
	std::map<std::string, domain_t> domain_map;
	
	std::map<uint64_t, std::string> pending_urls;
	
	std::shared_ptr<keyvalue::ServerClient> url_index;
	std::shared_ptr<keyvalue::ServerAsyncClient> url_index_async;
	
};


} // search
} // vnx

#endif /* INCLUDE_VNX_SEARCH_CRAWLPROCESSOR_H_ */
