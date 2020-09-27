
// AUTO GENERATED by vnxcppcodegen

#include <vnx/search/package.hxx>
#include <vnx/search/ArchiveServerClient.hxx>
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
#include <vnx/TopicPtr.hpp>
#include <vnx/keyvalue/SyncUpdate.hxx>

#include <vnx/vnx.h>


namespace vnx {
namespace search {

ArchiveServerClient::ArchiveServerClient(const std::string& service_name)
	:	Client::Client(vnx::Hash64(service_name))
{
}

ArchiveServerClient::ArchiveServerClient(vnx::Hash64 service_addr)
	:	Client::Client(service_addr)
{
}

::vnx::Object ArchiveServerClient::vnx_get_config_object() {
	auto _method = ::vnx::ModuleInterface_vnx_get_config_object::create();
	auto _return_value = vnx_request(_method, false);
	auto _result = std::dynamic_pointer_cast<const ::vnx::ModuleInterface_vnx_get_config_object_return>(_return_value);
	if(!_result) {
		throw std::logic_error("ArchiveServerClient: !_result");
	}
	return _result->_ret_0;
}

::vnx::Variant ArchiveServerClient::vnx_get_config(const std::string& name) {
	auto _method = ::vnx::ModuleInterface_vnx_get_config::create();
	_method->name = name;
	auto _return_value = vnx_request(_method, false);
	auto _result = std::dynamic_pointer_cast<const ::vnx::ModuleInterface_vnx_get_config_return>(_return_value);
	if(!_result) {
		throw std::logic_error("ArchiveServerClient: !_result");
	}
	return _result->_ret_0;
}

void ArchiveServerClient::vnx_set_config_object(const ::vnx::Object& config) {
	auto _method = ::vnx::ModuleInterface_vnx_set_config_object::create();
	_method->config = config;
	vnx_request(_method, false);
}

void ArchiveServerClient::vnx_set_config_object_async(const ::vnx::Object& config) {
	auto _method = ::vnx::ModuleInterface_vnx_set_config_object::create();
	_method->config = config;
	vnx_request(_method, true);
}

void ArchiveServerClient::vnx_set_config(const std::string& name, const ::vnx::Variant& value) {
	auto _method = ::vnx::ModuleInterface_vnx_set_config::create();
	_method->name = name;
	_method->value = value;
	vnx_request(_method, false);
}

void ArchiveServerClient::vnx_set_config_async(const std::string& name, const ::vnx::Variant& value) {
	auto _method = ::vnx::ModuleInterface_vnx_set_config::create();
	_method->name = name;
	_method->value = value;
	vnx_request(_method, true);
}

::vnx::TypeCode ArchiveServerClient::vnx_get_type_code() {
	auto _method = ::vnx::ModuleInterface_vnx_get_type_code::create();
	auto _return_value = vnx_request(_method, false);
	auto _result = std::dynamic_pointer_cast<const ::vnx::ModuleInterface_vnx_get_type_code_return>(_return_value);
	if(!_result) {
		throw std::logic_error("ArchiveServerClient: !_result");
	}
	return _result->_ret_0;
}

void ArchiveServerClient::vnx_restart() {
	auto _method = ::vnx::ModuleInterface_vnx_restart::create();
	vnx_request(_method, false);
}

void ArchiveServerClient::vnx_restart_async() {
	auto _method = ::vnx::ModuleInterface_vnx_restart::create();
	vnx_request(_method, true);
}

void ArchiveServerClient::vnx_close() {
	auto _method = ::vnx::ModuleInterface_vnx_close::create();
	vnx_request(_method, false);
}

void ArchiveServerClient::vnx_close_async() {
	auto _method = ::vnx::ModuleInterface_vnx_close::create();
	vnx_request(_method, true);
}


} // namespace vnx
} // namespace search
