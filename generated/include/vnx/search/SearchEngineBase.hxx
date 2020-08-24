
// AUTO GENERATED by vnxcppcodegen

#ifndef INCLUDE_vnx_search_SearchEngineBase_HXX_
#define INCLUDE_vnx_search_SearchEngineBase_HXX_

#include <vnx/search/package.hxx>
#include <vnx/Module.h>
#include <vnx/Object.hpp>
#include <vnx/TopicPtr.hpp>
#include <vnx/keyvalue/SyncInfo.hxx>
#include <vnx/keyvalue/SyncUpdate.hxx>
#include <vnx/search/SearchResult.hxx>
#include <vnx/search/query_options_t.hxx>


namespace vnx {
namespace search {

class SearchEngineBase : public ::vnx::Module {
public:
	
	::vnx::TopicPtr input_url_index = "backend.url_index.updates";
	::vnx::TopicPtr input_page_index = "backend.page_index.key_updates";
	::vnx::TopicPtr input_page_content = "backend.page_content.key_updates";
	std::string url_index_server = "UrlIndex";
	std::string page_index_server = "PageIndex";
	std::string page_content_server = "PageContent";
	int32_t max_link_cache = 500000;
	int32_t max_word_cache = 500000;
	int32_t max_num_pending = 100;
	int32_t num_query_threads = 4;
	int32_t num_update_threads = 4;
	int32_t commit_delay = 100;
	int32_t link_commit_interval = 60;
	int32_t word_commit_interval = 3600;
	int32_t lock_timeout = 100;
	int32_t queue_interval_ms = 10;
	int32_t stats_interval_ms = 10000;
	vnx::float32_t rank_decay = 0.5;
	vnx::bool_t update_word_array = 0;
	vnx::bool_t update_word_context = 0;
	vnx::bool_t update_page_info = 0;
	std::vector<std::string> protocols;
	
	typedef ::vnx::Module Super;
	
	static const vnx::Hash64 VNX_TYPE_HASH;
	static const vnx::Hash64 VNX_CODE_HASH;
	
	SearchEngineBase(const std::string& _vnx_name);
	
	vnx::Hash64 get_type_hash() const;
	const char* get_type_name() const;
	const vnx::TypeCode* get_type_code() const;
	
	void read(std::istream& _in);
	void write(std::ostream& _out) const;
	
	void accept(vnx::Visitor& _visitor) const;
	
	vnx::Object to_object() const;
	void from_object(const vnx::Object& object);
	
	vnx::Variant get_field(const std::string& name) const;
	void set_field(const std::string& name, const vnx::Variant& value);
	
	friend std::ostream& operator<<(std::ostream& _out, const SearchEngineBase& _value);
	friend std::istream& operator>>(std::istream& _in, SearchEngineBase& _value);
	
	static const vnx::TypeCode* static_get_type_code();
	static std::shared_ptr<vnx::TypeCode> static_create_type_code();
	
protected:
	virtual void query_async(const std::vector<std::string>& words, const ::vnx::search::query_options_t& options, const vnx::request_id_t& _request_id) const = 0;
	void query_async_return(const vnx::request_id_t& _request_id, const std::shared_ptr<const ::vnx::search::SearchResult>& _ret_0) const;
	virtual void get_domain_info_async(const std::string& host, const int32_t& limit, const uint32_t& offset, const vnx::request_id_t& _request_id) const = 0;
	void get_domain_info_async_return(const vnx::request_id_t& _request_id, const ::vnx::Object& _ret_0) const;
	virtual void get_page_info_async(const std::string& url_key, const vnx::request_id_t& _request_id) const = 0;
	void get_page_info_async_return(const vnx::request_id_t& _request_id, const ::vnx::Object& _ret_0) const;
	virtual void get_page_ranks_async(const std::vector<std::string>& url_keys, const vnx::request_id_t& _request_id) const = 0;
	void get_page_ranks_async_return(const vnx::request_id_t& _request_id, const std::vector<vnx::float32_t>& _ret_0) const;
	virtual std::vector<::vnx::Object> get_domain_list(const int32_t& limit, const uint32_t& offset) const = 0;
	virtual void reverse_lookup_async(const std::string& url_key, const vnx::request_id_t& _request_id) const = 0;
	void reverse_lookup_async_return(const vnx::request_id_t& _request_id, const std::vector<std::string>& _ret_0) const;
	virtual void reverse_domain_lookup_async(const std::string& url_key, const vnx::request_id_t& _request_id) const = 0;
	void reverse_domain_lookup_async_return(const vnx::request_id_t& _request_id, const std::vector<std::pair<std::string, uint32_t>>& _ret_0) const;
	virtual std::vector<std::string> suggest_words(const std::string& prefix, const int32_t& limit) const = 0;
	virtual std::vector<std::string> suggest_domains(const std::string& prefix, const int32_t& limit) const = 0;
	virtual void handle(std::shared_ptr<const ::vnx::keyvalue::SyncUpdate> _value, std::shared_ptr<const vnx::Sample> _sample) { handle(_value); }
	virtual void handle(std::shared_ptr<const ::vnx::keyvalue::SyncUpdate> _value) {}
	virtual void handle(std::shared_ptr<const ::vnx::keyvalue::SyncInfo> _value, std::shared_ptr<const vnx::Sample> _sample) { handle(_value); }
	virtual void handle(std::shared_ptr<const ::vnx::keyvalue::SyncInfo> _value) {}
	
	void vnx_handle_switch(std::shared_ptr<const vnx::Sample> _sample) override;
	std::shared_ptr<vnx::Value> vnx_call_switch(std::shared_ptr<const vnx::Value> _method, const vnx::request_id_t& _request_id) override;
	
};


} // namespace vnx
} // namespace search

#endif // INCLUDE_vnx_search_SearchEngineBase_HXX_
