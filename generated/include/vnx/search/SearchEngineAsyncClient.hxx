
// AUTO GENERATED by vnxcppcodegen

#ifndef INCLUDE_vnx_search_SearchEngine_ASYNC_CLIENT_HXX_
#define INCLUDE_vnx_search_SearchEngine_ASYNC_CLIENT_HXX_

#include <vnx/AsyncClient.h>
#include <vnx/Module.h>
#include <vnx/Object.hpp>
#include <vnx/TopicPtr.hpp>
#include <vnx/keyvalue/SyncInfo.hxx>
#include <vnx/keyvalue/SyncUpdate.hxx>
#include <vnx/search/page_entry_t.hxx>


namespace vnx {
namespace search {

class SearchEngineAsyncClient : public vnx::AsyncClient {
public:
	SearchEngineAsyncClient(const std::string& service_name);
	
	SearchEngineAsyncClient(vnx::Hash64 service_addr);
	
	uint64_t vnx_get_config_object(
			const std::function<void(const ::vnx::Object&)>& _callback = std::function<void(const ::vnx::Object&)>(),
			const std::function<void(const vnx::exception&)>& _error_callback = std::function<void(const vnx::exception&)>());
	
	uint64_t vnx_get_config(const std::string& name = "", 
			const std::function<void(const ::vnx::Variant&)>& _callback = std::function<void(const ::vnx::Variant&)>(),
			const std::function<void(const vnx::exception&)>& _error_callback = std::function<void(const vnx::exception&)>());
	
	uint64_t vnx_set_config_object(const ::vnx::Object& config = ::vnx::Object(), 
			const std::function<void()>& _callback = std::function<void()>(),
			const std::function<void(const vnx::exception&)>& _error_callback = std::function<void(const vnx::exception&)>());
	
	uint64_t vnx_set_config(const std::string& name = "", const ::vnx::Variant& value = ::vnx::Variant(), 
			const std::function<void()>& _callback = std::function<void()>(),
			const std::function<void(const vnx::exception&)>& _error_callback = std::function<void(const vnx::exception&)>());
	
	uint64_t vnx_get_type_code(
			const std::function<void(const ::vnx::TypeCode&)>& _callback = std::function<void(const ::vnx::TypeCode&)>(),
			const std::function<void(const vnx::exception&)>& _error_callback = std::function<void(const vnx::exception&)>());
	
	uint64_t vnx_get_module_info(
			const std::function<void(std::shared_ptr<const ::vnx::ModuleInfo>)>& _callback = std::function<void(std::shared_ptr<const ::vnx::ModuleInfo>)>(),
			const std::function<void(const vnx::exception&)>& _error_callback = std::function<void(const vnx::exception&)>());
	
	uint64_t vnx_restart(
			const std::function<void()>& _callback = std::function<void()>(),
			const std::function<void(const vnx::exception&)>& _error_callback = std::function<void(const vnx::exception&)>());
	
	uint64_t vnx_stop(
			const std::function<void()>& _callback = std::function<void()>(),
			const std::function<void(const vnx::exception&)>& _error_callback = std::function<void(const vnx::exception&)>());
	
	uint64_t vnx_self_test(
			const std::function<void(const vnx::bool_t&)>& _callback = std::function<void(const vnx::bool_t&)>(),
			const std::function<void(const vnx::exception&)>& _error_callback = std::function<void(const vnx::exception&)>());
	
	uint64_t get_page_entries(const std::vector<uint32_t>& page_ids = {}, 
			const std::function<void(const std::vector<::vnx::search::page_entry_t>&)>& _callback = std::function<void(const std::vector<::vnx::search::page_entry_t>&)>(),
			const std::function<void(const vnx::exception&)>& _error_callback = std::function<void(const vnx::exception&)>());
	
	uint64_t get_domain_info(const std::string& host = "", const int32_t& limit = 0, const uint32_t& offset = 0, 
			const std::function<void(const ::vnx::Object&)>& _callback = std::function<void(const ::vnx::Object&)>(),
			const std::function<void(const vnx::exception&)>& _error_callback = std::function<void(const vnx::exception&)>());
	
	uint64_t get_page_info(const std::string& url_key = "", 
			const std::function<void(const ::vnx::Object&)>& _callback = std::function<void(const ::vnx::Object&)>(),
			const std::function<void(const vnx::exception&)>& _error_callback = std::function<void(const vnx::exception&)>());
	
	uint64_t get_page_ranks(const std::vector<std::string>& url_keys = {}, const vnx::bool_t& direct = 0, 
			const std::function<void(const std::vector<vnx::float32_t>&)>& _callback = std::function<void(const std::vector<vnx::float32_t>&)>(),
			const std::function<void(const vnx::exception&)>& _error_callback = std::function<void(const vnx::exception&)>());
	
	uint64_t get_domain_list(const int32_t& limit = 0, const uint32_t& offset = 0, 
			const std::function<void(const std::vector<::vnx::Object>&)>& _callback = std::function<void(const std::vector<::vnx::Object>&)>(),
			const std::function<void(const vnx::exception&)>& _error_callback = std::function<void(const vnx::exception&)>());
	
	uint64_t get_page_ranking(const int32_t& limit = 0, const uint32_t& offset = 0, 
			const std::function<void(const std::vector<std::pair<std::string, uint32_t>>&)>& _callback = std::function<void(const std::vector<std::pair<std::string, uint32_t>>&)>(),
			const std::function<void(const vnx::exception&)>& _error_callback = std::function<void(const vnx::exception&)>());
	
