
// AUTO GENERATED by vnxcppcodegen

#include <vnx/search/package.hxx>
#include <vnx/search/ArchiveServerClient.hxx>
#include <vnx/Module.h>
#include <vnx/ModuleInterface_vnx_get_type_code.hxx>
#include <vnx/ModuleInterface_vnx_get_type_code_return.hxx>
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

::vnx::TypeCode ArchiveServerClient::vnx_get_type_code() {
	auto _method = ::vnx::ModuleInterface_vnx_get_type_code::create();
	auto _return_value = vnx_request(_method);
	auto _result = std::dynamic_pointer_cast<const ::vnx::ModuleInterface_vnx_get_type_code_return>(_return_value);
	if(!_result) {
		throw std::logic_error("ArchiveServerClient: !_result");
	}
	return _result->_ret_0;
}


} // namespace vnx
} // namespace search
