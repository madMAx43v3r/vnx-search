
// AUTO GENERATED by vnxcppcodegen

#ifndef INCLUDE_vnx_search_SearchEngine_CLIENT_HXX_
#define INCLUDE_vnx_search_SearchEngine_CLIENT_HXX_

#include <vnx/Client.h>
#include <vnx/Module.h>
#include <vnx/TopicPtr.h>
#include <vnx/keyvalue/KeyValuePair.hxx>
#include <vnx/keyvalue/SyncInfo.hxx>
#include <vnx/search/DomainIndex.hxx>
#include <vnx/search/SearchResult.hxx>
#include <vnx/search/search_flags_e.hxx>


namespace vnx {
namespace search {

class SearchEngineClient : public vnx::Client {
public:
	SearchEngineClient(const std::string& service_name);
	
	SearchEngineClient(vnx::Hash64 service_addr);
	
	::std::shared_ptr<const ::vnx::search::DomainIndex> get_domain_info(const ::std::string& host, const ::int32_t& limit, const ::uint32_t& offset);
	
	void handle(const ::std::shared_ptr<const ::vnx::keyvalue::KeyValuePair>& sample);
	
	void handle_async(const ::std::shared_ptr<const ::vnx::keyvalue::KeyValuePair>& sample);
	
	void handle(const ::std::shared_ptr<const ::vnx::keyvalue::SyncInfo>& sample);
	
	void handle_async(const ::std::shared_ptr<const ::vnx::keyvalue::SyncInfo>& sample);
	
	::std::shared_ptr<const ::vnx::search::SearchResult> query(const ::std::vector<::std::string>& words, const ::int32_t& limit, const ::uint32_t& offset, const ::std::vector<::vnx::search::search_flags_e>& flags);
	
	::std::vector<::std::string> reverse_lookup(const ::std::string& url_key);
	
	::std::vector<::std::string> suggest_domains(const ::std::string& prefix, const ::int32_t& limit);
	
	::std::vector<::std::string> suggest_words(const ::std::string& prefix, const ::int32_t& limit);
	
};


} // namespace vnx
} // namespace search

#endif // INCLUDE_vnx_search_SearchEngine_CLIENT_HXX_
