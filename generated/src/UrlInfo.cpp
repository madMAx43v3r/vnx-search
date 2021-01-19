
// AUTO GENERATED by vnxcppcodegen

#include <vnx/search/package.hxx>
#include <vnx/search/UrlInfo.hxx>
#include <vnx/Value.h>

#include <vnx/vnx.h>


namespace vnx {
namespace search {


const vnx::Hash64 UrlInfo::VNX_TYPE_HASH(0xd69101803f781acfull);
const vnx::Hash64 UrlInfo::VNX_CODE_HASH(0xabb80d4ce8ee23c6ull);

vnx::Hash64 UrlInfo::get_type_hash() const {
	return VNX_TYPE_HASH;
}

std::string UrlInfo::get_type_name() const {
	return "vnx.search.UrlInfo";
}

const vnx::TypeCode* UrlInfo::get_type_code() const {
	return vnx::search::vnx_native_type_code_UrlInfo;
}

std::shared_ptr<UrlInfo> UrlInfo::create() {
	return std::make_shared<UrlInfo>();
}

std::shared_ptr<vnx::Value> UrlInfo::clone() const {
	return std::make_shared<UrlInfo>(*this);
}

void UrlInfo::read(vnx::TypeInput& _in, const vnx::TypeCode* _type_code, const uint16_t* _code) {
	vnx::read(_in, *this, _type_code, _code);
}

void UrlInfo::write(vnx::TypeOutput& _out, const vnx::TypeCode* _type_code, const uint16_t* _code) const {
	vnx::write(_out, *this, _type_code, _code);
}

void UrlInfo::accept(vnx::Visitor& _visitor) const {
	const vnx::TypeCode* _type_code = vnx::search::vnx_native_type_code_UrlInfo;
	_visitor.type_begin(*_type_code);
	_visitor.type_field(_type_code->fields[0], 0); vnx::accept(_visitor, redirect);
	_visitor.type_field(_type_code->fields[1], 1); vnx::accept(_visitor, content_type);
	_visitor.type_field(_type_code->fields[2], 2); vnx::accept(_visitor, last_fetched);
	_visitor.type_field(_type_code->fields[3], 3); vnx::accept(_visitor, last_modified);
	_visitor.type_field(_type_code->fields[4], 4); vnx::accept(_visitor, fetch_duration_us);
	_visitor.type_field(_type_code->fields[5], 5); vnx::accept(_visitor, num_bytes);
	_visitor.type_field(_type_code->fields[6], 6); vnx::accept(_visitor, http_status);
	_visitor.type_field(_type_code->fields[7], 7); vnx::accept(_visitor, curl_status);
	_visitor.type_field(_type_code->fields[8], 8); vnx::accept(_visitor, is_fail);
	_visitor.type_end(*_type_code);
}

void UrlInfo::write(std::ostream& _out) const {
	_out << "{\"__type\": \"vnx.search.UrlInfo\"";
	_out << ", \"redirect\": "; vnx::write(_out, redirect);
	_out << ", \"content_type\": "; vnx::write(_out, content_type);
	_out << ", \"last_fetched\": "; vnx::write(_out, last_fetched);
	_out << ", \"last_modified\": "; vnx::write(_out, last_modified);
	_out << ", \"fetch_duration_us\": "; vnx::write(_out, fetch_duration_us);
	_out << ", \"num_bytes\": "; vnx::write(_out, num_bytes);
	_out << ", \"http_status\": "; vnx::write(_out, http_status);
	_out << ", \"curl_status\": "; vnx::write(_out, curl_status);
	_out << ", \"is_fail\": "; vnx::write(_out, is_fail);
	_out << "}";
}

void UrlInfo::read(std::istream& _in) {
	if(auto _json = vnx::read_json(_in)) {
		from_object(_json->to_object());
	}
}

vnx::Object UrlInfo::to_object() const {
	vnx::Object _object;
	_object["__type"] = "vnx.search.UrlInfo";
	_object["redirect"] = redirect;
	_object["content_type"] = content_type;
	_object["last_fetched"] = last_fetched;
	_object["last_modified"] = last_modified;
	_object["fetch_duration_us"] = fetch_duration_us;
	_object["num_bytes"] = num_bytes;
	_object["http_status"] = http_status;
	_object["curl_status"] = curl_status;
	_object["is_fail"] = is_fail;
	return _object;
}

void UrlInfo::from_object(const vnx::Object& _object) {
	for(const auto& _entry : _object.field) {
		if(_entry.first == "content_type") {
			_entry.second.to(content_type);
		} else if(_entry.first == "curl_status") {
			_entry.second.to(curl_status);
		} else if(_entry.first == "fetch_duration_us") {
			_entry.second.to(fetch_duration_us);
		} else if(_entry.first == "http_status") {
			_entry.second.to(http_status);
		} else if(_entry.first == "is_fail") {
			_entry.second.to(is_fail);
		} else if(_entry.first == "last_fetched") {
			_entry.second.to(last_fetched);
		} else if(_entry.first == "last_modified") {
			_entry.second.to(last_modified);
		} else if(_entry.first == "num_bytes") {
			_entry.second.to(num_bytes);
		} else if(_entry.first == "redirect") {
			_entry.second.to(redirect);
		}
	}
}

vnx::Variant UrlInfo::get_field(const std::string& _name) const {
	if(_name == "redirect") {
		return vnx::Variant(redirect);
	}
	if(_name == "content_type") {
		return vnx::Variant(content_type);
	}
	if(_name == "last_fetched") {
		return vnx::Variant(last_fetched);
	}
	if(_name == "last_modified") {
		return vnx::Variant(last_modified);
	}
	if(_name == "fetch_duration_us") {
		return vnx::Variant(fetch_duration_us);
	}
	if(_name == "num_bytes") {
		return vnx::Variant(num_bytes);
	}
	if(_name == "http_status") {
		return vnx::Variant(http_status);
	}
	if(_name == "curl_status") {
		return vnx::Variant(curl_status);
	}
	if(_name == "is_fail") {
		return vnx::Variant(is_fail);
	}
	return vnx::Variant();
}

void UrlInfo::set_field(const std::string& _name, const vnx::Variant& _value) {
	if(_name == "redirect") {
		_value.to(redirect);
	} else if(_name == "content_type") {
		_value.to(content_type);
	} else if(_name == "last_fetched") {
		_value.to(last_fetched);
	} else if(_name == "last_modified") {
		_value.to(last_modified);
	} else if(_name == "fetch_duration_us") {
		_value.to(fetch_duration_us);
	} else if(_name == "num_bytes") {
		_value.to(num_bytes);
	} else if(_name == "http_status") {
		_value.to(http_status);
	} else if(_name == "curl_status") {
		_value.to(curl_status);
	} else if(_name == "is_fail") {
		_value.to(is_fail);
	} else {
		throw std::logic_error("no such field: '" + _name + "'");
	}
}

/// \private
std::ostream& operator<<(std::ostream& _out, const UrlInfo& _value) {
	_value.write(_out);
	return _out;
}

/// \private
std::istream& operator>>(std::istream& _in, UrlInfo& _value) {
	_value.read(_in);
	return _in;
}

const vnx::TypeCode* UrlInfo::static_get_type_code() {
	const vnx::TypeCode* type_code = vnx::get_type_code(VNX_TYPE_HASH);
	if(!type_code) {
		type_code = vnx::register_type_code(static_create_type_code());
	}
	return type_code;
}

std::shared_ptr<vnx::TypeCode> UrlInfo::static_create_type_code() {
	std::shared_ptr<vnx::TypeCode> type_code = std::make_shared<vnx::TypeCode>();
	type_code->name = "vnx.search.UrlInfo";
	type_code->type_hash = vnx::Hash64(0xd69101803f781acfull);
	type_code->code_hash = vnx::Hash64(0xabb80d4ce8ee23c6ull);
	type_code->is_native = true;
	type_code->is_class = true;
	type_code->create_value = []() -> std::shared_ptr<vnx::Value> { return std::make_shared<UrlInfo>(); };
	type_code->fields.resize(9);
	{
		vnx::TypeField& field = type_code->fields[0];
		field.is_extended = true;
		field.name = "redirect";
		field.code = {32};
	}
	{
		vnx::TypeField& field = type_code->fields[1];
		field.is_extended = true;
		field.name = "content_type";
		field.code = {32};
	}
	{
		vnx::TypeField& field = type_code->fields[2];
		field.name = "last_fetched";
		field.code = {8};
	}
	{
		vnx::TypeField& field = type_code->fields[3];
		field.name = "last_modified";
		field.code = {8};
	}
	{
		vnx::TypeField& field = type_code->fields[4];
		field.name = "fetch_duration_us";
		field.code = {7};
	}
	{
		vnx::TypeField& field = type_code->fields[5];
		field.name = "num_bytes";
		field.value = vnx::to_string(-1);
		field.code = {7};
	}
	{
		vnx::TypeField& field = type_code->fields[6];
		field.name = "http_status";
		field.value = vnx::to_string(-1);
		field.code = {7};
	}
	{
		vnx::TypeField& field = type_code->fields[7];
		field.name = "curl_status";
		field.value = vnx::to_string(-1);
		field.code = {7};
	}
	{
		vnx::TypeField& field = type_code->fields[8];
		field.name = "is_fail";
		field.code = {31};
	}
	type_code->build();
	return type_code;
}


} // namespace vnx
} // namespace search