	uint64_t reverse_lookup(const std::string& url_key = "", 
			const std::function<void(const std::vector<std::string>&)>& _callback = std::function<void(const std::vector<std::string>&)>(),
			const std::function<void(const vnx::exception&)>& _error_callback = std::function<void(const vnx::exception&)>());
	
	uint64_t reverse_domain_lookup(const std::string& url_key = "", 
			const std::function<void(const std::vector<std::pair<std::string, uint32_t>>&)>& _callback = std::function<void(const std::vector<std::pair<std::string, uint32_t>>&)>(),
			const std::function<void(const vnx::exception&)>& _error_callback = std::function<void(const vnx::exception&)>());
	
	uint64_t suggest_words(const std::string& prefix = "", const int32_t& limit = 0, 
			const std::function<void(const std::vector<std::string>&)>& _callback = std::function<void(const std::vector<std::string>&)>(),
			const std::function<void(const vnx::exception&)>& _error_callback = std::function<void(const vnx::exception&)>());
	
	uint64_t suggest_domains(const std::string& prefix = "", const int32_t& limit = 0, 
			const std::function<void(const std::vector<std::string>&)>& _callback = std::function<void(const std::vector<std::string>&)>(),
			const std::function<void(const vnx::exception&)>& _error_callback = std::function<void(const vnx::exception&)>());
	
protected:
	int32_t vnx_purge_request(uint64_t _request_id, const vnx::exception& _ex) override;
	
	int32_t vnx_callback_switch(uint64_t _request_id, std::shared_ptr<const vnx::Value> _value) override;
	
private:
	std::unordered_map<uint64_t, std::pair<std::function<void(const ::vnx::Object&)>, std::function<void(const vnx::exception&)>>> vnx_queue_vnx_get_config_object;
	std::unordered_map<uint64_t, std::pair<std::function<void(const ::vnx::Variant&)>, std::function<void(const vnx::exception&)>>> vnx_queue_vnx_get_config;
	std::unordered_map<uint64_t, std::pair<std::function<void()>, std::function<void(const vnx::exception&)>>> vnx_queue_vnx_set_config_object;
	std::unordered_map<uint64_t, std::pair<std::function<void()>, std::function<void(const vnx::exception&)>>> vnx_queue_vnx_set_config;
	std::unordered_map<uint64_t, std::pair<std::function<void(const ::vnx::TypeCode&)>, std::function<void(const vnx::exception&)>>> vnx_queue_vnx_get_type_code;
	std::unordered_map<uint64_t, std::pair<std::function<void(std::shared_ptr<const ::vnx::ModuleInfo>)>, std::function<void(const vnx::exception&)>>> vnx_queue_vnx_get_module_info;
	std::unordered_map<uint64_t, std::pair<std::function<void()>, std::function<void(const vnx::exception&)>>> vnx_queue_vnx_restart;
	std::unordered_map<uint64_t, std::pair<std::function<void()>, std::function<void(const vnx::exception&)>>> vnx_queue_vnx_stop;
	std::unordered_map<uint64_t, std::pair<std::function<void(const vnx::bool_t&)>, std::function<void(const vnx::exception&)>>> vnx_queue_vnx_self_test;
	std::unordered_map<uint64_t, std::pair<std::function<void(const std::vector<::vnx::search::page_entry_t>&)>, std::function<void(const vnx::exception&)>>> vnx_queue_get_page_entries;
	std::unordered_map<uint64_t, std::pair<std::function<void(const ::vnx::Object&)>, std::function<void(const vnx::exception&)>>> vnx_queue_get_domain_info;
	std::unordered_map<uint64_t, std::pair<std::function<void(const ::vnx::Object&)>, std::function<void(const vnx::exception&)>>> vnx_queue_get_page_info;
	std::unordered_map<uint64_t, std::pair<std::function<void(const std::vector<vnx::float32_t>&)>, std::function<void(const vnx::exception&)>>> vnx_queue_get_page_ranks;
	std::unordered_map<uint64_t, std::pair<std::function<void(const std::vector<::vnx::Object>&)>, std::function<void(const vnx::exception&)>>> vnx_queue_get_domain_list;
	std::unordered_map<uint64_t, std::pair<std::function<void(const std::vector<std::pair<std::string, uint32_t>>&)>, std::function<void(const vnx::exception&)>>> vnx_queue_get_page_ranking;
	std::unordered_map<uint64_t, std::pair<std::function<void(const std::vector<std::string>&)>, std::function<void(const vnx::exception&)>>> vnx_queue_reverse_lookup;
	std::unordered_map<uint64_t, std::pair<std::function<void(const std::vector<std::pair<std::string, uint32_t>>&)>, std::function<void(const vnx::exception&)>>> vnx_queue_reverse_domain_lookup;
	std::unordered_map<uint64_t, std::pair<std::function<void(const std::vector<std::string>&)>, std::function<void(const vnx::exception&)>>> vnx_queue_suggest_words;
	std::unordered_map<uint64_t, std::pair<std::function<void(const std::vector<std::string>&)>, std::function<void(const vnx::exception&)>>> vnx_queue_suggest_domains;
	
};


} // namespace vnx
} // namespace search

#endif // INCLUDE_vnx_search_SearchEngine_ASYNC_CLIENT_HXX_
