
// AUTO GENERATED by vnxcppcodegen

#include <vnx/vnx.h>
#include <vnx/search/package.hxx>
#include <vnx/search/search_flags_e.hxx>



namespace vnx {
namespace search {


const vnx::Hash64 search_flags_e::VNX_TYPE_HASH(0xe55b90a8ffab58ffull);
const vnx::Hash64 search_flags_e::VNX_CODE_HASH(0x2950544b0fdbcb69ull);

vnx::Hash64 search_flags_e::get_type_hash() const {
	return VNX_TYPE_HASH;
}

const char* search_flags_e::get_type_name() const {
	return "vnx.search.search_flags_e";
}
const vnx::TypeCode* search_flags_e::get_type_code() const {
	return vnx::search::vnx_native_type_code_search_flags_e;
}

std::shared_ptr<search_flags_e> search_flags_e::create() {
	return std::make_shared<search_flags_e>();
}

std::shared_ptr<search_flags_e> search_flags_e::clone() const {
	return std::make_shared<search_flags_e>(*this);
}

void search_flags_e::read(vnx::TypeInput& _in, const vnx::TypeCode* _type_code, const uint16_t* _code) {
	vnx::read(_in, *this, _type_code, _code);
}

void search_flags_e::write(vnx::TypeOutput& _out, const vnx::TypeCode* _type_code, const uint16_t* _code) const {
	vnx::write(_out, *this, _type_code, _code);
}

void search_flags_e::accept(vnx::Visitor& _visitor) const {
	std::string _name;
	switch(value) {
		case GROUP_BY_DOMAIN: _name = "GROUP_BY_DOMAIN"; break;
		case LATEST_NEWS: _name = "LATEST_NEWS"; break;
	}
	_visitor.enum_value(value, _name);
}

void search_flags_e::write(std::ostream& _out) const {
	switch(value) {
		case GROUP_BY_DOMAIN: _out << "\"GROUP_BY_DOMAIN\""; break;
		case LATEST_NEWS: _out << "\"LATEST_NEWS\""; break;
		default: _out << value;
	}
}

void search_flags_e::read(std::istream& _in) {
	std::string _name;
	vnx::read(_in, _name);
	if(_name == "GROUP_BY_DOMAIN") value = 1161442717;
	else if(_name == "LATEST_NEWS") value = 1050161006;
	else value = std::atoi(_name.c_str());
}

vnx::Object search_flags_e::to_object() const {
	vnx::Object _object;
	_object["__type"] = "vnx.search.search_flags_e";
	_object["value"] = value;
	return _object;
}

void search_flags_e::from_object(const vnx::Object& _object) {
	for(const auto& _entry : _object.field) {
		if(_entry.first == "value") {
			_entry.second.to(value);
		}
	}
}

/// \private
std::ostream& operator<<(std::ostream& _out, const search_flags_e& _value) {
	_value.write(_out);
	return _out;
}

/// \private
std::istream& operator>>(std::istream& _in, search_flags_e& _value) {
	_value.read(_in);
	return _in;
}

const vnx::TypeCode* search_flags_e::static_get_type_code() {
	const vnx::TypeCode* type_code = vnx::get_type_code(VNX_TYPE_HASH);
	if(!type_code) {
		type_code = vnx::register_type_code(static_create_type_code());
	}
	return type_code;
}

std::shared_ptr<vnx::TypeCode> search_flags_e::static_create_type_code() {
	std::shared_ptr<vnx::TypeCode> type_code = std::make_shared<vnx::TypeCode>();
	type_code->name = "vnx.search.search_flags_e";
	type_code->type_hash = vnx::Hash64(0xe55b90a8ffab58ffull);
	type_code->code_hash = vnx::Hash64(0x2950544b0fdbcb69ull);
	type_code->is_native = true;
	type_code->is_enum = true;
	type_code->create_value = []() -> std::shared_ptr<vnx::Value> { return std::make_shared<vnx::Struct<search_flags_e>>(); };
	type_code->fields.resize(1);
	{
		vnx::TypeField& field = type_code->fields[0];
		field.name = "value";
		field.code = {3};
	}
	type_code->enum_map[1161442717] = "GROUP_BY_DOMAIN";
	type_code->enum_map[1050161006] = "LATEST_NEWS";
	type_code->build();
	return type_code;
}


} // namespace vnx
} // namespace search


namespace vnx {

void read(TypeInput& in, ::vnx::search::search_flags_e& value, const TypeCode* type_code, const uint16_t* code) {
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
		{
			const vnx::TypeField* const _field = type_code->field_map[0];
			if(_field) {
				vnx::read_value(_buf + _field->offset, value.value, _field->code.data());
			}
		}
	}
	for(const vnx::TypeField* _field : type_code->ext_fields) {
		switch(_field->native_index) {
			default: vnx::skip(in, type_code, _field->code.data());
		}
	}
}

void write(TypeOutput& out, const ::vnx::search::search_flags_e& value, const TypeCode* type_code, const uint16_t* code) {
	if(!type_code || (code && code[0] == CODE_ANY)) {
		type_code = vnx::search::vnx_native_type_code_search_flags_e;
		out.write_type_code(type_code);
		vnx::write_class_header<::vnx::search::search_flags_e>(out);
	}
	if(code && code[0] == CODE_STRUCT) {
		type_code = type_code->depends[code[1]];
	}
	char* const _buf = out.write(4);
	vnx::write_value(_buf + 0, value.value);
}

void read(std::istream& in, ::vnx::search::search_flags_e& value) {
	value.read(in);
}

void write(std::ostream& out, const ::vnx::search::search_flags_e& value) {
	value.write(out);
}

void accept(Visitor& visitor, const ::vnx::search::search_flags_e& value) {
	value.accept(visitor);
}

} // vnx
