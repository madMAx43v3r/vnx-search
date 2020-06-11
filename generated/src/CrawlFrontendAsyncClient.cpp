
// AUTO GENERATED by vnxcppcodegen

#include <vnx/vnx.h>
#include <vnx/search/package.hxx>
#include <vnx/search/CrawlFrontendAsyncClient.hxx>
#include <vnx/Hash64.hpp>
#include <vnx/Module.h>
#include <vnx/TopicPtr.hpp>
#include <vnx/search/CrawlFrontend_fetch.hxx>
#include <vnx/search/CrawlFrontend_fetch_return.hxx>
#include <vnx/search/CrawlFrontend_register_parser.hxx>
#include <vnx/search/CrawlFrontend_register_parser_return.hxx>
#include <vnx/search/FetchResult.hxx>



namespace vnx {
namespace search {

CrawlFrontendAsyncClient::CrawlFrontendAsyncClient(const std::string& service_name)
	:	AsyncClient::AsyncClient(vnx::Hash64(service_name))
{
}

CrawlFrontendAsyncClient::CrawlFrontendAsyncClient(vnx::Hash64 service_addr)
	:	AsyncClient::AsyncClient(service_addr)
{
}

uint64_t CrawlFrontendAsyncClient::fetch(const std::string& url, const std::function<void(std::shared_ptr<const ::vnx::search::FetchResult>)>& _callback, const std::function<void(const std::exception&)>& _error_callback) {
	auto _method = ::vnx::search::CrawlFrontend_fetch::create();
	_method->url = url;
	const auto _request_id = vnx_request(_method);
	vnx_queue_fetch[_request_id] = std::make_pair(_callback, _error_callback);
	vnx_num_pending++;
	return _request_id;
}

uint64_t CrawlFrontendAsyncClient::register_parser(const ::vnx::Hash64& address, const std::vector<std::string>& mime_types, const int32_t& num_threads, const std::function<void()>& _callback, const std::function<void(const std::exception&)>& _error_callback) {
	auto _method = ::vnx::search::CrawlFrontend_register_parser::create();
	_method->address = address;
	_method->mime_types = mime_types;
	_method->num_threads = num_threads;
	const auto _request_id = vnx_request(_method);
	vnx_queue_register_parser[_request_id] = std::make_pair(_callback, _error_callback);
	vnx_num_pending++;
	return _request_id;
}

std::vector<uint64_t> CrawlFrontendAsyncClient::vnx_get_pending_ids() const {
	std::vector<uint64_t> _list;
	for(const auto& entry : vnx_queue_fetch) {
		_list.push_back(entry.first);
	}
	for(const auto& entry : vnx_queue_register_parser) {
		_list.push_back(entry.first);
	}
	return _list;
}

void CrawlFrontendAsyncClient::vnx_purge_request(uint64_t _request_id, const std::exception& _ex) {
	{
		const auto _iter = vnx_queue_fetch.find(_request_id);
		if(_iter != vnx_queue_fetch.end()) {
			if(_iter->second.second) {
				_iter->second.second(_ex);
			}
			vnx_queue_fetch.erase(_iter);
			vnx_num_pending--;
		}
	}
	{
		const auto _iter = vnx_queue_register_parser.find(_request_id);
		if(_iter != vnx_queue_register_parser.end()) {
			if(_iter->second.second) {
				_iter->second.second(_ex);
			}
			vnx_queue_register_parser.erase(_iter);
			vnx_num_pending--;
		}
	}
}

void CrawlFrontendAsyncClient::vnx_callback_switch(uint64_t _request_id, std::shared_ptr<const vnx::Value> _value) {
	const auto _type_hash = _value->get_type_hash();
	if(_type_hash == vnx::Hash64(0xce1d8d53217a282eull)) {
		auto _result = std::dynamic_pointer_cast<const ::vnx::search::CrawlFrontend_fetch_return>(_value);
		if(!_result) {
			throw std::logic_error("CrawlFrontendAsyncClient: !_result");
		}
		const auto _iter = vnx_queue_fetch.find(_request_id);
		if(_iter != vnx_queue_fetch.end()) {
			const auto _callback = std::move(_iter->second.first);
			vnx_queue_fetch.erase(_iter);
			vnx_num_pending--;
			if(_callback) {
				_callback(_result->_ret_0);
			}
		} else {
			throw std::runtime_error("CrawlFrontendAsyncClient: invalid return received");
		}
	}
	else if(_type_hash == vnx::Hash64(0x8ced9de5e282cceaull)) {
		const auto _iter = vnx_queue_register_parser.find(_request_id);
		if(_iter != vnx_queue_register_parser.end()) {
			const auto _callback = std::move(_iter->second.first);
			vnx_queue_register_parser.erase(_iter);
			vnx_num_pending--;
			if(_callback) {
				_callback();
			}
		} else {
			throw std::runtime_error("CrawlFrontendAsyncClient: invalid return received");
		}
	}
	else {
		throw std::runtime_error("CrawlFrontendAsyncClient: unknown return type");
	}
}


} // namespace vnx
} // namespace search
