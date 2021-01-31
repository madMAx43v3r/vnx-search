
// AUTO GENERATED by vnxcppcodegen

#include <vnx/search/package.hxx>
#include <vnx/search/image_link_t.hxx>
#include <vnx/Variant.hpp>
#include <vnx/search/page_link_t.hxx>

#include <vnx/vnx.h>


namespace vnx {
namespace search {


const vnx::Hash64 image_link_t::VNX_TYPE_HASH(0xc6f3540672c0973ull);
const vnx::Hash64 image_link_t::VNX_CODE_HASH(0xe33d3cecd7039328ull);

vnx::Hash64 image_link_t::get_type_hash() const {
	return VNX_TYPE_HASH;
}

std::string image_link_t::get_type_name() const {
	return "vnx.search.image_link_t";
}

const vnx::TypeCode* image_link_t::get_type_code() const {
	return vnx::search::vnx_native_type_code_image_link_t;
}

std::shared_ptr<image_link_t> image_link_t::create() {
	return std::make_shared<image_link_t>();
}

std::shared_ptr<image_link_t> image_link_t::clone() const {
	return std::make_shared<image_link_t>(*this);
}

void image_link_t::read(vnx::TypeInput& _in, const vnx::TypeCode* _type_code, const uint16_t* _code) {
	vnx::read(_in, *this, _type_code, _code);
}

void image_link_t::write(vnx::TypeOutput& _out, const vnx::TypeCode* _type_code, const uint16_t* _code) const {
	vnx::write(_out, *this, _type_code, _code);
}

void image_link_t::accept(vnx::Visitor& _visitor) const {
	const vnx::TypeCode* _type_code = vnx::search::vnx_native_type_code_image_link_t;
	_visitor.type_begin(*_type_code);
	_visitor.type_field(_type_code->fields[0], 0); vnx::accept(_visitor, url);
	_visitor.type_field(_type_code->fields[1], 1); vnx::accept(_visitor, text);
	_visitor.type_field(_type_code->fields[2], 2); vnx::accept(_visitor, words);
	_visitor.type_field(_type_code->fields[3], 3); vnx::accept(_visitor, width);
	_visitor.type_field(_type_code->fields[4], 4); vnx::accept(_visitor, height);
	_visitor.type_end(*_type_code);
}

void image_link_t::write(std::ostream& _out) const {
	_out << "{";
	_out << "\"url\": "; vnx::write(_out, url);
	_out << ", \"text\": "; vnx::write(_out, text);
	_out << ", \"words\": "; vnx::write(_out, words);
	_out << ", \"width\": "; vnx::write(_out, width);
	_out << ", \"height\": "; vnx::write(_out, height);
	_out << "}";
}

void image_link_t::read(std::istream& _in) {
	if(auto _json = vnx::read_json(_in)) {
		from_object(_json->to_object());
	}
}

vnx::Object image_link_t::to_object() const {
	vnx::Object _object;
	_object["__type"] = "vnx.search.image_link_t";
	_object["url"] = url;
	_object["text"] = text;
	_object["words"] = words;
	_object["width"] = width;
	_object["height"] = height;
	return _object;
}

void image_link_t::from_object(const vnx::Object& _object) {
	for(const auto& _entry : _object.field) {
		if(_entry.first == "height") {
			_entry.second.to(height);
		} else if(_entry.first == "text") {
			_entry.second.to(text);
		} else if(_entry.first == "url") {
			_entry.second.to(url);
		} else if(_entry.first == "width") {
			_entry.second.to(width);
		} else if(_entry.first == "words") {
			_entry.second.to(words);
		}
	}
}

vnx::Variant image_link_t::get_field(const std::string& _name) const {
	if(_name == "url") {
		return vnx::Variant(url);
	}
	if(_name == "text") {
		return vnx::Variant(text);
	}
	if(_name == "words") {
		return vnx::Variant(words);
	}
	if(_name == "width") {
		return vnx::Variant(width);
	}
	if(_name == "height") {
		return vnx::Variant(height);
	}
	return vnx::Variant();
}

void image_link_t::set_field(const std::string& _name, const vnx::Variant& _value) {
	if(_name == "url") {
		_value.to(url);
	} else if(_name == "text") {
		_value.to(text);
	} else if(_name == "words") {
		_value.to(words);
	} else if(_name == "width") {
		_value.to(width);
	} else if(_name == "height") {
		_value.to(height);
	} else {
		throw std::logic_error("no such field: '" + _name + "'");
	}
}

/// \private
std::ostream& operator<<(std::ostream& _out, const image_link_t& _value) {
	_value.write(_out);
	return _out;
}

/// \private
std::istream& operator>>(std::istream& _in, image_link_t& _value) {
	_value.read(_in);
	return _in;
}

const vnx::TypeCode* image_link_t::static_get_type_code() {
	const vnx::TypeCode* type_code = vnx::get_type_code(VNX_TYPE_HASH);
	if(!type_code) {
		type_code = vnx::register_type_code(static_create_type_code());
	}
	return type_code;
}

std::shared_ptr<vnx::TypeCode> image_link_t::static_create_type_code() {
	std::shared_ptr<vnx::TypeCode> type_code = std::make_shared<vnx::TypeCode>();
	type_code->name = "vnx.search.image_link_t";
	type_code->type_hash = vnx::Hash64(0xc6f3540672c0973ull);
	type_code->code_hash = vnx::Hash64(0xe33d3cecd7039328ull);
	type_code->is_native = true;
	type_code->parents.resize(1);
	type_code->parents[0] = ::vnx::search::page_link_t::static_get_type_code();
	type_code->create_value = []() -> std::shared_ptr<vnx::Value> { return std::make_shared<vnx::Struct<image_link_t>>(); };
	type_code->fields.resize(5);
	{
		vnx::TypeField& field = type_code->fields[0];
		field.is_extended = true;
		field.name = "url";
		field.code = {32};
	}
	{
		vnx::TypeField& field = type_code->fields[1];
		field.is_extended = true;
		field.name = "text";
		field.code = {32};
	}
	{
		vnx::TypeField& field = type_code->fields[2];
		field.is_extended = true;
		field.name = "words";
		field.code = {12, 32};
	}
	{
		vnx::TypeField& field = type_code->fields[3];
		field.name = "width";
		field.value = vnx::to_string(-1);
		field.code = {7};
	}
	{
		vnx::TypeField& field = type_code->fields[4];
		field.name = "height";
		field.value = vnx::to_string(-1);
		field.code = {7};
	}
	type_code->build();
	return type_code;
}


} // namespace vnx
} // namespace search


