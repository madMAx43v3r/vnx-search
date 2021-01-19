
// AUTO GENERATED by vnxcppcodegen

#include <vnx/search/package.hxx>
#include <vnx/search/CrawlFrontend_fetch.hxx>
#include <vnx/Value.h>
#include <vnx/search/CrawlFrontend_fetch_return.hxx>

#include <vnx/vnx.h>


namespace vnx {
namespace search {


const vnx::Hash64 CrawlFrontend_fetch::VNX_TYPE_HASH(0xddf8de1011cf63d2ull);
const vnx::Hash64 CrawlFrontend_fetch::VNX_CODE_HASH(0x40fbca3869add456ull);

vnx::Hash64 CrawlFrontend_fetch::get_type_hash() const {
	return VNX_TYPE_HASH;
}

std::string CrawlFrontend_fetch::get_type_name() const {
	return "vnx.search.CrawlFrontend.fetch";
}

const vnx::TypeCode* CrawlFrontend_fetch::get_type_code() const {
	return vnx::search::vnx_native_type_code_CrawlFrontend_fetch;
}

std::shared_ptr<CrawlFrontend_fetch> CrawlFrontend_fetch::create() {
	return std::make_shared<CrawlFrontend_fetch>();
}

std::shared_ptr<vnx::Value> CrawlFrontend_fetch::clone() const {
	return std::make_shared<CrawlFrontend_fetch>(*this);
}

void CrawlFrontend_fetch::read(vnx::TypeInput& _in, const vnx::TypeCode* _type_code, const uint16_t* _code) {
	vnx::read(_in, *this, _type_code, _code);
}

void CrawlFrontend_fetch::write(vnx::TypeOutput& _out, const vnx::TypeCode* _type_code, const uint16_t* _code) const {
	vnx::write(_out, *this, _type_code, _code);
}

void CrawlFrontend_fetch::accept(vnx::Visitor& _visitor) const {
	const vnx::TypeCode* _type_code = vnx::search::vnx_native_type_code_CrawlFrontend_fetch;
	_visitor.type_begin(*_type_code);
	_visitor.type_field(_type_code->fields[0], 0); vnx::accept(_visitor, url);
	_visitor.type_end(*_type_code);
}

void CrawlFrontend_fetch::write(std::ostream& _out) const {
	_out << "{\"__type\": \"vnx.search.CrawlFrontend.fetch\"";
	_out << ", \"url\": "; vnx::write(_out, url);
	_out << "}";
}

void CrawlFrontend_fetch::read(std::istream& _in) {
	if(auto _json = vnx::read_json(_in)) {
		from_object(_json->to_object());
	}
}

vnx::Object CrawlFrontend_fetch::to_object() const {
	vnx::Object _object;
	_object["__type"] = "vnx.search.CrawlFrontend.fetch";
	_object["url"] = url;
	return _object;
}

void CrawlFrontend_fetch::from_object(const vnx::Object& _object) {
	for(const auto& _entry : _object.field) {
		if(_entry.first == "url") {
			_entry.second.to(url);
		}
	}
}

vnx::Variant CrawlFrontend_fetch::get_field(const std::string& _name) const {
	if(_name == "url") {
		return vnx::Variant(url);
	}
	return vnx::Variant();
}

void CrawlFrontend_fetch::set_field(const std::string& _name, const vnx::Variant& _value) {
	if(_name == "url") {
		_value.to(url);
	} else {
		throw std::logic_error("no such field: '" + _name + "'");
	}
}

/// \private
std::ostream& operator<<(std::ostream& _out, const CrawlFrontend_fetch& _value) {
	_value.write(_out);
	return _out;
}

/// \private
std::istream& operator>>(std::istream& _in, CrawlFrontend_fetch& _value) {
	_value.read(_in);
	return _in;
}

const vnx::TypeCode* CrawlFrontend_fetch::static_get_type_code() {
	const vnx::TypeCode* type_code = vnx::get_type_code(VNX_TYPE_HASH);
	if(!type_code) {
		type_code = vnx::register_type_code(static_create_type_code());
	}
	return type_code;
}

std::shared_ptr<vnx::TypeCode> CrawlFrontend_fetch::static_create_type_code() {
	std::shared_ptr<vnx::TypeCode> type_code = std::make_shared<vnx::TypeCode>();
	type_code->name = "vnx.search.CrawlFrontend.fetch";
	type_code->type_hash = vnx::Hash64(0xddf8de1011cf63d2ull);
	type_code->code_hash = vnx::Hash64(0x40fbca3869add456ull);
	type_code->is_native = true;
	type_code->is_class = true;
	type_code->is_method = true;
	type_code->create_value = []() -> std::shared_ptr<vnx::Value> { return std::make_shared<CrawlFrontend_fetch>(); };
	type_code->is_const = true;
	type_code->is_async = true;
	type_code->return_type = ::vnx::search::CrawlFrontend_fetch_return::static_get_type_code();
	type_code->fields.resize(1);
	{
		vnx::TypeField& field = type_code->fields[0];
		field.is_extended = true;
		field.name = "url";
		field.code = {32};
	}
	type_code->build();
	return type_code;
}


} // namespace vnx
} // namespace search


namespace vnx {

void read(TypeInput& in, ::vnx::search::CrawlFrontend_fetch& value, const TypeCode* type_code, const uint16_t* code) {
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
	if(type_code->is_matched) {
	}
	for(const vnx::TypeField* _field : type_code->ext_fields) {
		switch(_field->native_index) {
			case 0: vnx::read(in, value.url, type_code, _field->code.data()); break;
			default: vnx::skip(in, type_code, _field->code.data());
		}
	}
}

void write(TypeOutput& out, const ::vnx::search::CrawlFrontend_fetch& value, const TypeCode* type_code, const uint16_t* code) {
	if(code && code[0] == CODE_OBJECT) {
		vnx::write(out, value.to_object(), nullptr, code);
		return;
	}
	if(!type_code || (code && code[0] == CODE_ANY)) {
		type_code = vnx::search::vnx_native_type_code_CrawlFrontend_fetch;
		out.write_type_code(type_code);
		vnx::write_class_header<::vnx::search::CrawlFrontend_fetch>(out);
	}
	else if(code && code[0] == CODE_STRUCT) {
		type_code = type_code->depends[code[1]];
	}
	vnx::write(out, value.url, type_code, type_code->fields[0].code.data());
}

void read(std::istream& in, ::vnx::search::CrawlFrontend_fetch& value) {
	value.read(in);
}

void write(std::ostream& out, const ::vnx::search::CrawlFrontend_fetch& value) {
	value.write(out);
}

void accept(Visitor& visitor, const ::vnx::search::CrawlFrontend_fetch& value) {
	value.accept(visitor);
}

} // vnx
