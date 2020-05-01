
// AUTO GENERATED by vnxcppcodegen

#ifndef INCLUDE_vnx_search_CrawlProcessorBase_HXX_
#define INCLUDE_vnx_search_CrawlProcessorBase_HXX_

#include <vnx/search/package.hxx>
#include <vnx/Module.h>
#include <vnx/TopicPtr.h>
#include <vnx/keyvalue/KeyValuePair.hxx>
#include <vnx/search/CrawlStats.hxx>
#include <vnx/search/TextResponse.hxx>


namespace vnx {
namespace search {

class CrawlProcessorBase : public ::vnx::Module {
public:
	
	::vnx::TopicPtr input_text = "frontend.text_responses";
	::vnx::TopicPtr input_url_index = "backend.url_index.updates";
	::vnx::TopicPtr output_crawl_stats = "backend.crawl_stats";
	::std::string url_index_server = "UrlIndex";
	::std::string page_index_server = "PageIndex";
	::std::string page_content_server = "PageContent";
	::std::string crawl_frontend_server = "CrawlFrontend";
	::int32_t jump_cost = 3;
	::int32_t max_depth = 5;
	::int32_t reload_interval = 10000;
	::int32_t error_reload_interval = 100000;
	::int32_t sync_interval = 10000;
	::int32_t max_per_minute = 12;
	::int32_t max_num_pending = 100;
	::int32_t max_queue_size = 1000;
	::int32_t max_url_length = 256;
	::int32_t max_word_length = 64;
	::int32_t check_interval_ms = 500;
	::int32_t update_interval_ms = 5000;
	::int32_t robots_txt_timeout = 1000;
	::vnx::float32_t reload_power = 4;
	::uint32_t index_version = 0;
	::vnx::bool_t do_reprocess = false;
	::std::string user_agent = "Googlebot";
	::std::string profile = "default";
	::std::vector<::std::string> protocols;
	::std::vector<::std::string> root_urls;
	::std::vector<::std::string> domain_blacklist;
	
	typedef ::vnx::Module Super;
	
	static const vnx::Hash64 VNX_TYPE_HASH;
	static const vnx::Hash64 VNX_CODE_HASH;
	
	CrawlProcessorBase(const std::string& _vnx_name);
	
	vnx::Hash64 get_type_hash() const;
	const char* get_type_name() const;
	const vnx::TypeCode* get_type_code() const;
	
	void read(std::istream& _in);
	void write(std::ostream& _out) const;
	
	void accept(vnx::Visitor& _visitor) const;
	
	vnx::Object to_object() const;
	void from_object(const vnx::Object& object);
	
	friend std::ostream& operator<<(std::ostream& _out, const CrawlProcessorBase& _value);
	friend std::istream& operator>>(std::istream& _in, CrawlProcessorBase& _value);
	
	static const vnx::TypeCode* static_get_type_code();
	static std::shared_ptr<vnx::TypeCode> static_create_type_code();
	
protected:
	virtual ::std::shared_ptr<const ::vnx::search::CrawlStats> get_stats(const ::int32_t& limit) const = 0;
	virtual void handle(std::shared_ptr<const ::vnx::keyvalue::KeyValuePair> _value, std::shared_ptr<const ::vnx::Sample> _sample) { handle(_value); }
	virtual void handle(std::shared_ptr<const ::vnx::keyvalue::KeyValuePair> _value) {}
	virtual void handle(std::shared_ptr<const ::vnx::search::TextResponse> _value, std::shared_ptr<const ::vnx::Sample> _sample) { handle(_value); }
	virtual void handle(std::shared_ptr<const ::vnx::search::TextResponse> _value) {}
	
	void vnx_handle_switch(std::shared_ptr<const ::vnx::Sample> _sample) override;
	std::shared_ptr<vnx::Value> vnx_call_switch(vnx::TypeInput& _in, const vnx::TypeCode* _call_type, const vnx::request_id_t& _request_id) override;
	
private:
	
};


} // namespace vnx
} // namespace search

#endif // INCLUDE_vnx_search_CrawlProcessorBase_HXX_
