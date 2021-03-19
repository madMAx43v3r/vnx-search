/*
 * ArchiveServer.cpp
 *
 *  Created on: Aug 17, 2020
 *      Author: mad
 */

#include <vnx/search/ArchiveServer.h>
#include <vnx/search/UrlIndex.hxx>
#include <vnx/search/Util.h>


namespace vnx {
namespace search {

ArchiveServer::ArchiveServer(const std::string& _vnx_name)
	:	ArchiveServerBase(_vnx_name)
{
}

void ArchiveServer::init()
{
	vnx::open_pipe(vnx_name, this, 100);
}

void ArchiveServer::main()
{
	http_archive_async = std::make_shared<keyvalue::StorageAsyncClient>(http_archive_server);
	
	add_async_client(http_archive_async);
	
	Super::main();
}

void ArchiveServer::http_request_async(	std::shared_ptr<const addons::HttpRequest> request,
										const std::string& sub_path,
										const request_id_t& req_id) const
{
	if(request->method != "GET") {
		throw std::logic_error("invalid method");
	}
	const auto url_key = "//" + request->url.substr(path.size());
	log(INFO) << "GET " << url_key;
	
	http_archive_async->get_value(Variant(url_key),
			std::bind(&ArchiveServer::http_response, this, req_id, std::placeholders::_1),
			std::bind(&ArchiveServer::http_failure, this, req_id, std::placeholders::_1));
}

void ArchiveServer::http_request_chunk_async(	std::shared_ptr<const addons::HttpRequest> request,
												const std::string& sub_path,
												const int64_t& offset, const int64_t& max_bytes,
												const request_id_t& req_id) const
{
	throw std::logic_error("not implemented");
}

void ArchiveServer::http_response(	const request_id_t& req_id,
									std::shared_ptr<const keyvalue::Entry> entry) const
{
	auto response = addons::HttpResponse::create();
	response->status = 404;
	
	if(auto http = std::dynamic_pointer_cast<const HttpResponse>(entry->value))
	{
		response->status = 200;
		response->content_type = http->content_type;
		if(!http->content_charset.empty()) {
			response->content_type += "; charset=" + http->content_charset;
		}
		response->payload = http->payload;
	}
	http_request_async_return(req_id, response);
}

void ArchiveServer::http_failure(const request_id_t& req_id, const vnx::exception& ex) const
{
	auto response = addons::HttpResponse::create();
	response->status = 404;
	http_request_async_return(req_id, response);
}


} // search
} // vnx
