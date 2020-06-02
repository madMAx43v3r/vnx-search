
// AUTO GENERATED by vnxcppcodegen

#include <vnx/search/package.hxx>
#include <vnx/search/SearchEngineAsyncClient.hxx>
#include <vnx/Input.h>
#include <vnx/Output.h>
#include <vnx/Module.h>
#include <vnx/Object.h>
#include <vnx/TopicPtr.h>
#include <vnx/keyvalue/KeyValuePair.hxx>
#include <vnx/keyvalue/SyncInfo.hxx>
#include <vnx/search/SearchEngine_get_domain_info.hxx>
#include <vnx/search/SearchEngine_get_domain_info_return.hxx>
#include <vnx/search/SearchEngine_get_domain_list.hxx>
#include <vnx/search/SearchEngine_get_domain_list_return.hxx>
#include <vnx/search/SearchEngine_get_page_info.hxx>
#include <vnx/search/SearchEngine_get_page_info_return.hxx>
#include <vnx/search/SearchEngine_query.hxx>
#include <vnx/search/SearchEngine_query_return.hxx>
#include <vnx/search/SearchEngine_reverse_domain_lookup.hxx>
#include <vnx/search/SearchEngine_reverse_domain_lookup_return.hxx>
#include <vnx/search/SearchEngine_reverse_lookup.hxx>
#include <vnx/search/SearchEngine_reverse_lookup_return.hxx>
#include <vnx/search/SearchEngine_suggest_domains.hxx>
#include <vnx/search/SearchEngine_suggest_domains_return.hxx>
#include <vnx/search/SearchEngine_suggest_words.hxx>
#include <vnx/search/SearchEngine_suggest_words_return.hxx>
#include <vnx/search/SearchResult.hxx>
#include <vnx/search/search_flags_e.hxx>



