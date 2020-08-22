
// AUTO GENERATED by vnxcppcodegen

#ifndef INCLUDE_vnx_search_ArchiveServer_ASYNC_CLIENT_HXX_
#define INCLUDE_vnx_search_ArchiveServer_ASYNC_CLIENT_HXX_

#include <vnx/AsyncClient.h>
#include <vnx/Module.h>
#include <vnx/TopicPtr.hpp>
#include <vnx/keyvalue/SyncUpdate.hxx>


namespace vnx {
namespace search {

class ArchiveServerAsyncClient : public vnx::AsyncClient {
public:
	ArchiveServerAsyncClient(const std::string& service_name);
	
	ArchiveServerAsyncClient(vnx::Hash64 service_addr);
	
	uint64_t vnx_get_type_code(
			const std::function<void(::vnx::TypeCode)>& _callback = std::function<void(::vnx::TypeCode)>(),
			const std::function<void(const std::exception&)>& _error_callback = std::function<void(const std::exception&)>());
	
	std::vector<uint64_t> vnx_get_pending_ids() const override;
	
protected:
	void vnx_purge_request(uint64_t _request_id, const std::exception& _ex) override;
	
	void vnx_callback_switch(uint64_t _request_id, std::shared_ptr<const vnx::Value> _value) override;
	
private:
	std::map<uint64_t, std::pair<std::function<void(::vnx::TypeCode)>, std::function<void(const std::exception&)>>> vnx_queue_vnx_get_type_code;
	
};


} // namespace vnx
} // namespace search

#endif // INCLUDE_vnx_search_ArchiveServer_ASYNC_CLIENT_HXX_