namespace vnx {

void read(TypeInput& in, ::vnx::search::UrlInfo& value, const TypeCode* type_code, const uint16_t* code) {
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
		{
			const vnx::TypeField* const _field = type_code->field_map[2];
			if(_field) {
				vnx::read_value(_buf + _field->offset, value.last_fetched, _field->code.data());
			}
		}
		{
			const vnx::TypeField* const _field = type_code->field_map[3];
			if(_field) {
				vnx::read_value(_buf + _field->offset, value.last_modified, _field->code.data());
			}
		}
		{
			const vnx::TypeField* const _field = type_code->field_map[4];
			if(_field) {
				vnx::read_value(_buf + _field->offset, value.fetch_duration_us, _field->code.data());
			}
		}
		{
			const vnx::TypeField* const _field = type_code->field_map[5];
			if(_field) {
				vnx::read_value(_buf + _field->offset, value.num_bytes, _field->code.data());
			}
		}
		{
			const vnx::TypeField* const _field = type_code->field_map[6];
			if(_field) {
				vnx::read_value(_buf + _field->offset, value.http_status, _field->code.data());
			}
		}
		{
			const vnx::TypeField* const _field = type_code->field_map[7];
			if(_field) {
				vnx::read_value(_buf + _field->offset, value.curl_status, _field->code.data());
			}
		}
		{
			const vnx::TypeField* const _field = type_code->field_map[8];
			if(_field) {
				vnx::read_value(_buf + _field->offset, value.is_fail, _field->code.data());
			}
		}
	}
	for(const vnx::TypeField* _field : type_code->ext_fields) {
		switch(_field->native_index) {
			case 0: vnx::read(in, value.redirect, type_code, _field->code.data()); break;
			case 1: vnx::read(in, value.content_type, type_code, _field->code.data()); break;
			default: vnx::skip(in, type_code, _field->code.data());
		}
	}
}

