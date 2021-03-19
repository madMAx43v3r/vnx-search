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
#include <vnx/keyvalue/StorageAsyncClient.hxx>


namespace vnx {
namespace search {

class ArchiveServer : public ArchiveServerBase {
public:
	ArchiveServer(const std::string& _vnx_name);
	
protected:
	void init() override;
	
	void main() override;
	
	void http_request_async(std::shared_ptr<const addons::HttpRequest> request,
							const std::string& sub_path,
							const request_id_t& req_id) const override;
	
	void http_request_chunk_async(	std::shared_ptr<const addons::HttpRequest> request,
									const std::string& sub_path,
									const int64_t& offset, const int64_t& max_bytes,
									const request_id_t& req_id) const override;
	
private:
	void http_response(const request_id_t& req_id, std::shared_ptr<const keyvalue::Entry> entry) const;
	
	void http_failure(const request_id_t& req_id, const vnx::exception& ex) const;
	
private:
	std::shared_ptr<keyvalue::StorageAsyncClient> http_archive_async;
	
};


} // search
} // vnx

#endif /* INCLUDE_VNX_SEARCH_ARCHIVESERVER_H_ */
