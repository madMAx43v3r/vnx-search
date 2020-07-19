
// AUTO GENERATED by vnxcppcodegen

#include <vnx/search/package.hxx>
#include <vnx/search/HttpResponse.hxx>
#include <vnx/Buffer.hpp>
#include <vnx/search/Response.hxx>

#include <vnx/vnx.h>


namespace vnx {
namespace search {


const vnx::Hash64 HttpResponse::VNX_TYPE_HASH(0xd6552db423d70e21ull);
const vnx::Hash64 HttpResponse::VNX_CODE_HASH(0xc11213aa81fb7f5full);

vnx::Hash64 HttpResponse::get_type_hash() const {
	return VNX_TYPE_HASH;
}

const char* HttpResponse::get_type_name() const {
	return "vnx.search.HttpResponse";
}
const vnx::TypeCode* HttpResponse::get_type_code() const {
	return vnx::search::vnx_native_type_code_HttpResponse;
}

std::shared_ptr<HttpResponse> HttpResponse::create() {
	return std::make_shared<HttpResponse>();
}

std::shared_ptr<vnx::Value> HttpResponse::clone() const {
	return std::make_shared<HttpResponse>(*this);
}

void HttpResponse::read(vnx::TypeInput& _in, const vnx::TypeCode* _type_code, const uint16_t* _code) {
	vnx::read(_in, *this, _type_code, _code);
}

void HttpResponse::write(vnx::TypeOutput& _out, const vnx::TypeCode* _type_code, const uint16_t* _code) const {
	vnx::write(_out, *this, _type_code, _code);
}

void HttpResponse::accept(vnx::Visitor& _visitor) const {
	const vnx::TypeCode* _type_code = vnx::search::vnx_native_type_code_HttpResponse;
	_visitor.type_begin(*_type_code);
	_visitor.type_field(_type_code->fields[0], 0); vnx::accept(_visitor, url);
	_visitor.type_field(_type_code->fields[1], 1); vnx::accept(_visitor, date);
	_visitor.type_field(_type_code->fields[2], 2); vnx::accept(_visitor, last_modified);
	_visitor.type_field(_type_code->fields[3], 3); vnx::accept(_visitor, fetch_duration_us);
	_visitor.type_field(_type_code->fields[4], 4); vnx::accept(_visitor, status);
	_visitor.type_field(_type_code->fields[5], 5); vnx::accept(_visitor, content_type);
	_visitor.type_field(_type_code->fields[6], 6); vnx::accept(_visitor, content_charset);
	_visitor.type_field(_type_code->fields[7], 7); vnx::accept(_visitor, payload);
	_visitor.type_end(*_type_code);
}

void HttpResponse::write(std::ostream& _out) const {
	_out << "{\"__type\": \"vnx.search.HttpResponse\"";
	_out << ", \"url\": "; vnx::write(_out, url);
	_out << ", \"date\": "; vnx::write(_out, date);
	_out << ", \"last_modified\": "; vnx::write(_out, last_modified);
	_out << ", \"fetch_duration_us\": "; vnx::write(_out, fetch_duration_us);
	_out << ", \"status\": "; vnx::write(_out, status);
	_out << ", \"content_type\": "; vnx::write(_out, content_type);
	_out << ", \"content_charset\": "; vnx::write(_out, content_charset);
	_out << ", \"payload\": "; vnx::write(_out, payload);
	_out << "}";
}

void HttpResponse::read(std::istream& _in) {
	std::map<std::string, std::string> _object;
	vnx::read_object(_in, _object);
	for(const auto& _entry : _object) {
		if(_entry.first == "content_charset") {
			vnx::from_string(_entry.second, content_charset);
		} else if(_entry.first == "content_type") {
			vnx::from_string(_entry.second, content_type);
		} else if(_entry.first == "date") {
			vnx::from_string(_entry.second, date);
		} else if(_entry.first == "fetch_duration_us") {
			vnx::from_string(_entry.second, fetch_duration_us);
		} else if(_entry.first == "last_modified") {
			vnx::from_string(_entry.second, last_modified);
		} else if(_entry.first == "payload") {
			vnx::from_string(_entry.second, payload);
		} else if(_entry.first == "status") {
			vnx::from_string(_entry.second, status);
		} else if(_entry.first == "url") {
			vnx::from_string(_entry.second, url);
		}
	}
}

vnx::Object HttpResponse::to_object() const {
	vnx::Object _object;
	_object["__type"] = "vnx.search.HttpResponse";
	_object["url"] = url;
	_object["date"] = date;
	_object["last_modified"] = last_modified;
	_object["fetch_duration_us"] = fetch_duration_us;
	_object["status"] = status;
	_object["content_type"] = content_type;
	_object["content_charset"] = content_charset;
	_object["payload"] = payload;
	return _object;
}

void HttpResponse::from_object(const vnx::Object& _object) {
	for(const auto& _entry : _object.field) {
		if(_entry.first == "content_charset") {
			_entry.second.to(content_charset);
		} else if(_entry.first == "content_type") {
			_entry.second.to(content_type);
		} else if(_entry.first == "date") {
			_entry.second.to(date);
		} else if(_entry.first == "fetch_duration_us") {
			_entry.second.to(fetch_duration_us);
		} else if(_entry.first == "last_modified") {
			_entry.second.to(last_modified);
		} else if(_entry.first == "payload") {
			_entry.second.to(payload);
		} else if(_entry.first == "status") {
			_entry.second.to(status);
		} else if(_entry.first == "url") {
			_entry.second.to(url);
		}
	}
}

/// \private
std::ostream& operator<<(std::ostream& _out, const HttpResponse& _value) {
	_value.write(_out);
	return _out;
}

/// \private
std::istream& operator>>(std::istream& _in, HttpResponse& _value) {
	_value.read(_in);
	return _in;
}

const vnx::TypeCode* HttpResponse::static_get_type_code() {
	const vnx::TypeCode* type_code = vnx::get_type_code(VNX_TYPE_HASH);
	if(!type_code) {
		type_code = vnx::register_type_code(static_create_type_code());
	}
	return type_code;
}

std::shared_ptr<vnx::TypeCode> HttpResponse::static_create_type_code() {
	std::shared_ptr<vnx::TypeCode> type_code = std::make_shared<vnx::TypeCode>();
	type_code->name = "vnx.search.HttpResponse";
	type_code->type_hash = vnx::Hash64(0xd6552db423d70e21ull);
	type_code->code_hash = vnx::Hash64(0xc11213aa81fb7f5full);
	type_code->is_native = true;
	type_code->is_class = true;
	type_code->parents.resize(1);
	type_code->parents[0] = ::vnx::search::Response::static_get_type_code();
	type_code->create_value = []() -> std::shared_ptr<vnx::Value> { return std::make_shared<HttpResponse>(); };
	type_code->fields.resize(8);
	{
		vnx::TypeField& field = type_code->fields[0];
		field.is_extended = true;
		field.name = "url";
		field.code = {32};
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
	{
		vnx::TypeField& field = type_code->fields[4];
		field.name = "status";
		field.value = vnx::to_string(-1);
		field.code = {7};
	}
	{
		vnx::TypeField& field = type_code->fields[5];
		field.is_extended = true;
		field.name = "content_type";
		field.code = {32};
	}
	{
		vnx::TypeField& field = type_code->fields[6];
		field.is_extended = true;
		field.name = "content_charset";
		field.code = {32};
	}
	{
		vnx::TypeField& field = type_code->fields[7];
		field.is_extended = true;
		field.name = "payload";
		field.code = {12, 1};
	}
	type_code->build();
	return type_code;
}


} // namespace vnx
} // namespace search


namespace vnx {

void read(TypeInput& in, ::vnx::search::HttpResponse& value, const TypeCode* type_code, const uint16_t* code) {
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
		{
			const vnx::TypeField* const _field = type_code->field_map[4];
			if(_field) {
				vnx::read_value(_buf + _field->offset, value.status, _field->code.data());
			}
		}
	}
	for(const vnx::TypeField* _field : type_code->ext_fields) {
		switch(_field->native_index) {
			case 0: vnx::read(in, value.url, type_code, _field->code.data()); break;
			case 5: vnx::read(in, value.content_type, type_code, _field->code.data()); break;
			case 6: vnx::read(in, value.content_charset, type_code, _field->code.data()); break;
			case 7: vnx::read(in, value.payload, type_code, _field->code.data()); break;
			default: vnx::skip(in, type_code, _field->code.data());
		}
	}
}

void write(TypeOutput& out, const ::vnx::search::HttpResponse& value, const TypeCode* type_code, const uint16_t* code) {
	if(!type_code || (code && code[0] == CODE_ANY)) {
		type_code = vnx::search::vnx_native_type_code_HttpResponse;
		out.write_type_code(type_code);
		vnx::write_class_header<::vnx::search::HttpResponse>(out);
	}
	if(code && code[0] == CODE_STRUCT) {
		type_code = type_code->depends[code[1]];
	}
	char* const _buf = out.write(28);
	vnx::write_value(_buf + 0, value.date);
	vnx::write_value(_buf + 8, value.last_modified);
	vnx::write_value(_buf + 16, value.fetch_duration_us);
	vnx::write_value(_buf + 24, value.status);
	vnx::write(out, value.url, type_code, type_code->fields[0].code.data());
	vnx::write(out, value.content_type, type_code, type_code->fields[5].code.data());
	vnx::write(out, value.content_charset, type_code, type_code->fields[6].code.data());
	vnx::write(out, value.payload, type_code, type_code->fields[7].code.data());
}

void read(std::istream& in, ::vnx::search::HttpResponse& value) {
	value.read(in);
}

void write(std::ostream& out, const ::vnx::search::HttpResponse& value) {
	value.write(out);
}

void accept(Visitor& visitor, const ::vnx::search::HttpResponse& value) {
	value.accept(visitor);
}

} // vnx
