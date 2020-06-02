
// AUTO GENERATED by vnxcppcodegen

#include <vnx/search/package.hxx>
#include <vnx/search/Response.hxx>
#include <vnx/Input.h>
#include <vnx/Output.h>
#include <vnx/Visitor.h>
#include <vnx/Object.h>
#include <vnx/Struct.h>
#include <vnx/Value.h>



namespace vnx {
namespace search {


const vnx::Hash64 Response::VNX_TYPE_HASH(0xcf371c90803a778cull);
const vnx::Hash64 Response::VNX_CODE_HASH(0xb26d330e6838503aull);

vnx::Hash64 Response::get_type_hash() const {
	return VNX_TYPE_HASH;
}

const char* Response::get_type_name() const {
	return "vnx.search.Response";
}
const vnx::TypeCode* Response::get_type_code() const {
	return vnx::search::vnx_native_type_code_Response;
}

std::shared_ptr<Response> Response::create() {
	return std::make_shared<Response>();
}

std::shared_ptr<vnx::Value> Response::clone() const {
	return std::make_shared<Response>(*this);
}

void Response::read(vnx::TypeInput& _in, const vnx::TypeCode* _type_code, const uint16_t* _code) {
	vnx::read(_in, *this, _type_code, _code);
}

void Response::write(vnx::TypeOutput& _out, const vnx::TypeCode* _type_code, const uint16_t* _code) const {
	vnx::write(_out, *this, _type_code, _code);
}

void Response::accept(vnx::Visitor& _visitor) const {
	const vnx::TypeCode* _type_code = vnx::search::vnx_native_type_code_Response;
	_visitor.type_begin(*_type_code);
	_visitor.type_field(_type_code->fields[0], 0); vnx::accept(_visitor, url);
	_visitor.type_field(_type_code->fields[1], 1); vnx::accept(_visitor, date);
	_visitor.type_field(_type_code->fields[2], 2); vnx::accept(_visitor, last_modified);
	_visitor.type_field(_type_code->fields[3], 3); vnx::accept(_visitor, fetch_duration_us);
	_visitor.type_end(*_type_code);
}

void Response::write(std::ostream& _out) const {
	_out << "{\"__type\": \"vnx.search.Response\"";
	_out << ", \"url\": "; vnx::write(_out, url);
	_out << ", \"date\": "; vnx::write(_out, date);
	_out << ", \"last_modified\": "; vnx::write(_out, last_modified);
	_out << ", \"fetch_duration_us\": "; vnx::write(_out, fetch_duration_us);
	_out << "}";
}

void Response::read(std::istream& _in) {
	std::map<std::string, std::string> _object;
	vnx::read_object(_in, _object);
	for(const auto& _entry : _object) {
		if(_entry.first == "date") {
			vnx::from_string(_entry.second, date);
		} else if(_entry.first == "fetch_duration_us") {
			vnx::from_string(_entry.second, fetch_duration_us);
		} else if(_entry.first == "last_modified") {
			vnx::from_string(_entry.second, last_modified);
		} else if(_entry.first == "url") {
			vnx::from_string(_entry.second, url);
		}
	}
}

vnx::Object Response::to_object() const {
	vnx::Object _object;
	_object["url"] = url;
	_object["date"] = date;
	_object["last_modified"] = last_modified;
	_object["fetch_duration_us"] = fetch_duration_us;
	return _object;
}

void Response::from_object(const vnx::Object& _object) {
	for(const auto& _entry : _object.field) {
		if(_entry.first == "date") {
			_entry.second.to(date);
		} else if(_entry.first == "fetch_duration_us") {
			_entry.second.to(fetch_duration_us);
		} else if(_entry.first == "last_modified") {
			_entry.second.to(last_modified);
		} else if(_entry.first == "url") {
			_entry.second.to(url);
		}
	}
}

/// \private
std::ostream& operator<<(std::ostream& _out, const Response& _value) {
	_value.write(_out);
	return _out;
}

/// \private
std::istream& operator>>(std::istream& _in, Response& _value) {
	_value.read(_in);
	return _in;
}

const vnx::TypeCode* Response::static_get_type_code() {
	const vnx::TypeCode* type_code = vnx::get_type_code(VNX_TYPE_HASH);
	if(!type_code) {
		type_code = vnx::register_type_code(static_create_type_code());
	}
	return type_code;
}

std::shared_ptr<vnx::TypeCode> Response::static_create_type_code() {
	std::shared_ptr<vnx::TypeCode> type_code = std::make_shared<vnx::TypeCode>();
	type_code->name = "vnx.search.Response";
	type_code->type_hash = vnx::Hash64(0xcf371c90803a778cull);
	type_code->code_hash = vnx::Hash64(0xb26d330e6838503aull);
	type_code->is_native = true;
	type_code->is_class = true;
	type_code->create_value = []() -> std::shared_ptr<vnx::Value> { return std::make_shared<Response>(); };
	type_code->fields.resize(4);
	{
		vnx::TypeField& field = type_code->fields[0];
		field.is_extended = true;
		field.name = "url";
		field.code = {12, 5};
	}
	{
		vnx::TypeField& field = type_code->fields[1];
		field.name = "date";
		field.code = {8};
	}
	{
		vnx::TypeField& field = type_code->fields[2];
		field.name = "last_modified";
		field.code = {8};
	}
	{
		vnx::TypeField& field = type_code->fields[3];
		field.name = "fetch_duration_us";
		field.code = {8};
	}
	type_code->build();
	return type_code;
}


} // namespace vnx
} // namespace search


namespace vnx {

void read(TypeInput& in, ::vnx::search::Response& value, const TypeCode* type_code, const uint16_t* code) {
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
			const vnx::TypeField* const _field = type_code->field_map[1];
			if(_field) {
				vnx::read_value(_buf + _field->offset, value.date, _field->code.data());
			}
		}
		{
			const vnx::TypeField* const _field = type_code->field_map[2];
			if(_field) {
				vnx::read_value(_buf + _field->offset, value.last_modified, _field->code.data());
			}
		}
		{
			const vnx::TypeField* const _field = type_code->field_map[3];
			if(_field) {
				vnx::read_value(_buf + _field->offset, value.fetch_duration_us, _field->code.data());
			}
		}
	}
	for(const vnx::TypeField* _field : type_code->ext_fields) {
		switch(_field->native_index) {
			case 0: vnx::read(in, value.url, type_code, _field->code.data()); break;
			default: vnx::skip(in, type_code, _field->code.data());
		}
	}
}

void write(TypeOutput& out, const ::vnx::search::Response& value, const TypeCode* type_code, const uint16_t* code) {
	if(!type_code || (code && code[0] == CODE_ANY)) {
		type_code = vnx::search::vnx_native_type_code_Response;
		out.write_type_code(type_code);
		vnx::write_class_header<::vnx::search::Response>(out);
	}
	if(code && code[0] == CODE_STRUCT) {
		type_code = type_code->depends[code[1]];
	}
	char* const _buf = out.write(24);
	vnx::write_value(_buf + 0, value.date);
	vnx::write_value(_buf + 8, value.last_modified);
	vnx::write_value(_buf + 16, value.fetch_duration_us);
	vnx::write(out, value.url, type_code, type_code->fields[0].code.data());
}

void read(std::istream& in, ::vnx::search::Response& value) {
	value.read(in);
}

void write(std::ostream& out, const ::vnx::search::Response& value) {
	value.write(out);
}

void accept(Visitor& visitor, const ::vnx::search::Response& value) {
	value.accept(visitor);
}

} // vnx
