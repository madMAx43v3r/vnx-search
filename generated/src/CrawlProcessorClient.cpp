
// AUTO GENERATED by vnxcppcodegen

#include <vnx/search/package.hxx>
#include <vnx/search/CrawlProcessorClient.hxx>
#include <vnx/Module.h>
#include <vnx/ModuleInterface_vnx_close.hxx>
#include <vnx/ModuleInterface_vnx_close_return.hxx>
#include <vnx/ModuleInterface_vnx_get_config.hxx>
#include <vnx/ModuleInterface_vnx_get_config_object.hxx>
#include <vnx/ModuleInterface_vnx_get_config_object_return.hxx>
#include <vnx/ModuleInterface_vnx_get_config_return.hxx>
#include <vnx/ModuleInterface_vnx_get_type_code.hxx>
#include <vnx/ModuleInterface_vnx_get_type_code_return.hxx>
#include <vnx/ModuleInterface_vnx_restart.hxx>
#include <vnx/ModuleInterface_vnx_restart_return.hxx>
#include <vnx/ModuleInterface_vnx_set_config.hxx>
#include <vnx/ModuleInterface_vnx_set_config_object.hxx>
#include <vnx/ModuleInterface_vnx_set_config_object_return.hxx>
#include <vnx/ModuleInterface_vnx_set_config_return.hxx>
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

::vnx::Object CrawlProcessorClient::vnx_get_config_object() {
	auto _method = ::vnx::ModuleInterface_vnx_get_config_object::create();
	auto _return_value = vnx_request(_method, false);
	auto _result = std::dynamic_pointer_cast<const ::vnx::ModuleInterface_vnx_get_config_object_return>(_return_value);
	if(!_result) {
		throw std::logic_error("CrawlProcessorClient: !_result");
	}
	return _result->_ret_0;
}

::vnx::Variant CrawlProcessorClient::vnx_get_config(const std::string& name) {
	auto _method = ::vnx::ModuleInterface_vnx_get_config::create();
	_method->name = name;
	auto _return_value = vnx_request(_method, false);
	auto _result = std::dynamic_pointer_cast<const ::vnx::ModuleInterface_vnx_get_config_return>(_return_value);
	if(!_result) {
		throw std::logic_error("CrawlProcessorClient: !_result");
	}
	return _result->_ret_0;
}

void CrawlProcessorClient::vnx_set_config_object(const ::vnx::Object& config) {
	auto _method = ::vnx::ModuleInterface_vnx_set_config_object::create();
	_method->config = config;
	vnx_request(_method, false);
}

void CrawlProcessorClient::vnx_set_config_object_async(const ::vnx::Object& config) {
	auto _method = ::vnx::ModuleInterface_vnx_set_config_object::create();
	_method->config = config;
	vnx_request(_method, true);
}

void CrawlProcessorClient::vnx_set_config(const std::string& name, const ::vnx::Variant& value) {
	auto _method = ::vnx::ModuleInterface_vnx_set_config::create();
	_method->name = name;
	_method->value = value;
	vnx_request(_method, false);
}

void CrawlProcessorClient::vnx_set_config_async(const std::string& name, const ::vnx::Variant& value) {
	auto _method = ::vnx::ModuleInterface_vnx_set_config::create();
	_method->name = name;
	_method->value = value;
	vnx_request(_method, true);
}

::vnx::TypeCode CrawlProcessorClient::vnx_get_type_code() {
	auto _method = ::vnx::ModuleInterface_vnx_get_type_code::create();
	auto _return_value = vnx_request(_method, false);
	auto _result = std::dynamic_pointer_cast<const ::vnx::ModuleInterface_vnx_get_type_code_return>(_return_value);
	if(!_result) {
		throw std::logic_error("CrawlProcessorClient: !_result");
	}
	return _result->_ret_0;
}

void CrawlProcessorClient::vnx_restart() {
	auto _method = ::vnx::ModuleInterface_vnx_restart::create();
	vnx_request(_method, false);
}

void CrawlProcessorClient::vnx_restart_async() {
	auto _method = ::vnx::ModuleInterface_vnx_restart::create();
	vnx_request(_method, true);
}

void CrawlProcessorClient::vnx_close() {
	auto _method = ::vnx::ModuleInterface_vnx_close::create();
	vnx_request(_method, false);
}

void CrawlProcessorClient::vnx_close_async() {
	auto _method = ::vnx::ModuleInterface_vnx_close::create();
	vnx_request(_method, true);
}

::vnx::Object CrawlProcessorClient::get_stats(const int32_t& limit) {
	auto _method = ::vnx::search::CrawlProcessor_get_stats::create();
	_method->limit = limit;
	auto _return_value = vnx_request(_method, false);
	auto _result = std::dynamic_pointer_cast<const ::vnx::search::CrawlProcessor_get_stats_return>(_return_value);
	if(!_result) {
		throw std::logic_error("CrawlProcessorClient: !_result");
	}
	return _result->_ret_0;
}


} // namespace vnx
} // namespace search
