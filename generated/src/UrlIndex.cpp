
// AUTO GENERATED by vnxcppcodegen

#include <vnx/search/package.hxx>
#include <vnx/search/UrlIndex.hxx>
#include <vnx/search/UrlInfo.hxx>

#include <vnx/vnx.h>


namespace vnx {
namespace search {


const vnx::Hash64 UrlIndex::VNX_TYPE_HASH(0xab26319d1802d1b6ull);
const vnx::Hash64 UrlIndex::VNX_CODE_HASH(0x93d101eed5982fa7ull);

vnx::Hash64 UrlIndex::get_type_hash() const {
	return VNX_TYPE_HASH;
}

const char* UrlIndex::get_type_name() const {
	return "vnx.search.UrlIndex";
}
const vnx::TypeCode* UrlIndex::get_type_code() const {
	return vnx::search::vnx_native_type_code_UrlIndex;
}

std::shared_ptr<UrlIndex> UrlIndex::create() {
	return std::make_shared<UrlIndex>();
}

std::shared_ptr<vnx::Value> UrlIndex::clone() const {
	return std::make_shared<UrlIndex>(*this);
}

void UrlIndex::read(vnx::TypeInput& _in, const vnx::TypeCode* _type_code, const uint16_t* _code) {
	vnx::read(_in, *this, _type_code, _code);
}

void UrlIndex::write(vnx::TypeOutput& _out, const vnx::TypeCode* _type_code, const uint16_t* _code) const {
	vnx::write(_out, *this, _type_code, _code);
}

void UrlIndex::accept(vnx::Visitor& _visitor) const {
	const vnx::TypeCode* _type_code = vnx::search::vnx_native_type_code_UrlIndex;
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
	_visitor.type_field(_type_code->fields[9], 9); vnx::accept(_visitor, scheme);
	_visitor.type_field(_type_code->fields[10], 10); vnx::accept(_visitor, first_seen);
	_visitor.type_field(_type_code->fields[11], 11); vnx::accept(_visitor, fetch_count);
	_visitor.type_field(_type_code->fields[12], 12); vnx::accept(_visitor, depth);
	_visitor.type_end(*_type_code);
}

void UrlIndex::write(std::ostream& _out) const {
	_out << "{\"__type\": \"vnx.search.UrlIndex\"";
	_out << ", \"redirect\": "; vnx::write(_out, redirect);
	_out << ", \"content_type\": "; vnx::write(_out, content_type);
	_out << ", \"last_fetched\": "; vnx::write(_out, last_fetched);
	_out << ", \"last_modified\": "; vnx::write(_out, last_modified);
	_out << ", \"fetch_duration_us\": "; vnx::write(_out, fetch_duration_us);
	_out << ", \"num_bytes\": "; vnx::write(_out, num_bytes);
	_out << ", \"http_status\": "; vnx::write(_out, http_status);
	_out << ", \"curl_status\": "; vnx::write(_out, curl_status);
	_out << ", \"is_fail\": "; vnx::write(_out, is_fail);
	_out << ", \"scheme\": "; vnx::write(_out, scheme);
	_out << ", \"first_seen\": "; vnx::write(_out, first_seen);
	_out << ", \"fetch_count\": "; vnx::write(_out, fetch_count);
	_out << ", \"depth\": "; vnx::write(_out, depth);
	_out << "}";
}

void UrlIndex::read(std::istream& _in) {
	std::map<std::string, std::string> _object;
	vnx::read_object(_in, _object);
	for(const auto& _entry : _object) {
		if(_entry.first == "content_type") {
			vnx::from_string(_entry.second, content_type);
		} else if(_entry.first == "curl_status") {
			vnx::from_string(_entry.second, curl_status);
		} else if(_entry.first == "depth") {
			vnx::from_string(_entry.second, depth);
		} else if(_entry.first == "fetch_count") {
			vnx::from_string(_entry.second, fetch_count);
		} else if(_entry.first == "fetch_duration_us") {
			vnx::from_string(_entry.second, fetch_duration_us);
		} else if(_entry.first == "first_seen") {
			vnx::from_string(_entry.second, first_seen);
		} else if(_entry.first == "http_status") {
			vnx::from_string(_entry.second, http_status);
		} else if(_entry.first == "is_fail") {
			vnx::from_string(_entry.second, is_fail);
		} else if(_entry.first == "last_fetched") {
			vnx::from_string(_entry.second, last_fetched);
		} else if(_entry.first == "last_modified") {
			vnx::from_string(_entry.second, last_modified);
		} else if(_entry.first == "num_bytes") {
			vnx::from_string(_entry.second, num_bytes);
		} else if(_entry.first == "redirect") {
			vnx::from_string(_entry.second, redirect);
		} else if(_entry.first == "scheme") {
			vnx::from_string(_entry.second, scheme);
		}
	}
}

vnx::Object UrlIndex::to_object() const {
	vnx::Object _object;
	_object["__type"] = "vnx.search.UrlIndex";
	_object["redirect"] = redirect;
	_object["content_type"] = content_type;
	_object["last_fetched"] = last_fetched;
	_object["last_modified"] = last_modified;
	_object["fetch_duration_us"] = fetch_duration_us;
	_object["num_bytes"] = num_bytes;
	_object["http_status"] = http_status;
	_object["curl_status"] = curl_status;
	_object["is_fail"] = is_fail;
	_object["scheme"] = scheme;
	_object["first_seen"] = first_seen;
	_object["fetch_count"] = fetch_count;
	_object["depth"] = depth;
	return _object;
}

void UrlIndex::from_object(const vnx::Object& _object) {
	for(const auto& _entry : _object.field) {
		if(_entry.first == "content_type") {
			_entry.second.to(content_type);
		} else if(_entry.first == "curl_status") {
			_entry.second.to(curl_status);
		} else if(_entry.first == "depth") {
			_entry.second.to(depth);
		} else if(_entry.first == "fetch_count") {
			_entry.second.to(fetch_count);
		} else if(_entry.first == "fetch_duration_us") {
			_entry.second.to(fetch_duration_us);
		} else if(_entry.first == "first_seen") {
			_entry.second.to(first_seen);
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
		} else if(_entry.first == "scheme") {
			_entry.second.to(scheme);
		}
	}
}

/// \private
std::ostream& operator<<(std::ostream& _out, const UrlIndex& _value) {
	_value.write(_out);
	return _out;
}

/// \private
std::istream& operator>>(std::istream& _in, UrlIndex& _value) {
	_value.read(_in);
	return _in;
}

const vnx::TypeCode* UrlIndex::static_get_type_code() {
	const vnx::TypeCode* type_code = vnx::get_type_code(VNX_TYPE_HASH);
	if(!type_code) {
		type_code = vnx::register_type_code(static_create_type_code());
	}
	return type_code;
}

std::shared_ptr<vnx::TypeCode> UrlIndex::static_create_type_code() {
	std::shared_ptr<vnx::TypeCode> type_code = std::make_shared<vnx::TypeCode>();
	type_code->name = "vnx.search.UrlIndex";
	type_code->type_hash = vnx::Hash64(0xab26319d1802d1b6ull);
	type_code->code_hash = vnx::Hash64(0x93d101eed5982fa7ull);
	type_code->is_native = true;
	type_code->is_class = true;
	type_code->parents.resize(1);
	type_code->parents[0] = ::vnx::search::UrlInfo::static_get_type_code();
	type_code->create_value = []() -> std::shared_ptr<vnx::Value> { return std::make_shared<UrlIndex>(); };
	type_code->fields.resize(13);
	{
		vnx::TypeField& field = type_code->fields[0];
		field.is_extended = true;
		field.name = "redirect";
		field.code = {12, 5};
	}
	{
		vnx::TypeField& field = type_code->fields[1];
		field.is_extended = true;
		field.name = "content_type";
		field.code = {12, 5};
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
		field.code = {1};
	}
	{
		vnx::TypeField& field = type_code->fields[9];
		field.is_extended = true;
		field.name = "scheme";
		field.code = {12, 5};
	}
	{
		vnx::TypeField& field = type_code->fields[10];
		field.name = "first_seen";
		field.code = {8};
	}
	{
		vnx::TypeField& field = type_code->fields[11];
		field.name = "fetch_count";
		field.code = {3};
	}
	{
		vnx::TypeField& field = type_code->fields[12];
		field.name = "depth";
		field.value = vnx::to_string(-1);
		field.code = {7};
	}
	type_code->build();
	return type_code;
}


} // namespace vnx
} // namespace search


