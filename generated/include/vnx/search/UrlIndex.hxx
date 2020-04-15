
// AUTO GENERATED by vnxcppcodegen

#ifndef INCLUDE_vnx_search_UrlIndex_HXX_
#define INCLUDE_vnx_search_UrlIndex_HXX_

#include <vnx/search/package.hxx>
#include <vnx/Value.h>


namespace vnx {
namespace search {

class UrlIndex : public ::vnx::Value {
public:
	
	::std::string scheme;
	::std::string content_type;
	::int64_t first_seen = 0;
	::int64_t last_fetched = 0;
	::int64_t last_modified = 0;
	::int64_t fetch_duration_us = 0;
	::int32_t http_status = -1;
	::int32_t curl_status = -1;
	::int32_t fetch_count = 0;
	::int32_t depth = -1;
	::vnx::bool_t is_fail = 0;
	
	typedef ::vnx::Value Super;
	
	static const vnx::Hash64 VNX_TYPE_HASH;
	static const vnx::Hash64 VNX_CODE_HASH;
	
	vnx::Hash64 get_type_hash() const override;
	const char* get_type_name() const override;
	const vnx::TypeCode* get_type_code() const override;
	
	static std::shared_ptr<UrlIndex> create();
	std::shared_ptr<vnx::Value> clone() const;
	
	void read(vnx::TypeInput& _in, const vnx::TypeCode* _type_code, const uint16_t* _code);
	void write(vnx::TypeOutput& _out, const vnx::TypeCode* _type_code, const uint16_t* _code) const;
	
	void read(std::istream& _in);
	void write(std::ostream& _out) const;
	
	void accept(vnx::Visitor& _visitor) const;
	
	vnx::Object to_object() const;
	void from_object(const vnx::Object& object);
	
	friend std::ostream& operator<<(std::ostream& _out, const UrlIndex& _value);
	friend std::istream& operator>>(std::istream& _in, UrlIndex& _value);
	
	static const vnx::TypeCode* static_get_type_code();
	static std::shared_ptr<vnx::TypeCode> static_create_type_code();
	
};


} // namespace vnx
} // namespace search

#endif // INCLUDE_vnx_search_UrlIndex_HXX_
