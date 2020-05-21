
// AUTO GENERATED by vnxcppcodegen

#ifndef INCLUDE_vnx_search_CrawlFrontend_ASYNC_CLIENT_HXX_
#define INCLUDE_vnx_search_CrawlFrontend_ASYNC_CLIENT_HXX_

#include <vnx/AsyncClient.h>
#include <vnx/Hash64.h>
#include <vnx/Module.h>
#include <vnx/TopicPtr.h>
#include <vnx/search/HttpResponse.hxx>
#include <vnx/search/UrlIndex.hxx>


namespace vnx {
namespace search {

class CrawlFrontendAsyncClient : public vnx::AsyncClient {
public:
	CrawlFrontendAsyncClient(const std::string& service_name);
	
	CrawlFrontendAsyncClient(vnx::Hash64 service_addr);
	
	uint64_t fetch(const ::std::string& url, 
			const std::function<void(::std::shared_ptr<const ::vnx::search::UrlIndex>)>& _callback = std::function<void(::std::shared_ptr<const ::vnx::search::UrlIndex>)>());
	
	uint64_t handle(const ::std::shared_ptr<const ::vnx::search::HttpResponse>& sample, 
			const std::function<void()>& _callback = std::function<void()>());
	
	uint64_t register_parser(const ::vnx::Hash64& address, const ::std::vector<::std::string>& mime_types, const ::int32_t& num_threads, 
			const std::function<void()>& _callback = std::function<void()>());
	
	std::vector<uint64_t> vnx_get_pending_ids() const override;
	
protected:
	void vnx_purge_request(uint64_t _request_id) override;
	
	void vnx_callback_switch(uint64_t _request_id, std::shared_ptr<const vnx::Binary> _data) override;
	
private:
	std::map<uint64_t, std::function<void(::std::shared_ptr<const ::vnx::search::UrlIndex>)>> vnx_queue_fetch;
	std::map<uint64_t, std::function<void()>> vnx_queue_handle_vnx_search_HttpResponse;
	std::map<uint64_t, std::function<void()>> vnx_queue_register_parser;
	
};


} // namespace vnx
} // namespace search

#endif // INCLUDE_vnx_search_CrawlFrontend_ASYNC_CLIENT_HXX_
