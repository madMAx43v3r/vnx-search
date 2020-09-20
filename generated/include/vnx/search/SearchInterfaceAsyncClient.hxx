
// AUTO GENERATED by vnxcppcodegen

#ifndef INCLUDE_vnx_search_SearchInterface_ASYNC_CLIENT_HXX_
#define INCLUDE_vnx_search_SearchInterface_ASYNC_CLIENT_HXX_

#include <vnx/AsyncClient.h>
#include <vnx/Object.hpp>


namespace vnx {
namespace search {

class SearchInterfaceAsyncClient : public vnx::AsyncClient {
public:
	SearchInterfaceAsyncClient(const std::string& service_name);
	
	SearchInterfaceAsyncClient(vnx::Hash64 service_addr);
	
	uint64_t get_domain_info(const std::string& host, const int32_t& limit, const uint32_t& offset, 
			const std::function<void(::vnx::Object)>& _callback = std::function<void(::vnx::Object)>(),
			const std::function<void(const std::exception&)>& _error_callback = std::function<void(const std::exception&)>());
	
	uint64_t get_page_info(const std::string& url_key, 
			const std::function<void(::vnx::Object)>& _callback = std::function<void(::vnx::Object)>(),
			const std::function<void(const std::exception&)>& _error_callback = std::function<void(const std::exception&)>());
	
	uint64_t get_page_ranks(const std::vector<std::string>& url_keys, const vnx::bool_t& direct, 
			const std::function<void(std::vector<vnx::float32_t>)>& _callback = std::function<void(std::vector<vnx::float32_t>)>(),
			const std::function<void(const std::exception&)>& _error_callback = std::function<void(const std::exception&)>());
	
	uint64_t get_domain_list(const int32_t& limit, const uint32_t& offset, 
			const std::function<void(std::vector<::vnx::Object>)>& _callback = std::function<void(std::vector<::vnx::Object>)>(),
			const std::function<void(const std::exception&)>& _error_callback = std::function<void(const std::exception&)>());
	
	uint64_t reverse_lookup(const std::string& url_key, 
			const std::function<void(std::vector<std::string>)>& _callback = std::function<void(std::vector<std::string>)>(),
			const std::function<void(const std::exception&)>& _error_callback = std::function<void(const std::exception&)>());
	
	uint64_t reverse_domain_lookup(const std::string& url_key, 
			const std::function<void(std::vector<std::pair<std::string, uint32_t>>)>& _callback = std::function<void(std::vector<std::pair<std::string, uint32_t>>)>(),
			const std::function<void(const std::exception&)>& _error_callback = std::function<void(const std::exception&)>());
	
	uint64_t suggest_words(const std::string& prefix, const int32_t& limit, 
			const std::function<void(std::vector<std::string>)>& _callback = std::function<void(std::vector<std::string>)>(),
			const std::function<void(const std::exception&)>& _error_callback = std::function<void(const std::exception&)>());
	
	uint64_t suggest_domains(const std::string& prefix, const int32_t& limit, 
			const std::function<void(std::vector<std::string>)>& _callback = std::function<void(std::vector<std::string>)>(),
			const std::function<void(const std::exception&)>& _error_callback = std::function<void(const std::exception&)>());
	
	std::vector<uint64_t> vnx_get_pending_ids() const override;
	
protected:
	void vnx_purge_request(uint64_t _request_id, const std::exception& _ex) override;
	
	void vnx_callback_switch(uint64_t _request_id, std::shared_ptr<const vnx::Value> _value) override;
	
private:
	std::map<uint64_t, std::pair<std::function<void(::vnx::Object)>, std::function<void(const std::exception&)>>> vnx_queue_get_domain_info;
	std::map<uint64_t, std::pair<std::function<void(::vnx::Object)>, std::function<void(const std::exception&)>>> vnx_queue_get_page_info;
	std::map<uint64_t, std::pair<std::function<void(std::vector<vnx::float32_t>)>, std::function<void(const std::exception&)>>> vnx_queue_get_page_ranks;
	std::map<uint64_t, std::pair<std::function<void(std::vector<::vnx::Object>)>, std::function<void(const std::exception&)>>> vnx_queue_get_domain_list;
	std::map<uint64_t, std::pair<std::function<void(std::vector<std::string>)>, std::function<void(const std::exception&)>>> vnx_queue_reverse_lookup;
	std::map<uint64_t, std::pair<std::function<void(std::vector<std::pair<std::string, uint32_t>>)>, std::function<void(const std::exception&)>>> vnx_queue_reverse_domain_lookup;
	std::map<uint64_t, std::pair<std::function<void(std::vector<std::string>)>, std::function<void(const std::exception&)>>> vnx_queue_suggest_words;
	std::map<uint64_t, std::pair<std::function<void(std::vector<std::string>)>, std::function<void(const std::exception&)>>> vnx_queue_suggest_domains;
	
};


} // namespace vnx
} // namespace search

#endif // INCLUDE_vnx_search_SearchInterface_ASYNC_CLIENT_HXX_
