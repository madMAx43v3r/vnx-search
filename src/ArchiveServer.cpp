/*
 * ArchiveServer.cpp
 *
 *  Created on: Aug 17, 2020
 *      Author: mad
 */

#include <vnx/search/ArchiveServer.h>
#include <vnx/search/UrlIndex.hxx>
#include <vnx/search/Util.h>

#include <libxml/HTMLparser.h>
#include <libxml++/libxml++.h>


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
	const auto url_key = "//" + request->url.substr(path.size());
	log(DEBUG) << "GET '" << url_key << "'";
	
	http_archive_async->get_value(Variant(url_key),
			std::bind(&ArchiveServer::http_response, this, url_key, req_id, std::placeholders::_1),
			std::bind(&ArchiveServer::http_failure, this, req_id, std::placeholders::_1));
}

void ArchiveServer::http_request_chunk_async(	std::shared_ptr<const addons::HttpRequest> request,
												const std::string& sub_path,
												const int64_t& offset, const int64_t& max_bytes,
												const request_id_t& req_id) const
{
	throw std::logic_error("not implemented");
}

void ArchiveServer::http_response(	const std::string& url_key, const request_id_t& req_id,
									std::shared_ptr<const keyvalue::Entry> entry) const
{
	auto response = addons::HttpResponse::create();
	response->status = 404;
	
	if(auto http = std::dynamic_pointer_cast<const HttpResponse>(entry->value))
	{
		if(g_html_content_types.count(http->content_type)) {
			http = transform(http, url_key);
		}
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

std::shared_ptr<HttpResponse>
ArchiveServer::transform(	std::shared_ptr<const HttpResponse> http,
							const std::string& url_key) const
{
	const Url::Url parsed(url_key);
	const auto domain = parsed.host();
	const auto content = http->payload.as_string();
	
	xmlDoc* doc = ::htmlReadDoc((const xmlChar*)content.data(), 0, http->content_charset.c_str(),
			HTML_PARSE_RECOVER | HTML_PARSE_NOERROR | HTML_PARSE_NOWARNING);
	
	if(!doc) {
		throw std::runtime_error("htmlReadDoc() failed");
	}
	xmlpp::Document* doc_pp = new xmlpp::Document(doc);
	xmlpp::Element* root = doc_pp->get_root_node();
	
	if(!root) {
		throw std::runtime_error("get_root_node() failed");
	}
	
	for(const auto& entry : link_map) {
		const auto name = entry.first;
		const auto attr = entry.second;
		
		for(auto node : root->find("//" + name)) {
			if(auto elem = dynamic_cast<xmlpp::Element*>(node)) {
				elem->set_attribute(attr, transform_link(domain, elem->get_attribute_value(attr)));
			}
		}
	}
	
	auto out = vnx::clone(http);
	out->payload = doc_pp->write_to_string("UTF-8");
	return out;
}

std::string ArchiveServer::transform_link(const std::string& domain, const std::string& url) const
{
	const Url::Url parsed(url);
	const auto url_host = parsed.host();
	const auto url_path = parsed.path();
	if(url_path.empty() && url_path.front() != '/') {
		return url;
	}
	std::string out = path;
	if(url_host.empty()) {
		out += domain;
	} else {
		out += url_host;
	}
	out += parsed.fullpath();
	return out;
}


} // search
} // vnx
