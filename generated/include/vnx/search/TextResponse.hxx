
// AUTO GENERATED by vnxcppcodegen

#ifndef INCLUDE_vnx_search_TextResponse_HXX_
#define INCLUDE_vnx_search_TextResponse_HXX_

#include <vnx/search/package.hxx>
#include <vnx/search/Response.hxx>


namespace vnx {
namespace search {

class TextResponse : public ::vnx::search::Response {
public:
	
	std::string title;
	std::string text;
	std::vector<std::string> links;
	std::vector<std::string> images;
	
	typedef ::vnx::search::Response Super;
	
	static const vnx::Hash64 VNX_TYPE_HASH;
	static const vnx::Hash64 VNX_CODE_HASH;
	
	vnx::Hash64 get_type_hash() const override;
	const char* get_type_name() const override;
	const vnx::TypeCode* get_type_code() const override;
	
	static std::shared_ptr<TextResponse> create();
	std::shared_ptr<vnx::Value> clone() const;
	
	void read(vnx::TypeInput& _in, const vnx::TypeCode* _type_code, const uint16_t* _code);
	void write(vnx::TypeOutput& _out, const vnx::TypeCode* _type_code, const uint16_t* _code) const;
	
	void read(std::istream& _in);
	void write(std::ostream& _out) const;
	
	void accept(vnx::Visitor& _visitor) const;
	
	vnx::Object to_object() const;
	void from_object(const vnx::Object& object);
	
	friend std::ostream& operator<<(std::ostream& _out, const TextResponse& _value);
	friend std::istream& operator>>(std::istream& _in, TextResponse& _value);
	
	static const vnx::TypeCode* static_get_type_code();
	static std::shared_ptr<vnx::TypeCode> static_create_type_code();
	
};


} // namespace vnx
} // namespace search

#endif // INCLUDE_vnx_search_TextResponse_HXX_
