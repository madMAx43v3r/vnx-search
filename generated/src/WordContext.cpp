
// AUTO GENERATED by vnxcppcodegen

#include <vnx/search/package.hxx>
#include <vnx/search/WordContext.hxx>
#include <vnx/Input.h>
#include <vnx/Output.h>
#include <vnx/Visitor.h>
#include <vnx/Object.h>
#include <vnx/Struct.h>


namespace vnx {
namespace search {


const vnx::Hash64 WordContext::VNX_TYPE_HASH(0xdaa75d84f367905bull);
const vnx::Hash64 WordContext::VNX_CODE_HASH(0xa599c21c16f1afbfull);

vnx::Hash64 WordContext::get_type_hash() const {
	return VNX_TYPE_HASH;
}

const char* WordContext::get_type_name() const {
	return "vnx.search.WordContext";
}
const vnx::TypeCode* WordContext::get_type_code() const {
	return vnx::search::vnx_native_type_code_WordContext;
}

std::shared_ptr<WordContext> WordContext::create() {
	return std::make_shared<WordContext>();
}

std::shared_ptr<vnx::Value> WordContext::clone() const {
	return std::make_shared<WordContext>(*this);
}

void WordContext::read(vnx::TypeInput& _in, const vnx::TypeCode* _type_code, const uint16_t* _code) {
	vnx::read(_in, *this, _type_code, _code);
}

void WordContext::write(vnx::TypeOutput& _out, const vnx::TypeCode* _type_code, const uint16_t* _code) const {
	vnx::write(_out, *this, _type_code, _code);
}

void WordContext::accept(vnx::Visitor& _visitor) const {
	const vnx::TypeCode* _type_code = vnx::search::vnx_native_type_code_WordContext;
	_visitor.type_begin(*_type_code);
	_visitor.type_field(_type_code->fields[0], 0); vnx::accept(_visitor, last_update);
	_visitor.type_field(_type_code->fields[1], 1); vnx::accept(_visitor, pages);
	_visitor.type_end(*_type_code);
}

void WordContext::write(std::ostream& _out) const {
	_out << "{";
	_out << "\"last_update\": "; vnx::write(_out, last_update);
	_out << ", \"pages\": "; vnx::write(_out, pages);
	_out << "}";
}

void WordContext::read(std::istream& _in) {
	std::map<std::string, std::string> _object;
	vnx::read_object(_in, _object);
	for(const auto& _entry : _object) {
		if(_entry.first == "last_update") {
			vnx::from_string(_entry.second, last_update);
		} else if(_entry.first == "pages") {
			vnx::from_string(_entry.second, pages);
		}
	}
}

vnx::Object WordContext::to_object() const {
	vnx::Object _object;
	_object["last_update"] = last_update;
	_object["pages"] = pages;
	return _object;
}

void WordContext::from_object(const vnx::Object& _object) {
	for(const auto& _entry : _object.field) {
		if(_entry.first == "last_update") {
			_entry.second.to(last_update);
		} else if(_entry.first == "pages") {
			_entry.second.to(pages);
		}
	}
}

/// \private
std::ostream& operator<<(std::ostream& _out, const WordContext& _value) {
	_value.write(_out);
	return _out;
}

/// \private
std::istream& operator>>(std::istream& _in, WordContext& _value) {
	_value.read(_in);
	return _in;
}

const vnx::TypeCode* WordContext::static_get_type_code() {
	const vnx::TypeCode* type_code = vnx::get_type_code(vnx::Hash64(0xdaa75d84f367905bull));
	if(!type_code) {
		type_code = vnx::register_type_code(static_create_type_code());
	}
	return type_code;
}

std::shared_ptr<vnx::TypeCode> WordContext::static_create_type_code() {
	std::shared_ptr<vnx::TypeCode> type_code = std::make_shared<vnx::TypeCode>(true);
	type_code->name = "vnx.search.WordContext";
	type_code->type_hash = vnx::Hash64(0xdaa75d84f367905bull);
	type_code->code_hash = vnx::Hash64(0xa599c21c16f1afbfull);
	type_code->is_class = true;
	type_code->create_value = []() -> std::shared_ptr<vnx::Value> { return std::make_shared<WordContext>(); };
	type_code->methods.resize(0);
	type_code->fields.resize(2);
	{
		vnx::TypeField& field = type_code->fields[0];
		field.name = "last_update";
		field.code = {8};
	}
	{
		vnx::TypeField& field = type_code->fields[1];
		field.is_extended = true;
		field.name = "pages";
		field.code = {12, 23, 2, 4, 5, 3, 2};
	}
	type_code->build();
	return type_code;
}


} // namespace vnx
} // namespace search


namespace vnx {

void read(TypeInput& in, ::vnx::search::WordContext& value, const TypeCode* type_code, const uint16_t* code) {
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
				vnx::read_value(_buf + _field->offset, value.last_update, _field->code.data());
			}
		}
	}
	for(const vnx::TypeField* _field : type_code->ext_fields) {
		switch(_field->native_index) {
			case 1: vnx::read(in, value.pages, type_code, _field->code.data()); break;
			default: vnx::skip(in, type_code, _field->code.data());
		}
	}
}

void write(TypeOutput& out, const ::vnx::search::WordContext& value, const TypeCode* type_code, const uint16_t* code) {
	if(!type_code || (code && code[0] == CODE_ANY)) {
		type_code = vnx::search::vnx_native_type_code_WordContext;
		out.write_type_code(type_code);
		vnx::write_class_header<::vnx::search::WordContext>(out);
	}
	if(code && code[0] == CODE_STRUCT) {
		type_code = type_code->depends[code[1]];
	}
	char* const _buf = out.write(8);
	vnx::write_value(_buf + 0, value.last_update);
	vnx::write(out, value.pages, type_code, type_code->fields[1].code.data());
}

void read(std::istream& in, ::vnx::search::WordContext& value) {
	value.read(in);
}

void write(std::ostream& out, const ::vnx::search::WordContext& value) {
	value.write(out);
}

void accept(Visitor& visitor, const ::vnx::search::WordContext& value) {
	value.accept(visitor);
}

} // vnx