namespace vnx {
namespace search {

SearchEngineAsyncClient::SearchEngineAsyncClient(const std::string& service_name)
	:	AsyncClient::AsyncClient(vnx::Hash64(service_name))
{
}

SearchEngineAsyncClient::SearchEngineAsyncClient(vnx::Hash64 service_addr)
	:	AsyncClient::AsyncClient(service_addr)
{
}

uint64_t SearchEngineAsyncClient::get_domain_info(const std::string& host, const int32_t& limit, const uint32_t& offset, const std::function<void(::vnx::Object)>& _callback) {
	auto _method = ::vnx::search::SearchEngine_get_domain_info::create();
	_method->host = host;
	_method->limit = limit;
	_method->offset = offset;
	const auto _request_id = vnx_request(_method);
	vnx_queue_get_domain_info[_request_id] = _callback;
	vnx_num_pending++;
	return _request_id;
}

uint64_t SearchEngineAsyncClient::get_domain_list(const int32_t& limit, const uint32_t& offset, const std::function<void(std::vector<::vnx::Object>)>& _callback) {
	auto _method = ::vnx::search::SearchEngine_get_domain_list::create();
	_method->limit = limit;
	_method->offset = offset;
	const auto _request_id = vnx_request(_method);
	vnx_queue_get_domain_list[_request_id] = _callback;
	vnx_num_pending++;
	return _request_id;
}

uint64_t SearchEngineAsyncClient::get_page_info(const std::string& url_key, const std::function<void(::vnx::Object)>& _callback) {
	auto _method = ::vnx::search::SearchEngine_get_page_info::create();
	_method->url_key = url_key;
	const auto _request_id = vnx_request(_method);
	vnx_queue_get_page_info[_request_id] = _callback;
	vnx_num_pending++;
	return _request_id;
}

uint64_t SearchEngineAsyncClient::query(const std::vector<std::string>& words, const int32_t& limit, const uint32_t& offset, const std::vector<::vnx::search::search_flags_e>& flags, const std::function<void(std::shared_ptr<const ::vnx::search::SearchResult>)>& _callback) {
	auto _method = ::vnx::search::SearchEngine_query::create();
	_method->words = words;
	_method->limit = limit;
	_method->offset = offset;
	_method->flags = flags;
	const auto _request_id = vnx_request(_method);
	vnx_queue_query[_request_id] = _callback;
	vnx_num_pending++;
	return _request_id;
}

uint64_t SearchEngineAsyncClient::reverse_domain_lookup(const std::string& url_key, const std::function<void(std::vector<std::string>)>& _callback) {
	auto _method = ::vnx::search::SearchEngine_reverse_domain_lookup::create();
	_method->url_key = url_key;
	const auto _request_id = vnx_request(_method);
	vnx_queue_reverse_domain_lookup[_request_id] = _callback;
	vnx_num_pending++;
	return _request_id;
}

uint64_t SearchEngineAsyncClient::reverse_lookup(const std::string& url_key, const std::function<void(std::vector<std::string>)>& _callback) {
	auto _method = ::vnx::search::SearchEngine_reverse_lookup::create();
	_method->url_key = url_key;
	const auto _request_id = vnx_request(_method);
	vnx_queue_reverse_lookup[_request_id] = _callback;
	vnx_num_pending++;
	return _request_id;
}

uint64_t SearchEngineAsyncClient::suggest_domains(const std::string& prefix, const int32_t& limit, const std::function<void(std::vector<std::string>)>& _callback) {
	auto _method = ::vnx::search::SearchEngine_suggest_domains::create();
	_method->prefix = prefix;
	_method->limit = limit;
	const auto _request_id = vnx_request(_method);
	vnx_queue_suggest_domains[_request_id] = _callback;
	vnx_num_pending++;
	return _request_id;
}

uint64_t SearchEngineAsyncClient::suggest_words(const std::string& prefix, const int32_t& limit, const std::function<void(std::vector<std::string>)>& _callback) {
	auto _method = ::vnx::search::SearchEngine_suggest_words::create();
	_method->prefix = prefix;
	_method->limit = limit;
	const auto _request_id = vnx_request(_method);
	vnx_queue_suggest_words[_request_id] = _callback;
	vnx_num_pending++;
	return _request_id;
}

std::vector<uint64_t> SearchEngineAsyncClient::vnx_get_pending_ids() const {
	std::vector<uint64_t> _list;
	for(const auto& entry : vnx_queue_get_domain_info) {
		_list.push_back(entry.first);
	}
	for(const auto& entry : vnx_queue_get_domain_list) {
		_list.push_back(entry.first);
	}
	for(const auto& entry : vnx_queue_get_page_info) {
		_list.push_back(entry.first);
	}
	for(const auto& entry : vnx_queue_query) {
		_list.push_back(entry.first);
	}
	for(const auto& entry : vnx_queue_reverse_domain_lookup) {
		_list.push_back(entry.first);
	}
	for(const auto& entry : vnx_queue_reverse_lookup) {
		_list.push_back(entry.first);
	}
	for(const auto& entry : vnx_queue_suggest_domains) {
		_list.push_back(entry.first);
	}
	for(const auto& entry : vnx_queue_suggest_words) {
		_list.push_back(entry.first);
	}
	return _list;
}

void SearchEngineAsyncClient::vnx_purge_request(uint64_t _request_id) {
	vnx_num_pending -= vnx_queue_get_domain_info.erase(_request_id);
	vnx_num_pending -= vnx_queue_get_domain_list.erase(_request_id);
	vnx_num_pending -= vnx_queue_get_page_info.erase(_request_id);
	vnx_num_pending -= vnx_queue_query.erase(_request_id);
	vnx_num_pending -= vnx_queue_reverse_domain_lookup.erase(_request_id);
	vnx_num_pending -= vnx_queue_reverse_lookup.erase(_request_id);
	vnx_num_pending -= vnx_queue_suggest_domains.erase(_request_id);
	vnx_num_pending -= vnx_queue_suggest_words.erase(_request_id);
}

void SearchEngineAsyncClient::vnx_callback_switch(uint64_t _request_id, std::shared_ptr<const vnx::Value> _value) {
	const auto _type_hash = _value->get_type_hash();
	if(_type_hash == vnx::Hash64(0xdd84ef2259be7eull)) {
		auto _result = std::dynamic_pointer_cast<const ::vnx::search::SearchEngine_get_domain_info_return>(_value);
		if(!_result) {
			throw std::logic_error("AsyncClient: !_result");
		}
		const auto _iter = vnx_queue_get_domain_info.find(_request_id);
		if(_iter != vnx_queue_get_domain_info.end()) {
			const auto _callback = std::move(_iter->second);
			vnx_queue_get_domain_info.erase(_iter);
			vnx_num_pending--;
			if(_callback) {
				_callback(_result->_ret_0);
			}
		}
	}
	else if(_type_hash == vnx::Hash64(0xa06392efe70f0679ull)) {
		auto _result = std::dynamic_pointer_cast<const ::vnx::search::SearchEngine_get_domain_list_return>(_value);
		if(!_result) {
			throw std::logic_error("AsyncClient: !_result");
		}
		const auto _iter = vnx_queue_get_domain_list.find(_request_id);
		if(_iter != vnx_queue_get_domain_list.end()) {
			const auto _callback = std::move(_iter->second);
			vnx_queue_get_domain_list.erase(_iter);
			vnx_num_pending--;
			if(_callback) {
				_callback(_result->_ret_0);
			}
		}
	}
	else if(_type_hash == vnx::Hash64(0xa79acda144d4c33bull)) {
		auto _result = std::dynamic_pointer_cast<const ::vnx::search::SearchEngine_get_page_info_return>(_value);
		if(!_result) {
			throw std::logic_error("AsyncClient: !_result");
		}
		const auto _iter = vnx_queue_get_page_info.find(_request_id);
		if(_iter != vnx_queue_get_page_info.end()) {
			const auto _callback = std::move(_iter->second);
			vnx_queue_get_page_info.erase(_iter);
			vnx_num_pending--;
			if(_callback) {
				_callback(_result->_ret_0);
			}
		}
	}
	else if(_type_hash == vnx::Hash64(0xc4adbd01e9504960ull)) {
		auto _result = std::dynamic_pointer_cast<const ::vnx::search::SearchEngine_query_return>(_value);
		if(!_result) {
			throw std::logic_error("AsyncClient: !_result");
		}
		const auto _iter = vnx_queue_query.find(_request_id);
		if(_iter != vnx_queue_query.end()) {
			const auto _callback = std::move(_iter->second);
			vnx_queue_query.erase(_iter);
			vnx_num_pending--;
			if(_callback) {
				_callback(_result->_ret_0);
			}
		}
	}
	else if(_type_hash == vnx::Hash64(0x8f0160842cd83465ull)) {
		auto _result = std::dynamic_pointer_cast<const ::vnx::search::SearchEngine_reverse_domain_lookup_return>(_value);
		if(!_result) {
			throw std::logic_error("AsyncClient: !_result");
		}
		const auto _iter = vnx_queue_reverse_domain_lookup.find(_request_id);
		if(_iter != vnx_queue_reverse_domain_lookup.end()) {
			const auto _callback = std::move(_iter->second);
			vnx_queue_reverse_domain_lookup.erase(_iter);
			vnx_num_pending--;
			if(_callback) {
				_callback(_result->_ret_0);
			}
		}
	}
	else if(_type_hash == vnx::Hash64(0x12b287ac93bf5253ull)) {
		auto _result = std::dynamic_pointer_cast<const ::vnx::search::SearchEngine_reverse_lookup_return>(_value);
		if(!_result) {
			throw std::logic_error("AsyncClient: !_result");
		}
		const auto _iter = vnx_queue_reverse_lookup.find(_request_id);
		if(_iter != vnx_queue_reverse_lookup.end()) {
			const auto _callback = std::move(_iter->second);
			vnx_queue_reverse_lookup.erase(_iter);
			vnx_num_pending--;
			if(_callback) {
				_callback(_result->_ret_0);
			}
		}
	}
	else if(_type_hash == vnx::Hash64(0x385aae88fe3df753ull)) {
		auto _result = std::dynamic_pointer_cast<const ::vnx::search::SearchEngine_suggest_domains_return>(_value);
		if(!_result) {
			throw std::logic_error("AsyncClient: !_result");
		}
		const auto _iter = vnx_queue_suggest_domains.find(_request_id);
		if(_iter != vnx_queue_suggest_domains.end()) {
			const auto _callback = std::move(_iter->second);
			vnx_queue_suggest_domains.erase(_iter);
			vnx_num_pending--;
			if(_callback) {
				_callback(_result->_ret_0);
			}
		}
	}
	else if(_type_hash == vnx::Hash64(0xcf74c35d3594cc90ull)) {
		auto _result = std::dynamic_pointer_cast<const ::vnx::search::SearchEngine_suggest_words_return>(_value);
		if(!_result) {
			throw std::logic_error("AsyncClient: !_result");
		}
		const auto _iter = vnx_queue_suggest_words.find(_request_id);
		if(_iter != vnx_queue_suggest_words.end()) {
			const auto _callback = std::move(_iter->second);
			vnx_queue_suggest_words.erase(_iter);
			vnx_num_pending--;
			if(_callback) {
				_callback(_result->_ret_0);
			}
		}
	}
	else {
		throw std::runtime_error("unknown return value");
	}
}


} // namespace vnx
} // namespace search
