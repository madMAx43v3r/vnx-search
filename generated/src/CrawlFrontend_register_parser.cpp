
// AUTO GENERATED by vnxcppcodegen

#include <vnx/search/package.hxx>
#include <vnx/search/CrawlFrontend_register_parser.hxx>
#include <vnx/Hash64.hpp>
#include <vnx/Value.h>
#include <vnx/search/CrawlFrontend_register_parser_return.hxx>

#include <vnx/vnx.h>


namespace vnx {
namespace search {


const vnx::Hash64 CrawlFrontend_register_parser::VNX_TYPE_HASH(0x4b91d9c8a161bbfdull);
const vnx::Hash64 CrawlFrontend_register_parser::VNX_CODE_HASH(0xb04794a3be17e5d2ull);

vnx::Hash64 CrawlFrontend_register_parser::get_type_hash() const {
	return VNX_TYPE_HASH;
}

const char* CrawlFrontend_register_parser::get_type_name() const {
	return "vnx.search.CrawlFrontend.register_parser";
}
const vnx::TypeCode* CrawlFrontend_register_parser::get_type_code() const {
	return vnx::search::vnx_native_type_code_CrawlFrontend_register_parser;
}

std::shared_ptr<CrawlFrontend_register_parser> CrawlFrontend_register_parser::create() {
	return std::make_shared<CrawlFrontend_register_parser>();
}

std::shared_ptr<vnx::Value> CrawlFrontend_register_parser::clone() const {
	return std::make_shared<CrawlFrontend_register_parser>(*this);
}

void CrawlFrontend_register_parser::read(vnx::TypeInput& _in, const vnx::TypeCode* _type_code, const uint16_t* _code) {
	vnx::read(_in, *this, _type_code, _code);
}

void CrawlFrontend_register_parser::write(vnx::TypeOutput& _out, const vnx::TypeCode* _type_code, const uint16_t* _code) const {
	vnx::write(_out, *this, _type_code, _code);
}

void CrawlFrontend_register_parser::accept(vnx::Visitor& _visitor) const {
	const vnx::TypeCode* _type_code = vnx::search::vnx_native_type_code_CrawlFrontend_register_parser;
	_visitor.type_begin(*_type_code);
	_visitor.type_field(_type_code->fields[0], 0); vnx::accept(_visitor, address);
	_visitor.type_field(_type_code->fields[1], 1); vnx::accept(_visitor, mime_types);
	_visitor.type_field(_type_code->fields[2], 2); vnx::accept(_visitor, num_threads);
	_visitor.type_end(*_type_code);
}

void CrawlFrontend_register_parser::write(std::ostream& _out) const {
	_out << "{\"__type\": \"vnx.search.CrawlFrontend.register_parser\"";
	_out << ", \"address\": "; vnx::write(_out, address);
	_out << ", \"mime_types\": "; vnx::write(_out, mime_types);
	_out << ", \"num_threads\": "; vnx::write(_out, num_threads);
	_out << "}";
}

void CrawlFrontend_register_parser::read(std::istream& _in) {
	std::map<std::string, std::string> _object;
	vnx::read_object(_in, _object);
	for(const auto& _entry : _object) {
		if(_entry.first == "address") {
			vnx::from_string(_entry.second, address);
		} else if(_entry.first == "mime_types") {
			vnx::from_string(_entry.second, mime_types);
		} else if(_entry.first == "num_threads") {
			vnx::from_string(_entry.second, num_threads);
		}
	}
}

vnx::Object CrawlFrontend_register_parser::to_object() const {
	vnx::Object _object;
	_object["__type"] = "vnx.search.CrawlFrontend.register_parser";
	_object["address"] = address;
	_object["mime_types"] = mime_types;
	_object["num_threads"] = num_threads;
	return _object;
}

void CrawlFrontend_register_parser::from_object(const vnx::Object& _object) {
	for(const auto& _entry : _object.field) {
		if(_entry.first == "address") {
			_entry.second.to(address);
		} else if(_entry.first == "mime_types") {
			_entry.second.to(mime_types);
		} else if(_entry.first == "num_threads") {
			_entry.second.to(num_threads);
		}
	}
}

vnx::Variant CrawlFrontend_register_parser::get_field(const std::string& _name) const {
	if(_name == "address") {
		return vnx::Variant(address);
	}
	if(_name == "mime_types") {
		return vnx::Variant(mime_types);
	}
	if(_name == "num_threads") {
		return vnx::Variant(num_threads);
	}
	return vnx::Variant();
}

void CrawlFrontend_register_parser::set_field(const std::string& _name, const vnx::Variant& _value) {
	if(_name == "address") {
		_value.to(address);
	} else if(_name == "mime_types") {
		_value.to(mime_types);
	} else if(_name == "num_threads") {
		_value.to(num_threads);
	} else {
		throw std::logic_error("no such field: '" + _name + "'");
	}
}

/// \private
std::ostream& operator<<(std::ostream& _out, const CrawlFrontend_register_parser& _value) {
	_value.write(_out);
	return _out;
}

/// \private
std::istream& operator>>(std::istream& _in, CrawlFrontend_register_parser& _value) {
	_value.read(_in);
	return _in;
}

const vnx::TypeCode* CrawlFrontend_register_parser::static_get_type_code() {
	const vnx::TypeCode* type_code = vnx::get_type_code(VNX_TYPE_HASH);
	if(!type_code) {
		type_code = vnx::register_type_code(static_create_type_code());
	}
	return type_code;
}

std::shared_ptr<vnx::TypeCode> CrawlFrontend_register_parser::static_create_type_code() {
	std::shared_ptr<vnx::TypeCode> type_code = std::make_shared<vnx::TypeCode>();
	type_code->name = "vnx.search.CrawlFrontend.register_parser";
	type_code->type_hash = vnx::Hash64(0x4b91d9c8a161bbfdull);
	type_code->code_hash = vnx::Hash64(0xb04794a3be17e5d2ull);
	type_code->is_native = true;
	type_code->is_class = true;
	type_code->is_method = true;
	type_code->create_value = []() -> std::shared_ptr<vnx::Value> { return std::make_shared<CrawlFrontend_register_parser>(); };
	type_code->return_type = ::vnx::search::CrawlFrontend_register_parser_return::static_get_type_code();
	type_code->fields.resize(3);
	{
		vnx::TypeField& field = type_code->fields[0];
		field.is_extended = true;
		field.name = "address";
		field.code = {4};
	}
	{
		vnx::TypeField& field = type_code->fields[1];
		field.is_extended = true;
		field.name = "mime_types";
		field.code = {12, 32};
	}
	{
		vnx::TypeField& field = type_code->fields[2];
		field.name = "num_threads";
		field.code = {7};
	}
	type_code->build();
	return type_code;
}


} // namespace vnx
} // namespace search


