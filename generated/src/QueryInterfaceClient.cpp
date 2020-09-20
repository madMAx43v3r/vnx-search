
// AUTO GENERATED by vnxcppcodegen

#include <vnx/search/package.hxx>
#include <vnx/search/QueryInterfaceClient.hxx>
#include <vnx/search/QueryInterface_query.hxx>
#include <vnx/search/QueryInterface_query_return.hxx>
#include <vnx/search/SearchResult.hxx>
#include <vnx/search/query_options_t.hxx>

#include <vnx/vnx.h>


namespace vnx {
namespace search {

QueryInterfaceClient::QueryInterfaceClient(const std::string& service_name)
	:	Client::Client(vnx::Hash64(service_name))
{
}

QueryInterfaceClient::QueryInterfaceClient(vnx::Hash64 service_addr)
	:	Client::Client(service_addr)
{
}

std::shared_ptr<const ::vnx::search::SearchResult> QueryInterfaceClient::query(const std::vector<std::string>& words, const ::vnx::search::query_options_t& options) {
	auto _method = ::vnx::search::QueryInterface_query::create();
	_method->words = words;
	_method->options = options;
	auto _return_value = vnx_request(_method);
	auto _result = std::dynamic_pointer_cast<const ::vnx::search::QueryInterface_query_return>(_return_value);
	if(!_result) {
		throw std::logic_error("QueryInterfaceClient: !_result");
	}
	return _result->_ret_0;
}


} // namespace vnx
} // namespace search