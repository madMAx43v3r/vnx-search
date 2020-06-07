
// AUTO GENERATED by vnxcppcodegen

#include <vnx/vnx.h>
#include <vnx/search/package.hxx>
#include <vnx/search/SearchEngineClient.hxx>
#include <vnx/Module.h>
#include <vnx/Object.hpp>
#include <vnx/TopicPtr.hpp>
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

SearchEngineClient::SearchEngineClient(const std::string& service_name)
	:	Client::Client(vnx::Hash64(service_name))
{
}

SearchEngineClient::SearchEngineClient(vnx::Hash64 service_addr)
	:	Client::Client(service_addr)
{
}

::vnx::Object SearchEngineClient::get_domain_info(const std::string& host, const int32_t& limit, const uint32_t& offset) {
	auto _method = ::vnx::search::SearchEngine_get_domain_info::create();
	_method->host = host;
	_method->limit = limit;
	_method->offset = offset;
	auto _return_value = vnx_request(_method);
	auto _result = std::dynamic_pointer_cast<const ::vnx::search::SearchEngine_get_domain_info_return>(_return_value);
	if(!_result) {
		throw std::logic_error("Client: !_result");
	}
	return _result->_ret_0;
}

std::vector<::vnx::Object> SearchEngineClient::get_domain_list(const int32_t& limit, const uint32_t& offset) {
	auto _method = ::vnx::search::SearchEngine_get_domain_list::create();
	_method->limit = limit;
	_method->offset = offset;
	auto _return_value = vnx_request(_method);
	auto _result = std::dynamic_pointer_cast<const ::vnx::search::SearchEngine_get_domain_list_return>(_return_value);
	if(!_result) {
		throw std::logic_error("Client: !_result");
	}
	return _result->_ret_0;
}

::vnx::Object SearchEngineClient::get_page_info(const std::string& url_key) {
	auto _method = ::vnx::search::SearchEngine_get_page_info::create();
	_method->url_key = url_key;
	auto _return_value = vnx_request(_method);
	auto _result = std::dynamic_pointer_cast<const ::vnx::search::SearchEngine_get_page_info_return>(_return_value);
	if(!_result) {
		throw std::logic_error("Client: !_result");
	}
	return _result->_ret_0;
}

std::shared_ptr<const ::vnx::search::SearchResult> SearchEngineClient::query(const std::vector<std::string>& words, const int32_t& limit, const uint32_t& offset, const std::vector<::vnx::search::search_flags_e>& flags) {
	auto _method = ::vnx::search::SearchEngine_query::create();
	_method->words = words;
	_method->limit = limit;
	_method->offset = offset;
	_method->flags = flags;
	auto _return_value = vnx_request(_method);
	auto _result = std::dynamic_pointer_cast<const ::vnx::search::SearchEngine_query_return>(_return_value);
	if(!_result) {
		throw std::logic_error("Client: !_result");
	}
	return _result->_ret_0;
}

std::vector<std::string> SearchEngineClient::reverse_domain_lookup(const std::string& url_key) {
	auto _method = ::vnx::search::SearchEngine_reverse_domain_lookup::create();
	_method->url_key = url_key;
	auto _return_value = vnx_request(_method);
	auto _result = std::dynamic_pointer_cast<const ::vnx::search::SearchEngine_reverse_domain_lookup_return>(_return_value);
	if(!_result) {
		throw std::logic_error("Client: !_result");
	}
	return _result->_ret_0;
}

std::vector<std::string> SearchEngineClient::reverse_lookup(const std::string& url_key) {
	auto _method = ::vnx::search::SearchEngine_reverse_lookup::create();
	_method->url_key = url_key;
	auto _return_value = vnx_request(_method);
	auto _result = std::dynamic_pointer_cast<const ::vnx::search::SearchEngine_reverse_lookup_return>(_return_value);
	if(!_result) {
		throw std::logic_error("Client: !_result");
	}
	return _result->_ret_0;
}

std::vector<std::string> SearchEngineClient::suggest_domains(const std::string& prefix, const int32_t& limit) {
	auto _method = ::vnx::search::SearchEngine_suggest_domains::create();
	_method->prefix = prefix;
	_method->limit = limit;
	auto _return_value = vnx_request(_method);
	auto _result = std::dynamic_pointer_cast<const ::vnx::search::SearchEngine_suggest_domains_return>(_return_value);
	if(!_result) {
		throw std::logic_error("Client: !_result");
	}
	return _result->_ret_0;
}

std::vector<std::string> SearchEngineClient::suggest_words(const std::string& prefix, const int32_t& limit) {
	auto _method = ::vnx::search::SearchEngine_suggest_words::create();
	_method->prefix = prefix;
	_method->limit = limit;
	auto _return_value = vnx_request(_method);
	auto _result = std::dynamic_pointer_cast<const ::vnx::search::SearchEngine_suggest_words_return>(_return_value);
	if(!_result) {
		throw std::logic_error("Client: !_result");
	}
	return _result->_ret_0;
}


} // namespace vnx
} // namespace search