namespace vnx {

void read(TypeInput& in, ::vnx::search::CrawlFrontend_register_parser& value, const TypeCode* type_code, const uint16_t* code) {
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
			const vnx::TypeField* const _field = type_code->field_map[2];
			if(_field) {
				vnx::read_value(_buf + _field->offset, value.num_threads, _field->code.data());
			}
		}
	}
	for(const vnx::TypeField* _field : type_code->ext_fields) {
		switch(_field->native_index) {
			case 0: vnx::read(in, value.address, type_code, _field->code.data()); break;
			case 1: vnx::read(in, value.mime_types, type_code, _field->code.data()); break;
			default: vnx::skip(in, type_code, _field->code.data());
		}
	}
}

void write(TypeOutput& out, const ::vnx::search::CrawlFrontend_register_parser& value, const TypeCode* type_code, const uint16_t* code) {
	if(!type_code || (code && code[0] == CODE_ANY)) {
		type_code = vnx::search::vnx_native_type_code_CrawlFrontend_register_parser;
		out.write_type_code(type_code);
		vnx::write_class_header<::vnx::search::CrawlFrontend_register_parser>(out);
	}
	if(code && code[0] == CODE_STRUCT) {
		type_code = type_code->depends[code[1]];
	}
	char* const _buf = out.write(4);
	vnx::write_value(_buf + 0, value.num_threads);
	vnx::write(out, value.address, type_code, type_code->fields[0].code.data());
	vnx::write(out, value.mime_types, type_code, type_code->fields[1].code.data());
}

void read(std::istream& in, ::vnx::search::CrawlFrontend_register_parser& value) {
	value.read(in);
}

void write(std::ostream& out, const ::vnx::search::CrawlFrontend_register_parser& value) {
	value.write(out);
}

void accept(Visitor& visitor, const ::vnx::search::CrawlFrontend_register_parser& value) {
	value.accept(visitor);
}

} // vnx
