
// AUTO GENERATED by vnxcppcodegen

#ifndef INCLUDE_vnx_search_CrawlProcessorBase_HXX_
#define INCLUDE_vnx_search_CrawlProcessorBase_HXX_

#include <vnx/search/package.hxx>
#include <vnx/Module.h>
#include <vnx/Object.hpp>
#include <vnx/TopicPtr.hpp>
#include <vnx/keyvalue/SyncUpdate.hxx>


namespace vnx {
namespace search {

class CrawlProcessorBase : public ::vnx::Module {
public:
	
	::vnx::TopicPtr input_url_index = "backend.url_index.updates";
	::vnx::TopicPtr output_crawl_stats = "crawl.stats";
	std::string url_index_server = "UrlIndex";
	std::string page_index_server = "PageIndex";
	std::string page_content_server = "PageContent";
	std::string crawl_frontend_server = "CrawlFrontend";
	int32_t jump_cost = 3;
	int32_t max_depth = 5;
	int32_t reload_interval = 10000;
	int32_t sync_interval = 10000;
	int32_t max_per_minute = 12;
	int32_t max_num_pending = 100;
	int32_t max_queue_size = 1000;
	int32_t max_url_length = 256;
	int32_t max_word_length = 64;
	int32_t num_threads = 4;
	int32_t check_interval_ms = 500;
	int32_t robots_timeout = 1000;
	int32_t robots_reload_interval = 2678400;
	int32_t commit_delay = 10;
	int32_t lock_timeout = 100;
	vnx::float32_t reload_power = 4;
	uint32_t index_version = 1;
	vnx::bool_t inititial_sync = false;
	std::string user_agent = "Googlebot";
	std::vector<std::string> protocols;
	std::vector<std::string> root_urls;
	std::vector<std::string> domain_blacklist;
	std::vector<std::string> path_blacklist;
	std::vector<std::string> regex_blacklist;
	
	typedef ::vnx::Module Super;
	
	static const vnx::Hash64 VNX_TYPE_HASH;
	static const vnx::Hash64 VNX_CODE_HASH;
	
	static constexpr uint64_t VNX_TYPE_ID = 0x508848d1f9d97d9full;
	
	CrawlProcessorBase(const std::string& _vnx_name);
	
	vnx::Hash64 get_type_hash() const override;
	std::string get_type_name() const override;
	const vnx::TypeCode* get_type_code() const override;
	
	void read(std::istream& _in) override;
	void write(std::ostream& _out) const override;
	
	void accept(vnx::Visitor& _visitor) const override;
	
	vnx::Object to_object() const override;
	void from_object(const vnx::Object& object) override;
	
	vnx::Variant get_field(const std::string& name) const override;
	void set_field(const std::string& name, const vnx::Variant& value) override;
	
	friend std::ostream& operator<<(std::ostream& _out, const CrawlProcessorBase& _value);
	friend std::istream& operator>>(std::istream& _in, CrawlProcessorBase& _value);
	
	static const vnx::TypeCode* static_get_type_code();
	static std::shared_ptr<vnx::TypeCode> static_create_type_code();
	
protected:
	using Super::handle;
	
	virtual ::vnx::Object get_stats(const int32_t& limit) const = 0;
	virtual void check_all_urls() = 0;
	virtual void check_root_urls() = 0;
	virtual void handle(std::shared_ptr<const ::vnx::keyvalue::SyncUpdate> _value) {}
	
	void vnx_handle_switch(std::shared_ptr<const vnx::Value> _value) override;
	std::shared_ptr<vnx::Value> vnx_call_switch(std::shared_ptr<const vnx::Value> _method, const vnx::request_id_t& _request_id) override;
	
};


} // namespace vnx
} // namespace search


namespace vnx {

} // vnx

#endif // INCLUDE_vnx_search_CrawlProcessorBase_HXX_
