
// AUTO GENERATED by vnxcppcodegen

#ifndef INCLUDE_vnx_search_QueryEngine_CLIENT_HXX_
#define INCLUDE_vnx_search_QueryEngine_CLIENT_HXX_

#include <vnx/Client.h>
#include <vnx/Module.h>
#include <vnx/search/SearchResult.hxx>
#include <vnx/search/query_options_t.hxx>


namespace vnx {
namespace search {

class QueryEngineClient : public vnx::Client {
public:
	QueryEngineClient(const std::string& service_name);
	
	QueryEngineClient(vnx::Hash64 service_addr);
	
	::vnx::Object vnx_get_config_object();
	
	::vnx::Variant vnx_get_config(const std::string& name);
	
	void vnx_set_config_object(const ::vnx::Object& config);
	
	void vnx_set_config_object_async(const ::vnx::Object& config);
	
	void vnx_set_config(const std::string& name, const ::vnx::Variant& value);
	
	void vnx_set_config_async(const std::string& name, const ::vnx::Variant& value);
	
	::vnx::TypeCode vnx_get_type_code();
	
	std::shared_ptr<const ::vnx::ModuleInfo> vnx_get_module_info();
	
	void vnx_restart();
	
	void vnx_restart_async();
	
	void vnx_stop();
	
	void vnx_stop_async();
	
	vnx::bool_t vnx_self_test();
	
	std::shared_ptr<const ::vnx::search::SearchResult> query(const std::vector<std::string>& words, const ::vnx::search::query_options_t& options);
	
};


} // namespace vnx
} // namespace search

#endif // INCLUDE_vnx_search_QueryEngine_CLIENT_HXX_
