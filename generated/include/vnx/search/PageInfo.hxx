
// AUTO GENERATED by vnxcppcodegen

#ifndef INCLUDE_vnx_search_PageInfo_HXX_
#define INCLUDE_vnx_search_PageInfo_HXX_

#include <vnx/search/package.hxx>
#include <vnx/Value.h>
#include <vnx/search/reverse_link_t.hxx>


namespace vnx {
namespace search {

class PageInfo : public ::vnx::Value {
public:
	
	uint32_t id = 0;
	uint16_t version = 0;
	vnx::bool_t is_deleted = 0;
	vnx::float32_t rank_value = 0;
	int64_t last_updated = 0;
	uint64_t index_version = 0;
	uint64_t link_version = 0;
	uint64_t word_version = 0;
	uint64_t array_version = 0;
	std::vector<uint32_t> words;
	std::vector<std::string> links;
	std::vector<::vnx::search::reverse_link_t> reverse_links;
	std::map<std::string, int32_t> reverse_domains;
	
	typedef ::vnx::Value Super;
	
	static const vnx::Hash64 VNX_TYPE_HASH;
	static const vnx::Hash64 VNX_CODE_HASH;
	
	static constexpr uint64_t VNX_TYPE_ID = 0x547cfd9b2bb19c80ull;
	
	vnx::Hash64 get_type_hash() const override;
	std::string get_type_name() const override;
	const vnx::TypeCode* get_type_code() const override;
	
	static std::shared_ptr<PageInfo> create();
	std::shared_ptr<vnx::Value> clone() const override;
	
	void read(vnx::TypeInput& _in, const vnx::TypeCode* _type_code, const uint16_t* _code) override;
	void write(vnx::TypeOutput& _out, const vnx::TypeCode* _type_code, const uint16_t* _code) const override;
	
	void read(std::istream& _in) override;
	void write(std::ostream& _out) const override;
	
	void accept(vnx::Visitor& _visitor) const override;
	
	vnx::Object to_object() const override;
	void from_object(const vnx::Object& object) override;
	
	vnx::Variant get_field(const std::string& name) const override;
	void set_field(const std::string& name, const vnx::Variant& value) override;
	
	friend std::ostream& operator<<(std::ostream& _out, const PageInfo& _value);
	friend std::istream& operator>>(std::istream& _in, PageInfo& _value);
	
	static const vnx::TypeCode* static_get_type_code();
	static std::shared_ptr<vnx::TypeCode> static_create_type_code();
	
};


} // namespace vnx
} // namespace search


namespace vnx {

} // vnx

#endif // INCLUDE_vnx_search_PageInfo_HXX_
