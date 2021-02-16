
// AUTO GENERATED by vnxcppcodegen

#include <vnx/search/package.hxx>
#include <vnx/search/TextResponse.hxx>
#include <vnx/search/Response.hxx>
#include <vnx/search/image_link_t.hxx>
#include <vnx/search/page_link_t.hxx>

#include <vnx/vnx.h>


namespace vnx {
namespace search {


const vnx::Hash64 TextResponse::VNX_TYPE_HASH(0x7cee1cd5b88ec569ull);
const vnx::Hash64 TextResponse::VNX_CODE_HASH(0xf287e8287e1259feull);

vnx::Hash64 TextResponse::get_type_hash() const {
	return VNX_TYPE_HASH;
}

std::string TextResponse::get_type_name() const {
	return "vnx.search.TextResponse";
}

const vnx::TypeCode* TextResponse::get_type_code() const {
	return vnx::search::vnx_native_type_code_TextResponse;
}

std::shared_ptr<TextResponse> TextResponse::create() {
	return std::make_shared<TextResponse>();
}

std::shared_ptr<vnx::Value> TextResponse::clone() const {
	return std::make_shared<TextResponse>(*this);
}

void TextResponse::read(vnx::TypeInput& _in, const vnx::TypeCode* _type_code, const uint16_t* _code) {
	vnx::read(_in, *this, _type_code, _code);
}

void TextResponse::write(vnx::TypeOutput& _out, const vnx::TypeCode* _type_code, const uint16_t* _code) const {
	vnx::write(_out, *this, _type_code, _code);
}

void TextResponse::accept(vnx::Visitor& _visitor) const {
	const vnx::TypeCode* _type_code = vnx::search::vnx_native_type_code_TextResponse;
	_visitor.type_begin(*_type_code);
	_visitor.type_field(_type_code->fields[0], 0); vnx::accept(_visitor, url);
	_visitor.type_field(_type_code->fields[1], 1); vnx::accept(_visitor, date);
	_visitor.type_field(_type_code->fields[2], 2); vnx::accept(_visitor, last_modified);
	_visitor.type_field(_type_code->fields[3], 3); vnx::accept(_visitor, fetch_duration_us);
	_visitor.type_field(_type_code->fields[4], 4); vnx::accept(_visitor, title);
	_visitor.type_field(_type_code->fields[5], 5); vnx::accept(_visitor, text);
	_visitor.type_field(_type_code->fields[6], 6); vnx::accept(_visitor, base_url);
	_visitor.type_field(_type_code->fields[7], 7); vnx::accept(_visitor, links);
	_visitor.type_field(_type_code->fields[8], 8); vnx::accept(_visitor, images);
	_visitor.type_field(_type_code->fields[9], 9); vnx::accept(_visitor, resources);
	_visitor.type_end(*_type_code);
}

void TextResponse::write(std::ostream& _out) const {
	_out << "{\"__type\": \"vnx.search.TextResponse\"";
	_out << ", \"url\": "; vnx::write(_out, url);
	_out << ", \"date\": "; vnx::write(_out, date);
	_out << ", \"last_modified\": "; vnx::write(_out, last_modified);
	_out << ", \"fetch_duration_us\": "; vnx::write(_out, fetch_duration_us);
	_out << ", \"title\": "; vnx::write(_out, title);
	_out << ", \"text\": "; vnx::write(_out, text);
	_out << ", \"base_url\": "; vnx::write(_out, base_url);
	_out << ", \"links\": "; vnx::write(_out, links);
	_out << ", \"images\": "; vnx::write(_out, images);
	_out << ", \"resources\": "; vnx::write(_out, resources);
	_out << "}";
}

void TextResponse::read(std::istream& _in) {
	if(auto _json = vnx::read_json(_in)) {
		from_object(_json->to_object());
	}
}

vnx::Object TextResponse::to_object() const {
	vnx::Object _object;
	_object["__type"] = "vnx.search.TextResponse";
	_object["url"] = url;
	_object["date"] = date;
	_object["last_modified"] = last_modified;
	_object["fetch_duration_us"] = fetch_duration_us;
	_object["title"] = title;
	_object["text"] = text;
	_object["base_url"] = base_url;
	_object["links"] = links;
	_object["images"] = images;
	_object["resources"] = resources;
	return _object;
}

void TextResponse::from_object(const vnx::Object& _object) {
	for(const auto& _entry : _object.field) {
		if(_entry.first == "base_url") {
			_entry.second.to(base_url);
		} else if(_entry.first == "date") {
			_entry.second.to(date);
		} else if(_entry.first == "fetch_duration_us") {
			_entry.second.to(fetch_duration_us);
		} else if(_entry.first == "images") {
			_entry.second.to(images);
		} else if(_entry.first == "last_modified") {
			_entry.second.to(last_modified);
		} else if(_entry.first == "links") {
			_entry.second.to(links);
		} else if(_entry.first == "resources") {
			_entry.second.to(resources);
		} else if(_entry.first == "text") {
			_entry.second.to(text);
		} else if(_entry.first == "title") {
			_entry.second.to(title);
		} else if(_entry.first == "url") {
			_entry.second.to(url);
		}
	}
}

vnx::Variant TextResponse::get_field(const std::string& _name) const {
	if(_name == "url") {
		return vnx::Variant(url);
	}
	if(_name == "date") {
		return vnx::Variant(date);
	}
	if(_name == "last_modified") {
		return vnx::Variant(last_modified);
	}
	if(_name == "fetch_duration_us") {
		return vnx::Variant(fetch_duration_us);
	}
	if(_name == "title") {
		return vnx::Variant(title);
	}
	if(_name == "text") {
		return vnx::Variant(text);
	}
	if(_name == "base_url") {
		return vnx::Variant(base_url);
	}
	if(_name == "links") {
		return vnx::Variant(links);
	}
	if(_name == "images") {
		return vnx::Variant(images);
	}
	if(_name == "resources") {
		return vnx::Variant(resources);
	}
	return vnx::Variant();
}

void TextResponse::set_field(const std::string& _name, const vnx::Variant& _value) {
	if(_name == "url") {
		_value.to(url);
	} else if(_name == "date") {
		_value.to(date);
	} else if(_name == "last_modified") {
		_value.to(last_modified);
	} else if(_name == "fetch_duration_us") {
		_value.to(fetch_duration_us);
	} else if(_name == "title") {
		_value.to(title);
	} else if(_name == "text") {
		_value.to(text);
	} else if(_name == "base_url") {
		_value.to(base_url);
	} else if(_name == "links") {
		_value.to(links);
	} else if(_name == "images") {
		_value.to(images);
	} else if(_name == "resources") {
		_value.to(resources);
	} else {
		throw std::logic_error("no such field: '" + _name + "'");
	}
}

/// \private
std::ostream& operator<<(std::ostream& _out, const TextResponse& _value) {
	_value.write(_out);
	return _out;
}

/// \private
std::istream& operator>>(std::istream& _in, TextResponse& _value) {
	_value.read(_in);
	return _in;
}

const vnx::TypeCode* TextResponse::static_get_type_code() {
	const vnx::TypeCode* type_code = vnx::get_type_code(VNX_TYPE_HASH);
	if(!type_code) {
		type_code = vnx::register_type_code(static_create_type_code());
	}
	return type_code;
}

std::shared_ptr<vnx::TypeCode> TextResponse::static_create_type_code() {
	auto type_code = std::make_shared<vnx::TypeCode>();
	type_code->name = "vnx.search.TextResponse";
	type_code->type_hash = vnx::Hash64(0x7cee1cd5b88ec569ull);
	type_code->code_hash = vnx::Hash64(0xf287e8287e1259feull);
	type_code->is_native = true;
	type_code->is_class = true;
	type_code->native_size = sizeof(::vnx::search::TextResponse);
	type_code->parents.resize(1);
	type_code->parents[0] = ::vnx::search::Response::static_get_type_code();
	type_code->create_value = []() -> std::shared_ptr<vnx::Value> { return std::make_shared<TextResponse>(); };
	type_code->depends.resize(2);
	type_code->depends[0] = ::vnx::search::page_link_t::static_get_type_code();
	type_code->depends[1] = ::vnx::search::image_link_t::static_get_type_code();
	type_code->fields.resize(10);
	{
		auto& field = type_code->fields[0];
		field.is_extended = true;
		field.name = "url";
		field.code = {32};
	}
	{
		auto& field = type_code->fields[1];
		field.data_size = 8;
		field.name = "date";
		field.code = {8};
	}
	{
		auto& field = type_code->fields[2];
		field.data_size = 8;
		field.name = "last_modified";
		field.code = {8};
	}
	{
		auto& field = type_code->fields[3];
		field.data_size = 8;
		field.name = "fetch_duration_us";
		field.code = {8};
	}
	{
		auto& field = type_code->fields[4];
		field.is_extended = true;
		field.name = "title";
		field.code = {32};
	}
	{
		auto& field = type_code->fields[5];
		field.is_extended = true;
		field.name = "text";
		field.code = {32};
	}
	{
		auto& field = type_code->fields[6];
		field.is_extended = true;
		field.name = "base_url";
		field.code = {32};
	}
	{
		auto& field = type_code->fields[7];
		field.is_extended = true;
		field.name = "links";
		field.code = {12, 19, 0};
	}
	{
		auto& field = type_code->fields[8];
		field.is_extended = true;
		field.name = "images";
		field.code = {12, 19, 1};
	}
	{
		auto& field = type_code->fields[9];
		field.is_extended = true;
		field.name = "resources";
		field.code = {12, 32};
	}
	type_code->build();
	return type_code;
}


} // namespace vnx
} // namespace search


