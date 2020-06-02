
// AUTO GENERATED by vnxcppcodegen

#ifndef INCLUDE_vnx_search_ContentParser_CLIENT_HXX_
#define INCLUDE_vnx_search_ContentParser_CLIENT_HXX_

#include <vnx/Client.h>
#include <vnx/Module.h>
#include <vnx/search/HttpResponse.hxx>
#include <vnx/search/TextResponse.hxx>


namespace vnx {
namespace search {

class ContentParserClient : public vnx::Client {
public:
	ContentParserClient(const std::string& service_name);
	
	ContentParserClient(vnx::Hash64 service_addr);
	
	std::shared_ptr<const ::vnx::search::TextResponse> parse(const std::shared_ptr<const ::vnx::search::HttpResponse>& response);
	
};


} // namespace vnx
} // namespace search

#endif // INCLUDE_vnx_search_ContentParser_CLIENT_HXX_
