
// AUTO GENERATED by vnxcppcodegen

#include <vnx/search/package.hxx>
#include <vnx/search/QueryInterfaceAsyncClient.hxx>
#include <vnx/search/QueryInterface_query.hxx>
#include <vnx/search/QueryInterface_query_return.hxx>
#include <vnx/search/SearchResult.hxx>
#include <vnx/search/query_options_t.hxx>

#include <vnx/vnx.h>


namespace vnx {
namespace search {

QueryInterfaceAsyncClient::QueryInterfaceAsyncClient(const std::string& service_name)
	:	AsyncClient::AsyncClient(vnx::Hash64(service_name))
{
}

QueryInterfaceAsyncClient::QueryInterfaceAsyncClient(vnx::Hash64 service_addr)
	:	AsyncClient::AsyncClient(service_addr)
{
}

uint64_t QueryInterfaceAsyncClient::query(const std::vector<std::string>& words, const ::vnx::search::query_options_t& options, const std::function<void(std::shared_ptr<const ::vnx::search::SearchResult>)>& _callback, const std::function<void(const std::exception&)>& _error_callback) {
	auto _method = ::vnx::search::QueryInterface_query::create();
	_method->words = words;
	_method->options = options;
	const auto _request_id = vnx_request(_method);
	vnx_queue_query[_request_id] = std::make_pair(_callback, _error_callback);
	vnx_num_pending++;
	return _request_id;
}

std::vector<uint64_t> QueryInterfaceAsyncClient::vnx_get_pending_ids() const {
	std::vector<uint64_t> _list;
	for(const auto& entry : vnx_queue_query) {
		_list.push_back(entry.first);
	}
	return _list;
}

void QueryInterfaceAsyncClient::vnx_purge_request(uint64_t _request_id, const std::exception& _ex) {
	{
		const auto _iter = vnx_queue_query.find(_request_id);
		if(_iter != vnx_queue_query.end()) {
			if(_iter->second.second) {
				_iter->second.second(_ex);
			}
			vnx_queue_query.erase(_iter);
			vnx_num_pending--;
		}
	}
}

void QueryInterfaceAsyncClient::vnx_callback_switch(uint64_t _request_id, std::shared_ptr<const vnx::Value> _value) {
	const auto _type_hash = _value->get_type_hash();
	if(_type_hash == vnx::Hash64(0x9132d7d126fc55a6ull)) {
		auto _result = std::dynamic_pointer_cast<const ::vnx::search::QueryInterface_query_return>(_value);
		if(!_result) {
			throw std::logic_error("QueryInterfaceAsyncClient: !_result");
		}
		const auto _iter = vnx_queue_query.find(_request_id);
		if(_iter != vnx_queue_query.end()) {
			const auto _callback = std::move(_iter->second.first);
			vnx_queue_query.erase(_iter);
			vnx_num_pending--;
			if(_callback) {
				_callback(_result->_ret_0);
			}
		} else {
			throw std::runtime_error("QueryInterfaceAsyncClient: invalid return received");
		}
	}
	else {
		throw std::runtime_error("QueryInterfaceAsyncClient: unknown return type");
	}
}


} // namespace vnx
} // namespace search