namespace vnx {

void read(TypeInput& in, ::vnx::search::TextResponse& value, const TypeCode* type_code, const uint16_t* code) {
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
		if(const auto* const _field = type_code->field_map[1]) {
			vnx::read_value(_buf + _field->offset, value.date, _field->code.data());
		}
		if(const auto* const _field = type_code->field_map[2]) {
			vnx::read_value(_buf + _field->offset, value.last_modified, _field->code.data());
		}
		if(const auto* const _field = type_code->field_map[3]) {
			vnx::read_value(_buf + _field->offset, value.fetch_duration_us, _field->code.data());
		}
	}
	for(const auto* _field : type_code->ext_fields) {
		switch(_field->native_index) {
			case 0: vnx::read(in, value.url, type_code, _field->code.data()); break;
			case 4: vnx::read(in, value.title, type_code, _field->code.data()); break;
			case 5: vnx::read(in, value.text, type_code, _field->code.data()); break;
			case 6: vnx::read(in, value.base_url, type_code, _field->code.data()); break;
			case 7: vnx::read(in, value.links, type_code, _field->code.data()); break;
			case 8: vnx::read(in, value.images, type_code, _field->code.data()); break;
			case 9: vnx::read(in, value.resources, type_code, _field->code.data()); break;
			default: vnx::skip(in, type_code, _field->code.data());
		}
	}
}