namespace vnx {

void read(TypeInput& in, ::vnx::search::UrlIndex& value, const TypeCode* type_code, const uint16_t* code) {
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
		{
			const vnx::TypeField* const _field = type_code->field_map[10];
			if(_field) {
				vnx::read_value(_buf + _field->offset, value.first_seen, _field->code.data());
			}
		}
		{
			const vnx::TypeField* const _field = type_code->field_map[11];
			if(_field) {
				vnx::read_value(_buf + _field->offset, value.fetch_count, _field->code.data());
			}
		}
		{
			const vnx::TypeField* const _field = type_code->field_map[12];
			if(_field) {
				vnx::read_value(_buf + _field->offset, value.depth, _field->code.data());
			}
		}
	}
	for(const vnx::TypeField* _field : type_code->ext_fields) {
		switch(_field->native_index) {
			case 0: vnx::read(in, value.redirect, type_code, _field->code.data()); break;
			case 1: vnx::read(in, value.content_type, type_code, _field->code.data()); break;
			case 9: vnx::read(in, value.scheme, type_code, _field->code.data()); break;
			default: vnx::skip(in, type_code, _field->code.data());
		}
	}
}

void write(TypeOutput& out, const ::vnx::search::UrlIndex& value, const TypeCode* type_code, const uint16_t* code) {
	if(!type_code || (code && code[0] == CODE_ANY)) {
		type_code = vnx::search::vnx_native_type_code_UrlIndex;
		out.write_type_code(type_code);
		vnx::write_class_header<::vnx::search::UrlIndex>(out);
	}
	if(code && code[0] == CODE_STRUCT) {
		type_code = type_code->depends[code[1]];
	}
	char* const _buf = out.write(49);
	vnx::write_value(_buf + 0, value.last_fetched);
	vnx::write_value(_buf + 8, value.last_modified);
	vnx::write_value(_buf + 16, value.fetch_duration_us);
	vnx::write_value(_buf + 20, value.num_bytes);
	vnx::write_value(_buf + 24, value.http_status);
	vnx::write_value(_buf + 28, value.curl_status);
	vnx::write_value(_buf + 32, value.is_fail);
	vnx::write_value(_buf + 33, value.first_seen);
	vnx::write_value(_buf + 41, value.fetch_count);
	vnx::write_value(_buf + 45, value.depth);
	vnx::write(out, value.redirect, type_code, type_code->fields[0].code.data());
	vnx::write(out, value.content_type, type_code, type_code->fields[1].code.data());
	vnx::write(out, value.scheme, type_code, type_code->fields[9].code.data());
}

void read(std::istream& in, ::vnx::search::UrlIndex& value) {
	value.read(in);
}

void write(std::ostream& out, const ::vnx::search::UrlIndex& value) {
	value.write(out);
}

void accept(Visitor& visitor, const ::vnx::search::UrlIndex& value) {
	value.accept(visitor);
}

} // vnx
