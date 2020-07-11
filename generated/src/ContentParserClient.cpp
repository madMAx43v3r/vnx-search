
// AUTO GENERATED by vnxcppcodegen

#include <vnx/search/package.hxx>
#include <vnx/search/ContentParserClient.hxx>
#include <vnx/Module.h>
#include <vnx/ModuleInterface_vnx_get_type_code.hxx>
#include <vnx/ModuleInterface_vnx_get_type_code_return.hxx>
#include <vnx/search/ContentParser_parse.hxx>
#include <vnx/search/ContentParser_parse_return.hxx>
#include <vnx/search/HttpResponse.hxx>
#include <vnx/search/TextResponse.hxx>

#include <vnx/vnx.h>


namespace vnx {
namespace search {

ContentParserClient::ContentParserClient(const std::string& service_name)
	:	Client::Client(vnx::Hash64(service_name))
{
}

ContentParserClient::ContentParserClient(vnx::Hash64 service_addr)
	:	Client::Client(service_addr)
{
}

::vnx::TypeCode ContentParserClient::vnx_get_type_code() {
	auto _method = ::vnx::ModuleInterface_vnx_get_type_code::create();
	auto _return_value = vnx_request(_method);
	auto _result = std::dynamic_pointer_cast<const ::vnx::ModuleInterface_vnx_get_type_code_return>(_return_value);
	if(!_result) {
		throw std::logic_error("ContentParserClient: !_result");
	}
	return _result->_ret_0;
}

std::shared_ptr<const ::vnx::search::TextResponse> ContentParserClient::parse(const std::shared_ptr<const ::vnx::search::HttpResponse>& response) {
	auto _method = ::vnx::search::ContentParser_parse::create();
	_method->response = response;
	auto _return_value = vnx_request(_method);
	auto _result = std::dynamic_pointer_cast<const ::vnx::search::ContentParser_parse_return>(_return_value);
	if(!_result) {
		throw std::logic_error("ContentParserClient: !_result");
	}
	return _result->_ret_0;
}


} // namespace vnx
} // namespace search
