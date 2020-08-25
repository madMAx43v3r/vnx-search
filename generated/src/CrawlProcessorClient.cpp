
// AUTO GENERATED by vnxcppcodegen

#include <vnx/search/package.hxx>
#include <vnx/search/CrawlProcessorClient.hxx>
#include <vnx/Module.h>
#include <vnx/ModuleInterface_vnx_get_type_code.hxx>
#include <vnx/ModuleInterface_vnx_get_type_code_return.hxx>
#include <vnx/Object.hpp>
#include <vnx/TopicPtr.hpp>
#include <vnx/keyvalue/SyncUpdate.hxx>
#include <vnx/search/CrawlProcessor_get_stats.hxx>
#include <vnx/search/CrawlProcessor_get_stats_return.hxx>

#include <vnx/vnx.h>


namespace vnx {
namespace search {

CrawlProcessorClient::CrawlProcessorClient(const std::string& service_name)
	:	Client::Client(vnx::Hash64(service_name))
{
}

CrawlProcessorClient::CrawlProcessorClient(vnx::Hash64 service_addr)
	:	Client::Client(service_addr)
{
}

::vnx::TypeCode CrawlProcessorClient::vnx_get_type_code() {
	auto _method = ::vnx::ModuleInterface_vnx_get_type_code::create();
	auto _return_value = vnx_request(_method);
	auto _result = std::dynamic_pointer_cast<const ::vnx::ModuleInterface_vnx_get_type_code_return>(_return_value);
	if(!_result) {
		throw std::logic_error("CrawlProcessorClient: !_result");
	}
	return _result->_ret_0;
}

::vnx::Object CrawlProcessorClient::get_stats(const int32_t& limit) {
	auto _method = ::vnx::search::CrawlProcessor_get_stats::create();
	_method->limit = limit;
	auto _return_value = vnx_request(_method);
	auto _result = std::dynamic_pointer_cast<const ::vnx::search::CrawlProcessor_get_stats_return>(_return_value);
	if(!_result) {
		throw std::logic_error("CrawlProcessorClient: !_result");
	}
	return _result->_ret_0;
}


} // namespace vnx
} // namespace search
