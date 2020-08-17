/*
 * ArchiveServer.h
 *
 *  Created on: Aug 17, 2020
 *      Author: mad
 */

#ifndef INCLUDE_VNX_SEARCH_ARCHIVESERVER_H_
#define INCLUDE_VNX_SEARCH_ARCHIVESERVER_H_

#include <vnx/search/ArchiveServerBase.hxx>
#include <vnx/search/HttpResponse.hxx>
#include <vnx/search/CrawlFrontendAsyncClient.hxx>
#include <vnx/keyvalue/StorageAsyncClient.hxx>
#include <vnx/ThreadPool.h>


namespace vnx {
namespace search {

class ArchiveServer : public ArchiveServerBase {
public:
	ArchiveServer(const std::string& _vnx_name);
	
protected:
	void init() override;
	
	void main() override;
	
	void handle(std::shared_ptr<const keyvalue::SyncUpdate> entry) override;
	
private:
	struct url_t {
		uint64_t request_id = -1;
	};
	
	struct domain_t {
		int64_t last_fetch_us = 0;
		std::queue<std::string> queue;
	};
	
	void check_queue();
	
	void parse_html(std::shared_ptr<const HttpResponse> response);
	
	void check_url(const std::string& url);
	
	void check_url_callback_1(	const std::string& url,
								std::shared_ptr<const keyvalue::Entry> entry);
	
	void check_url_callback_2(	const std::string& url,
								std::shared_ptr<const UrlIndex> url_index,
								std::shared_ptr<const keyvalue::Entry> entry);
	
	void load_callback(const std::string& url, UrlInfo url_info);
	
	void url_update(	const std::string& url_key,
						const std::string& new_scheme,
						const UrlInfo& info);
	
	void url_update_callback(	const std::string& url_key,
								const std::string& new_scheme,
								const UrlInfo& info,
								std::shared_ptr<const keyvalue::Entry> entry);
	
	void print_stats();
	
private:
	std::unordered_map<std::string, url_t> url_map;
	std::map<std::string, domain_t> domain_map;
	
	std::shared_ptr<ThreadPool> threads;
	std::shared_ptr<CrawlFrontendAsyncClient> frontend_async;
	std::shared_ptr<keyvalue::StorageAsyncClient> http_archive_async;
	std::shared_ptr<keyvalue::StorageAsyncClient> url_index_async;
	
	int64_t fetch_counter = 0;
	int64_t error_counter = 0;
	
};


} // search
} // vnx

#endif /* INCLUDE_VNX_SEARCH_ARCHIVESERVER_H_ */
