
// AUTO GENERATED by vnxcppcodegen

#ifndef INCLUDE_vnx_search_SearchEngine_CLIENT_HXX_
#define INCLUDE_vnx_search_SearchEngine_CLIENT_HXX_

#include <vnx/Client.h>
#include <vnx/Module.h>
#include <vnx/Object.hpp>
#include <vnx/TopicPtr.hpp>
#include <vnx/keyvalue/SyncInfo.hxx>
#include <vnx/keyvalue/SyncUpdate.hxx>
#include <vnx/search/page_entry_t.hxx>


namespace vnx {
namespace search {

class SearchEngineClient : public vnx::Client {
public:
	SearchEngineClient(const std::string& service_name);
	
	SearchEngineClient(vnx::Hash64 service_addr);
	
	::vnx::TypeCode vnx_get_type_code();
	
	std::vector<::vnx::search::page_entry_t> get_page_entries(const std::vector<uint32_t>& page_ids);
	
	::vnx::Object get_domain_info(const std::string& host, const int32_t& limit, const uint32_t& offset);
	
	::vnx::Object get_page_info(const std::string& url_key);
	
	std::vector<vnx::float32_t> get_page_ranks(const std::vector<std::string>& url_keys, const vnx::bool_t& direct);
	
	std::vector<::vnx::Object> get_domain_list(const int32_t& limit, const uint32_t& offset);
	
	std::vector<std::string> reverse_lookup(const std::string& url_key);
	
	std::vector<std::pair<std::string, uint32_t>> reverse_domain_lookup(const std::string& url_key);
	
	std::vector<std::string> suggest_words(const std::string& prefix, const int32_t& limit);
	
	std::vector<std::string> suggest_domains(const std::string& prefix, const int32_t& limit);
	
};


} // namespace vnx
} // namespace search

#endif // INCLUDE_vnx_search_SearchEngine_CLIENT_HXX_