void write(TypeOutput& out, const ::vnx::search::UrlInfo& value, const TypeCode* type_code, const uint16_t* code) {
	if(code && code[0] == CODE_OBJECT) {
		vnx::write(out, value.to_object(), nullptr, code);
		return;
	}
	if(!type_code || (code && code[0] == CODE_ANY)) {
		type_code = vnx::search::vnx_native_type_code_UrlInfo;
		out.write_type_code(type_code);
		vnx::write_class_header<::vnx::search::UrlInfo>(out);
	}
	else if(code && code[0] == CODE_STRUCT) {
		type_code = type_code->depends[code[1]];
	}
	char* const _buf = out.write(33);
	vnx::write_value(_buf + 0, value.last_fetched);
	vnx::write_value(_buf + 8, value.last_modified);
	vnx::write_value(_buf + 16, value.fetch_duration_us);
	vnx::write_value(_buf + 20, value.num_bytes);
	vnx::write_value(_buf + 24, value.http_status);
	vnx::write_value(_buf + 28, value.curl_status);
	vnx::write_value(_buf + 32, value.is_fail);
	vnx::write(out, value.redirect, type_code, type_code->fields[0].code.data());
	vnx::write(out, value.content_type, type_code, type_code->fields[1].code.data());
}

void read(std::istream& in, ::vnx::search::UrlInfo& value) {
	value.read(in);
}

void write(std::ostream& out, const ::vnx::search::UrlInfo& value) {
	value.write(out);
}

void accept(Visitor& visitor, const ::vnx::search::UrlInfo& value) {
	value.accept(visitor);
}

} // vnx
