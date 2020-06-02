
// AUTO GENERATED by vnxcppcodegen

#include <vnx/search/package.hxx>
#include <vnx/search/SearchEngine_reverse_lookup.hxx>
#include <vnx/Input.h>
#include <vnx/Output.h>
#include <vnx/Visitor.h>
#include <vnx/Object.h>
#include <vnx/Struct.h>
#include <vnx/Value.h>
#include <vnx/search/SearchEngine_reverse_lookup_return.hxx>



namespace vnx {
namespace search {


const vnx::Hash64 SearchEngine_reverse_lookup::VNX_TYPE_HASH(0x4e3b1cbd5cbd42afull);
const vnx::Hash64 SearchEngine_reverse_lookup::VNX_CODE_HASH(0x1f1048cd120909f7ull);

vnx::Hash64 SearchEngine_reverse_lookup::get_type_hash() const {
	return VNX_TYPE_HASH;
}

const char* SearchEngine_reverse_lookup::get_type_name() const {
	return "vnx.search.SearchEngine.reverse_lookup";
}
const vnx::TypeCode* SearchEngine_reverse_lookup::get_type_code() const {
	return vnx::search::vnx_native_type_code_SearchEngine_reverse_lookup;
}

std::shared_ptr<SearchEngine_reverse_lookup> SearchEngine_reverse_lookup::create() {
	return std::make_shared<SearchEngine_reverse_lookup>();
}

std::shared_ptr<vnx::Value> SearchEngine_reverse_lookup::clone() const {
	return std::make_shared<SearchEngine_reverse_lookup>(*this);
}

void SearchEngine_reverse_lookup::read(vnx::TypeInput& _in, const vnx::TypeCode* _type_code, const uint16_t* _code) {
	vnx::read(_in, *this, _type_code, _code);
}

void SearchEngine_reverse_lookup::write(vnx::TypeOutput& _out, const vnx::TypeCode* _type_code, const uint16_t* _code) const {
	vnx::write(_out, *this, _type_code, _code);
}

void SearchEngine_reverse_lookup::accept(vnx::Visitor& _visitor) const {
	const vnx::TypeCode* _type_code = vnx::search::vnx_native_type_code_SearchEngine_reverse_lookup;
	_visitor.type_begin(*_type_code);
	_visitor.type_field(_type_code->fields[0], 0); vnx::accept(_visitor, url_key);
	_visitor.type_end(*_type_code);
}

void SearchEngine_reverse_lookup::write(std::ostream& _out) const {
	_out << "{\"__type\": \"vnx.search.SearchEngine.reverse_lookup\"";
	_out << ", \"url_key\": "; vnx::write(_out, url_key);
	_out << "}";
}

void SearchEngine_reverse_lookup::read(std::istream& _in) {
	std::map<std::string, std::string> _object;
	vnx::read_object(_in, _object);
	for(const auto& _entry : _object) {
		if(_entry.first == "url_key") {
			vnx::from_string(_entry.second, url_key);
		}
	}
}

vnx::Object SearchEngine_reverse_lookup::to_object() const {
	vnx::Object _object;
	_object["url_key"] = url_key;
	return _object;
}

void SearchEngine_reverse_lookup::from_object(const vnx::Object& _object) {
	for(const auto& _entry : _object.field) {
		if(_entry.first == "url_key") {
			_entry.second.to(url_key);
		}
	}
}

/// \private
std::ostream& operator<<(std::ostream& _out, const SearchEngine_reverse_lookup& _value) {
	_value.write(_out);
	return _out;
}

/// \private
std::istream& operator>>(std::istream& _in, SearchEngine_reverse_lookup& _value) {
	_value.read(_in);
	return _in;
}

const vnx::TypeCode* SearchEngine_reverse_lookup::static_get_type_code() {
	const vnx::TypeCode* type_code = vnx::get_type_code(VNX_TYPE_HASH);
	if(!type_code) {
		type_code = vnx::register_type_code(static_create_type_code());
	}
	return type_code;
}

std::shared_ptr<vnx::TypeCode> SearchEngine_reverse_lookup::static_create_type_code() {
	std::shared_ptr<vnx::TypeCode> type_code = std::make_shared<vnx::TypeCode>();
	type_code->name = "vnx.search.SearchEngine.reverse_lookup";
	type_code->type_hash = vnx::Hash64(0x4e3b1cbd5cbd42afull);
	type_code->code_hash = vnx::Hash64(0x1f1048cd120909f7ull);
	type_code->is_native = true;
	type_code->is_class = true;
	type_code->is_method = true;
	type_code->create_value = []() -> std::shared_ptr<vnx::Value> { return std::make_shared<SearchEngine_reverse_lookup>(); };
	type_code->return_type = ::vnx::search::SearchEngine_reverse_lookup_return::static_get_type_code();
	type_code->fields.resize(1);
	{
		vnx::TypeField& field = type_code->fields[0];
		field.is_extended = true;
		field.name = "url_key";
		field.code = {12, 5};
	}
	type_code->build();
	return type_code;
}


} // namespace vnx
} // namespace search


namespace vnx {

void read(TypeInput& in, ::vnx::search::SearchEngine_reverse_lookup& value, const TypeCode* type_code, const uint16_t* code) {
	if(!type_code) {
		throw std::logic_error("read(): type_code == 0");
	}
	if(code) {
		switch(code[0]) {
			case CODE_STRUCT: type_code = type_code->depends[code[1]]; break;
			case CODE_ALT_STRUCT: type_code = type_code->depends[vnx::flip_bytes(code[1])]; break;
			default: vnx::skip(in, type_code, code); return;
		}
	}
	const char* const _buf = in.read(type_code->total_field_size);
	if(type_code->is_matched) {
	}
	for(const vnx::TypeField* _field : type_code->ext_fields) {
		switch(_field->native_index) {
			case 0: vnx::read(in, value.url_key, type_code, _field->code.data()); break;
			default: vnx::skip(in, type_code, _field->code.data());
		}
	}
}

void write(TypeOutput& out, const ::vnx::search::SearchEngine_reverse_lookup& value, const TypeCode* type_code, const uint16_t* code) {
	if(!type_code || (code && code[0] == CODE_ANY)) {
		type_code = vnx::search::vnx_native_type_code_SearchEngine_reverse_lookup;
		out.write_type_code(type_code);
		vnx::write_class_header<::vnx::search::SearchEngine_reverse_lookup>(out);
	}
	if(code && code[0] == CODE_STRUCT) {
		type_code = type_code->depends[code[1]];
	}
	vnx::write(out, value.url_key, type_code, type_code->fields[0].code.data());
}

void read(std::istream& in, ::vnx::search::SearchEngine_reverse_lookup& value) {
	value.read(in);
}

void write(std::ostream& out, const ::vnx::search::SearchEngine_reverse_lookup& value) {
	value.write(out);
}

void accept(Visitor& visitor, const ::vnx::search::SearchEngine_reverse_lookup& value) {
	value.accept(visitor);
}

} // vnx
