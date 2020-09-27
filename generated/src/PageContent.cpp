
// AUTO GENERATED by vnxcppcodegen

#include <vnx/search/package.hxx>
#include <vnx/search/PageContent.hxx>
#include <vnx/Value.h>

#include <vnx/vnx.h>


namespace vnx {
namespace search {


const vnx::Hash64 PageContent::VNX_TYPE_HASH(0x4cd00be0870fbe22ull);
const vnx::Hash64 PageContent::VNX_CODE_HASH(0x12bf4eb00e88cb34ull);

vnx::Hash64 PageContent::get_type_hash() const {
	return VNX_TYPE_HASH;
}

const char* PageContent::get_type_name() const {
	return "vnx.search.PageContent";
}

const vnx::TypeCode* PageContent::get_type_code() const {
	return vnx::search::vnx_native_type_code_PageContent;
}

std::shared_ptr<PageContent> PageContent::create() {
	return std::make_shared<PageContent>();
}

std::shared_ptr<vnx::Value> PageContent::clone() const {
	return std::make_shared<PageContent>(*this);
}

void PageContent::read(vnx::TypeInput& _in, const vnx::TypeCode* _type_code, const uint16_t* _code) {
	vnx::read(_in, *this, _type_code, _code);
}

void PageContent::write(vnx::TypeOutput& _out, const vnx::TypeCode* _type_code, const uint16_t* _code) const {
	vnx::write(_out, *this, _type_code, _code);
}

void PageContent::accept(vnx::Visitor& _visitor) const {
	const vnx::TypeCode* _type_code = vnx::search::vnx_native_type_code_PageContent;
	_visitor.type_begin(*_type_code);
	_visitor.type_field(_type_code->fields[0], 0); vnx::accept(_visitor, text);
	_visitor.type_end(*_type_code);
}

void PageContent::write(std::ostream& _out) const {
	_out << "{\"__type\": \"vnx.search.PageContent\"";
	_out << ", \"text\": "; vnx::write(_out, text);
	_out << "}";
}

void PageContent::read(std::istream& _in) {
	std::map<std::string, std::string> _object;
	vnx::read_object(_in, _object);
	for(const auto& _entry : _object) {
		if(_entry.first == "text") {
			vnx::from_string(_entry.second, text);
		}
	}
}

vnx::Object PageContent::to_object() const {
	vnx::Object _object;
	_object["__type"] = "vnx.search.PageContent";
	_object["text"] = text;
	return _object;
}

void PageContent::from_object(const vnx::Object& _object) {
	for(const auto& _entry : _object.field) {
		if(_entry.first == "text") {
			_entry.second.to(text);
		}
	}
}

vnx::Variant PageContent::get_field(const std::string& _name) const {
	if(_name == "text") {
		return vnx::Variant(text);
	}
	return vnx::Variant();
}

void PageContent::set_field(const std::string& _name, const vnx::Variant& _value) {
	if(_name == "text") {
		_value.to(text);
	} else {
		throw std::logic_error("no such field: '" + _name + "'");
	}
}

/// \private
std::ostream& operator<<(std::ostream& _out, const PageContent& _value) {
	_value.write(_out);
	return _out;
}

/// \private
std::istream& operator>>(std::istream& _in, PageContent& _value) {
	_value.read(_in);
	return _in;
}

const vnx::TypeCode* PageContent::static_get_type_code() {
	const vnx::TypeCode* type_code = vnx::get_type_code(VNX_TYPE_HASH);
	if(!type_code) {
		type_code = vnx::register_type_code(static_create_type_code());
	}
	return type_code;
}

std::shared_ptr<vnx::TypeCode> PageContent::static_create_type_code() {
	std::shared_ptr<vnx::TypeCode> type_code = std::make_shared<vnx::TypeCode>();
	type_code->name = "vnx.search.PageContent";
	type_code->type_hash = vnx::Hash64(0x4cd00be0870fbe22ull);
	type_code->code_hash = vnx::Hash64(0x12bf4eb00e88cb34ull);
	type_code->is_native = true;
	type_code->is_class = true;
	type_code->create_value = []() -> std::shared_ptr<vnx::Value> { return std::make_shared<PageContent>(); };
	type_code->fields.resize(1);
	{
		vnx::TypeField& field = type_code->fields[0];
		field.is_extended = true;
		field.name = "text";
		field.code = {32};
	}
	type_code->build();
	return type_code;
}


} // namespace vnx
} // namespace search


namespace vnx {

void read(TypeInput& in, ::vnx::search::PageContent& value, const TypeCode* type_code, const uint16_t* code) {
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
	if(type_code->is_matched) {
	}
	for(const vnx::TypeField* _field : type_code->ext_fields) {
		switch(_field->native_index) {
			case 0: vnx::read(in, value.text, type_code, _field->code.data()); break;
			default: vnx::skip(in, type_code, _field->code.data());
		}
	}
}

void write(TypeOutput& out, const ::vnx::search::PageContent& value, const TypeCode* type_code, const uint16_t* code) {
	if(code && code[0] == CODE_OBJECT) {
		vnx::write(out, value.to_object(), nullptr, code);
		return;
	}
	if(!type_code || (code && code[0] == CODE_ANY)) {
		type_code = vnx::search::vnx_native_type_code_PageContent;
		out.write_type_code(type_code);
		vnx::write_class_header<::vnx::search::PageContent>(out);
	}
	if(code && code[0] == CODE_STRUCT) {
		type_code = type_code->depends[code[1]];
	}
	vnx::write(out, value.text, type_code, type_code->fields[0].code.data());
}

void read(std::istream& in, ::vnx::search::PageContent& value) {
	value.read(in);
}

void write(std::ostream& out, const ::vnx::search::PageContent& value) {
	value.write(out);
}

void accept(Visitor& visitor, const ::vnx::search::PageContent& value) {
	value.accept(visitor);
}

} // vnx
