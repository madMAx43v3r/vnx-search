
// AUTO GENERATED by vnxcppcodegen

#ifndef INCLUDE_vnx_search_SearchEngine_ASYNC_CLIENT_HXX_
#define INCLUDE_vnx_search_SearchEngine_ASYNC_CLIENT_HXX_

#include <vnx/AsyncClient.h>
#include <vnx/Module.h>
#include <vnx/TopicPtr.h>
#include <vnx/keyvalue/KeyValuePair.hxx>
#include <vnx/keyvalue/SyncInfo.hxx>
#include <vnx/search/DomainIndex.hxx>
#include <vnx/search/SearchResult.hxx>
#include <vnx/search/search_flags_e.hxx>


namespace vnx {
namespace search {

class SearchEngineAsyncClient : public vnx::AsyncClient {
public:
	SearchEngineAsyncClient(const std::string& service_name);
	
	SearchEngineAsyncClient(vnx::Hash64 service_addr);
	
	uint64_t get_domain_info(const ::std::string& host, const ::int32_t& limit, const ::uint32_t& offset, 
			const std::function<void(::std::shared_ptr<const ::vnx::search::DomainIndex>)>& _callback = std::function<void(::std::shared_ptr<const ::vnx::search::DomainIndex>)>());
	
	uint64_t handle(const ::std::shared_ptr<const ::vnx::keyvalue::KeyValuePair>& sample, 
			const std::function<void()>& _callback = std::function<void()>());
	
	uint64_t handle(const ::std::shared_ptr<const ::vnx::keyvalue::SyncInfo>& sample, 
			const std::function<void()>& _callback = std::function<void()>());
	
	uint64_t query(const ::std::vector<::std::string>& words, const ::int32_t& limit, const ::uint32_t& offset, const ::std::vector<::vnx::search::search_flags_e>& flags, 
			const std::function<void(::std::shared_ptr<const ::vnx::search::SearchResult>)>& _callback = std::function<void(::std::shared_ptr<const ::vnx::search::SearchResult>)>());
	
	uint64_t reverse_lookup(const ::std::string& url_key, 
			const std::function<void(::std::vector<::std::string>)>& _callback = std::function<void(::std::vector<::std::string>)>());
	
	uint64_t suggest_domains(const ::std::string& prefix, const ::int32_t& limit, 
			const std::function<void(::std::vector<::std::string>)>& _callback = std::function<void(::std::vector<::std::string>)>());
	
	uint64_t suggest_words(const ::std::string& prefix, const ::int32_t& limit, 
			const std::function<void(::std::vector<::std::string>)>& _callback = std::function<void(::std::vector<::std::string>)>());
	
	std::vector<uint64_t> vnx_get_pending_ids() const override;
	
protected:
	void vnx_purge_request(uint64_t _request_id) override;
	
	void vnx_callback_switch(uint64_t _request_id, std::shared_ptr<const vnx::Binary> _data) override;
	
private:
	std::map<uint64_t, std::function<void(::std::shared_ptr<const ::vnx::search::DomainIndex>)>> vnx_queue_get_domain_info;
	std::map<uint64_t, std::function<void()>> vnx_queue_handle_vnx_keyvalue_KeyValuePair;
	std::map<uint64_t, std::function<void()>> vnx_queue_handle_vnx_keyvalue_SyncInfo;
	std::map<uint64_t, std::function<void(::std::shared_ptr<const ::vnx::search::SearchResult>)>> vnx_queue_query;
	std::map<uint64_t, std::function<void(::std::vector<::std::string>)>> vnx_queue_reverse_lookup;
	std::map<uint64_t, std::function<void(::std::vector<::std::string>)>> vnx_queue_suggest_domains;
	std::map<uint64_t, std::function<void(::std::vector<::std::string>)>> vnx_queue_suggest_words;
	
};


} // namespace vnx
} // namespace search

#endif // INCLUDE_vnx_search_SearchEngine_ASYNC_CLIENT_HXX_
