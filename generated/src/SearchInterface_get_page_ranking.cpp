
// AUTO GENERATED by vnxcppcodegen

#include <vnx/search/package.hxx>
#include <vnx/search/SearchInterface_get_page_ranking.hxx>
#include <vnx/Value.h>
#include <vnx/search/SearchInterface_get_page_ranking_return.hxx>

#include <vnx/vnx.h>


namespace vnx {
namespace search {


const vnx::Hash64 SearchInterface_get_page_ranking::VNX_TYPE_HASH(0xd3878abff5f383d6ull);
const vnx::Hash64 SearchInterface_get_page_ranking::VNX_CODE_HASH(0x37baf207396af96ull);

vnx::Hash64 SearchInterface_get_page_ranking::get_type_hash() const {
	return VNX_TYPE_HASH;
}

std::string SearchInterface_get_page_ranking::get_type_name() const {
	return "vnx.search.SearchInterface.get_page_ranking";
}

const vnx::TypeCode* SearchInterface_get_page_ranking::get_type_code() const {
	return vnx::search::vnx_native_type_code_SearchInterface_get_page_ranking;
}

std::shared_ptr<SearchInterface_get_page_ranking> SearchInterface_get_page_ranking::create() {
	return std::make_shared<SearchInterface_get_page_ranking>();
}

std::shared_ptr<vnx::Value> SearchInterface_get_page_ranking::clone() const {
	return std::make_shared<SearchInterface_get_page_ranking>(*this);
}

void SearchInterface_get_page_ranking::read(vnx::TypeInput& _in, const vnx::TypeCode* _type_code, const uint16_t* _code) {
	vnx::read(_in, *this, _type_code, _code);
}

void SearchInterface_get_page_ranking::write(vnx::TypeOutput& _out, const vnx::TypeCode* _type_code, const uint16_t* _code) const {
	vnx::write(_out, *this, _type_code, _code);
}

void SearchInterface_get_page_ranking::accept(vnx::Visitor& _visitor) const {
	const vnx::TypeCode* _type_code = vnx::search::vnx_native_type_code_SearchInterface_get_page_ranking;
	_visitor.type_begin(*_type_code);
	_visitor.type_field(_type_code->fields[0], 0); vnx::accept(_visitor, limit);
	_visitor.type_field(_type_code->fields[1], 1); vnx::accept(_visitor, offset);
	_visitor.type_end(*_type_code);
}

void SearchInterface_get_page_ranking::write(std::ostream& _out) const {
	_out << "{\"__type\": \"vnx.search.SearchInterface.get_page_ranking\"";
	_out << ", \"limit\": "; vnx::write(_out, limit);
	_out << ", \"offset\": "; vnx::write(_out, offset);
	_out << "}";
}

void SearchInterface_get_page_ranking::read(std::istream& _in) {
	if(auto _json = vnx::read_json(_in)) {
		from_object(_json->to_object());
	}
}

vnx::Object SearchInterface_get_page_ranking::to_object() const {
	vnx::Object _object;
	_object["__type"] = "vnx.search.SearchInterface.get_page_ranking";
	_object["limit"] = limit;
	_object["offset"] = offset;
	return _object;
}

void SearchInterface_get_page_ranking::from_object(const vnx::Object& _object) {
	for(const auto& _entry : _object.field) {
		if(_entry.first == "limit") {
			_entry.second.to(limit);
		} else if(_entry.first == "offset") {
			_entry.second.to(offset);
		}
	}
}

vnx::Variant SearchInterface_get_page_ranking::get_field(const std::string& _name) const {
	if(_name == "limit") {
		return vnx::Variant(limit);
	}
	if(_name == "offset") {
		return vnx::Variant(offset);
	}
	return vnx::Variant();
}

void SearchInterface_get_page_ranking::set_field(const std::string& _name, const vnx::Variant& _value) {
	if(_name == "limit") {
		_value.to(limit);
	} else if(_name == "offset") {
		_value.to(offset);
	} else {
		throw std::logic_error("no such field: '" + _name + "'");
	}
}

/// \private
std::ostream& operator<<(std::ostream& _out, const SearchInterface_get_page_ranking& _value) {
	_value.write(_out);
	return _out;
}

/// \private
std::istream& operator>>(std::istream& _in, SearchInterface_get_page_ranking& _value) {
	_value.read(_in);
	return _in;
}

const vnx::TypeCode* SearchInterface_get_page_ranking::static_get_type_code() {
	const vnx::TypeCode* type_code = vnx::get_type_code(VNX_TYPE_HASH);
	if(!type_code) {
		type_code = vnx::register_type_code(static_create_type_code());
	}
	return type_code;
}

std::shared_ptr<vnx::TypeCode> SearchInterface_get_page_ranking::static_create_type_code() {
	auto type_code = std::make_shared<vnx::TypeCode>();
	type_code->name = "vnx.search.SearchInterface.get_page_ranking";
	type_code->type_hash = vnx::Hash64(0xd3878abff5f383d6ull);
	type_code->code_hash = vnx::Hash64(0x37baf207396af96ull);
	type_code->is_native = true;
	type_code->is_class = true;
	type_code->is_method = true;
	type_code->native_size = sizeof(::vnx::search::SearchInterface_get_page_ranking);
	type_code->create_value = []() -> std::shared_ptr<vnx::Value> { return std::make_shared<SearchInterface_get_page_ranking>(); };
	type_code->is_const = true;
	type_code->is_async = true;
	type_code->return_type = ::vnx::search::SearchInterface_get_page_ranking_return::static_get_type_code();
	type_code->fields.resize(2);
	{
		auto& field = type_code->fields[0];
		field.data_size = 4;
		field.name = "limit";
		field.value = vnx::to_string(10);
		field.code = {7};
	}
	{
		auto& field = type_code->fields[1];
		field.data_size = 4;
		field.name = "offset";
		field.code = {3};
	}
	type_code->build();
	return type_code;
}


} // namespace vnx
} // namespace search