namespace vnx {

void read(TypeInput& in, ::vnx::search::image_link_t& value, const TypeCode* type_code, const uint16_t* code) {
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
		Variant tmp;
		vnx::read(in, tmp, type_code, code);
		value.vnx_read_fallback(tmp);
		return;
	}
	if(code) {
		switch(code[0]) {
			case CODE_STRUCT: type_code = type_code->depends[code[1]]; break;
			case CODE_ALT_STRUCT: type_code = type_code->depends[vnx::flip_bytes(code[1])]; break;
			default: {
				Variant tmp;
				vnx::read(in, tmp, type_code, code);
				value.vnx_read_fallback(tmp);
				return;
			}
		}
	}
	const char* const _buf = in.read(type_code->total_field_size);
	if(type_code->is_matched) {
		{
			const vnx::TypeField* const _field = type_code->field_map[3];
			if(_field) {
				vnx::read_value(_buf + _field->offset, value.width, _field->code.data());
			}
		}
		{
			const vnx::TypeField* const _field = type_code->field_map[4];
			if(_field) {
				vnx::read_value(_buf + _field->offset, value.height, _field->code.data());
			}
		}
	}
	for(const vnx::TypeField* _field : type_code->ext_fields) {
		switch(_field->native_index) {
			case 0: vnx::read(in, value.url, type_code, _field->code.data()); break;
			case 1: vnx::read(in, value.text, type_code, _field->code.data()); break;
			case 2: vnx::read(in, value.words, type_code, _field->code.data()); break;
			default: vnx::skip(in, type_code, _field->code.data());
		}
	}
}

void write(TypeOutput& out, const ::vnx::search::image_link_t& value, const TypeCode* type_code, const uint16_t* code) {
	if(code && code[0] == CODE_OBJECT) {
		vnx::write(out, value.to_object(), nullptr, code);
		return;
	}
	if(!type_code || (code && code[0] == CODE_ANY)) {
		type_code = vnx::search::vnx_native_type_code_image_link_t;
		out.write_type_code(type_code);
		vnx::write_class_header<::vnx::search::image_link_t>(out);
	}
	else if(code && code[0] == CODE_STRUCT) {
		type_code = type_code->depends[code[1]];
	}
	char* const _buf = out.write(8);
	vnx::write_value(_buf + 0, value.width);
	vnx::write_value(_buf + 4, value.height);
	vnx::write(out, value.url, type_code, type_code->fields[0].code.data());
	vnx::write(out, value.text, type_code, type_code->fields[1].code.data());
	vnx::write(out, value.words, type_code, type_code->fields[2].code.data());
}

void read(std::istream& in, ::vnx::search::image_link_t& value) {
	value.read(in);
}

void write(std::ostream& out, const ::vnx::search::image_link_t& value) {
	value.write(out);
}

void accept(Visitor& visitor, const ::vnx::search::image_link_t& value) {
	value.accept(visitor);
}

} // vnx