void write(TypeOutput& out, const ::vnx::search::TextResponse& value, const TypeCode* type_code, const uint16_t* code) {
	if(code && code[0] == CODE_OBJECT) {
		vnx::write(out, value.to_object(), nullptr, code);
		return;
	}
	if(!type_code || (code && code[0] == CODE_ANY)) {
		type_code = vnx::search::vnx_native_type_code_TextResponse;
		out.write_type_code(type_code);
		vnx::write_class_header<::vnx::search::TextResponse>(out);
	}
	else if(code && code[0] == CODE_STRUCT) {
		type_code = type_code->depends[code[1]];
	}
	char* const _buf = out.write(24);
	vnx::write_value(_buf + 0, value.date);
	vnx::write_value(_buf + 8, value.last_modified);
	vnx::write_value(_buf + 16, value.fetch_duration_us);
	vnx::write(out, value.url, type_code, type_code->fields[0].code.data());
	vnx::write(out, value.title, type_code, type_code->fields[4].code.data());
	vnx::write(out, value.text, type_code, type_code->fields[5].code.data());
	vnx::write(out, value.base_url, type_code, type_code->fields[6].code.data());
	vnx::write(out, value.links, type_code, type_code->fields[7].code.data());
	vnx::write(out, value.images, type_code, type_code->fields[8].code.data());
	vnx::write(out, value.resources, type_code, type_code->fields[9].code.data());
}

void read(std::istream& in, ::vnx::search::TextResponse& value) {
	value.read(in);
}

void write(std::ostream& out, const ::vnx::search::TextResponse& value) {
	value.write(out);
}

void accept(Visitor& visitor, const ::vnx::search::TextResponse& value) {
	value.accept(visitor);
}

} // vnx