namespace vnx {

void read(TypeInput& in, ::vnx::search::SearchInterface_get_page_ranking& value, const TypeCode* type_code, const uint16_t* code) {
	if(code) {
		switch(code[0]) {
			case CODE_OBJECT:
			case CODE_ALT_OBJECT: {
				Object tmp;
				vnx::read(in, tmp, type_code, code);
				value.from_object(tmp);
				return;
			}
			case CODE_DYNAMIC:
			case CODE_ALT_DYNAMIC:
				vnx::read_dynamic(in, value);
				return;
		}
	}
	if(!type_code) {
		vnx::skip(in, type_code, code);
		return;
	}
	if(code) {
		switch(code[0]) {
			case CODE_STRUCT: type_code = type_code->depends[code[1]]; break;
			case CODE_ALT_STRUCT: type_code = type_code->depends[vnx::flip_bytes(code[1])]; break;
			default: {
				vnx::skip(in, type_code, code);
				return;
			}
		}
	}
	const char* const _buf = in.read(type_code->total_field_size);
	if(type_code->is_matched) {
		if(const auto* const _field = type_code->field_map[0]) {
			vnx::read_value(_buf + _field->offset, value.limit, _field->code.data());
		}
		if(const auto* const _field = type_code->field_map[1]) {
			vnx::read_value(_buf + _field->offset, value.offset, _field->code.data());
		}
	}
	for(const auto* _field : type_code->ext_fields) {
		switch(_field->native_index) {
			default: vnx::skip(in, type_code, _field->code.data());
		}
	}
}

void write(TypeOutput& out, const ::vnx::search::SearchInterface_get_page_ranking& value, const TypeCode* type_code, const uint16_t* code) {
	if(code && code[0] == CODE_OBJECT) {
		vnx::write(out, value.to_object(), nullptr, code);
		return;
	}
	if(!type_code || (code && code[0] == CODE_ANY)) {
		type_code = vnx::search::vnx_native_type_code_SearchInterface_get_page_ranking;
		out.write_type_code(type_code);
		vnx::write_class_header<::vnx::search::SearchInterface_get_page_ranking>(out);
	}
	else if(code && code[0] == CODE_STRUCT) {
		type_code = type_code->depends[code[1]];
	}
	char* const _buf = out.write(8);
	vnx::write_value(_buf + 0, value.limit);
	vnx::write_value(_buf + 4, value.offset);
}

void read(std::istream& in, ::vnx::search::SearchInterface_get_page_ranking& value) {
	value.read(in);
}

void write(std::ostream& out, const ::vnx::search::SearchInterface_get_page_ranking& value) {
	value.write(out);
}

void accept(Visitor& visitor, const ::vnx::search::SearchInterface_get_page_ranking& value) {
	value.accept(visitor);
}

} // vnx
