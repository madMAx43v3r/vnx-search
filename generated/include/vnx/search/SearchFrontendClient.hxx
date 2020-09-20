
// AUTO GENERATED by vnxcppcodegen

#ifndef INCLUDE_vnx_search_SearchFrontend_CLIENT_HXX_
#define INCLUDE_vnx_search_SearchFrontend_CLIENT_HXX_

#include <vnx/Client.h>
#include <vnx/Module.h>
#include <vnx/addons/HttpRequest.hxx>
#include <vnx/addons/HttpResponse.hxx>
#include <vnx/search/query_options_t.hxx>


namespace vnx {
namespace search {

class SearchFrontendClient : public vnx::Client {
public:
	SearchFrontendClient(const std::string& service_name);
	
	SearchFrontendClient(vnx::Hash64 service_addr);
	
	::vnx::TypeCode vnx_get_type_code();
	
	std::shared_ptr<const ::vnx::addons::HttpResponse> http_request(const std::shared_ptr<const ::vnx::addons::HttpRequest>& request, const std::string& sub_path);
	
};


} // namespace vnx
} // namespace search

#endif // INCLUDE_vnx_search_SearchFrontend_CLIENT_HXX_
