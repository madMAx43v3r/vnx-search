
// AUTO GENERATED by vnxcppcodegen

#ifndef INCLUDE_vnx_search_reverse_link_t_HXX_
#define INCLUDE_vnx_search_reverse_link_t_HXX_

#include <vnx/Type.h>
#include <vnx/search/package.hxx>
#include <vnx/Variant.hpp>


namespace vnx {
namespace search {

struct reverse_link_t {
	
	
	std::string url_key;
	std::vector<uint32_t> words;
	
	static const vnx::Hash64 VNX_TYPE_HASH;
	static const vnx::Hash64 VNX_CODE_HASH;
	
	static constexpr uint64_t VNX_TYPE_ID = 0xaaf3ea38b730dac0ull;
	
	vnx::Hash64 get_type_hash() const;
	std::string get_type_name() const;
	const vnx::TypeCode* get_type_code() const;
	
	void vnx_read_fallback(const ::vnx::Variant& var);
	
	static std::shared_ptr<reverse_link_t> create();
	std::shared_ptr<reverse_link_t> clone() const;
	
	void read(vnx::TypeInput& _in, const vnx::TypeCode* _type_code, const uint16_t* _code);
	void write(vnx::TypeOutput& _out, const vnx::TypeCode* _type_code, const uint16_t* _code) const;
	
	void read(std::istream& _in);
	void write(std::ostream& _out) const;
	
	void accept(vnx::Visitor& _visitor) const;
	
	vnx::Object to_object() const;
	void from_object(const vnx::Object& object);
	
	vnx::Variant get_field(const std::string& name) const;
	void set_field(const std::string& name, const vnx::Variant& value);
	
	friend std::ostream& operator<<(std::ostream& _out, const reverse_link_t& _value);
	friend std::istream& operator>>(std::istream& _in, reverse_link_t& _value);
	
	static const vnx::TypeCode* static_get_type_code();
	static std::shared_ptr<vnx::TypeCode> static_create_type_code();
	
};


} // namespace vnx
} // namespace search


namespace vnx {

template<>
struct is_equivalent<::vnx::search::reverse_link_t> {
	bool operator()(const uint16_t* code, const TypeCode* type_code);
};

} // vnx

#endif // INCLUDE_vnx_search_reverse_link_t_HXX_
