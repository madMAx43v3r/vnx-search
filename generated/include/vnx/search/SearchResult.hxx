
// AUTO GENERATED by vnxcppcodegen

#ifndef INCLUDE_vnx_search_SearchResult_HXX_
#define INCLUDE_vnx_search_SearchResult_HXX_

#include <vnx/search/package.hxx>
#include <vnx/Value.h>
#include <vnx/search/result_item_t.hxx>


namespace vnx {
namespace search {

class SearchResult : public ::vnx::Value {
public:
	
	vnx::bool_t is_fail = 0;
	vnx::bool_t has_more = 0;
	int32_t num_results_total = 0;
	int32_t load_time_us = 0;
	int32_t compute_time_us = 0;
	std::string error_msg;
	std::vector<std::string> words;
	std::vector<::vnx::search::result_item_t> items;
	
	typedef ::vnx::Value Super;
	
	static const vnx::Hash64 VNX_TYPE_HASH;
	static const vnx::Hash64 VNX_CODE_HASH;
	
	vnx::Hash64 get_type_hash() const override;
	const char* get_type_name() const override;
	const vnx::TypeCode* get_type_code() const override;
	
	static std::shared_ptr<SearchResult> create();
	std::shared_ptr<vnx::Value> clone() const;
	
	void read(vnx::TypeInput& _in, const vnx::TypeCode* _type_code, const uint16_t* _code);
	void write(vnx::TypeOutput& _out, const vnx::TypeCode* _type_code, const uint16_t* _code) const;
	
	void read(std::istream& _in);
	void write(std::ostream& _out) const;
	
	void accept(vnx::Visitor& _visitor) const;
	
	vnx::Object to_object() const;
	void from_object(const vnx::Object& object);
	
	friend std::ostream& operator<<(std::ostream& _out, const SearchResult& _value);
	friend std::istream& operator>>(std::istream& _in, SearchResult& _value);
	
	static const vnx::TypeCode* static_get_type_code();
	static std::shared_ptr<vnx::TypeCode> static_create_type_code();
	
};


} // namespace vnx
} // namespace search

#endif // INCLUDE_vnx_search_SearchResult_HXX_
