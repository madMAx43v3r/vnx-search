
// AUTO GENERATED by vnxcppcodegen

#ifndef INCLUDE_vnx_search_SearchEngine_ASYNC_CLIENT_HXX_
#define INCLUDE_vnx_search_SearchEngine_ASYNC_CLIENT_HXX_

#include <vnx/AsyncClient.h>
#include <vnx/Module.h>
#include <vnx/TopicPtr.h>
#include <vnx/keyvalue/KeyValuePair.hxx>
#include <vnx/keyvalue/SyncInfo.hxx>
#include <vnx/search/SearchResult.hxx>


namespace vnx {
namespace search {

class SearchEngineAsyncClient : public vnx::AsyncClient {
public:
	SearchEngineAsyncClient(const std::string& service_name);
	
	SearchEngineAsyncClient(vnx::Hash64 service_addr);
	
	uint64_t handle(const ::std::shared_ptr<const ::vnx::keyvalue::KeyValuePair>& sample, 
			const std::function<void()>& _callback = std::function<void()>());
	
	uint64_t handle(const ::std::shared_ptr<const ::vnx::keyvalue::SyncInfo>& sample, 
			const std::function<void()>& _callback = std::function<void()>());
	
	uint64_t query(const ::std::vector<::std::string>& words, const ::int64_t& max_results, 
			const std::function<void(::std::shared_ptr<const ::vnx::search::SearchResult>)>& _callback = std::function<void(::std::shared_ptr<const ::vnx::search::SearchResult>)>());
	
	std::vector<uint64_t> vnx_get_pending_ids() const override;
	
protected:
	void vnx_purge_request(uint64_t _request_id) override;
	
	void vnx_callback_switch(uint64_t _request_id, std::shared_ptr<const vnx::Binary> _data) override;
	
private:
	std::map<uint64_t, std::function<void()>> vnx_queue_handle_vnx_keyvalue_KeyValuePair;
	std::map<uint64_t, std::function<void()>> vnx_queue_handle_vnx_keyvalue_SyncInfo;
	std::map<uint64_t, std::function<void(::std::shared_ptr<const ::vnx::search::SearchResult>)>> vnx_queue_query;
	
};


} // namespace vnx
} // namespace search

#endif // INCLUDE_vnx_search_SearchEngine_ASYNC_CLIENT_HXX_
