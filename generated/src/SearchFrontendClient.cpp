
// AUTO GENERATED by vnxcppcodegen

#include <vnx/search/package.hxx>
#include <vnx/search/SearchFrontendClient.hxx>
#include <vnx/Module.h>
#include <vnx/ModuleInterface_vnx_get_type_code.hxx>
#include <vnx/ModuleInterface_vnx_get_type_code_return.hxx>
#include <vnx/addons/HttpComponent_http_request.hxx>
#include <vnx/addons/HttpComponent_http_request_return.hxx>
#include <vnx/addons/HttpRequest.hxx>
#include <vnx/addons/HttpResponse.hxx>
#include <vnx/search/query_options_t.hxx>

#include <vnx/vnx.h>


namespace vnx {
namespace search {

SearchFrontendClient::SearchFrontendClient(const std::string& service_name)
	:	Client::Client(vnx::Hash64(service_name))
{
}

SearchFrontendClient::SearchFrontendClient(vnx::Hash64 service_addr)
	:	Client::Client(service_addr)
{
}

::vnx::TypeCode SearchFrontendClient::vnx_get_type_code() {
	auto _method = ::vnx::ModuleInterface_vnx_get_type_code::create();
	auto _return_value = vnx_request(_method);
	auto _result = std::dynamic_pointer_cast<const ::vnx::ModuleInterface_vnx_get_type_code_return>(_return_value);
	if(!_result) {
		throw std::logic_error("SearchFrontendClient: !_result");
	}
	return _result->_ret_0;
}

std::shared_ptr<const ::vnx::addons::HttpResponse> SearchFrontendClient::http_request(const std::shared_ptr<const ::vnx::addons::HttpRequest>& request, const std::string& sub_path) {
	auto _method = ::vnx::addons::HttpComponent_http_request::create();
	_method->request = request;
	_method->sub_path = sub_path;
	auto _return_value = vnx_request(_method);
	auto _result = std::dynamic_pointer_cast<const ::vnx::addons::HttpComponent_http_request_return>(_return_value);
	if(!_result) {
		throw std::logic_error("SearchFrontendClient: !_result");
	}
	return _result->_ret_0;
}


} // namespace vnx
} // namespace search
