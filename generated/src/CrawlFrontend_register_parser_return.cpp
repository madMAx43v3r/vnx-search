
// AUTO GENERATED by vnxcppcodegen

#include <vnx/search/package.hxx>
#include <vnx/search/CrawlFrontend_register_parser_return.hxx>
#include <vnx/Value.h>

#include <vnx/vnx.h>


namespace vnx {
namespace search {


const vnx::Hash64 CrawlFrontend_register_parser_return::VNX_TYPE_HASH(0x8ced9de5e282cceaull);
const vnx::Hash64 CrawlFrontend_register_parser_return::VNX_CODE_HASH(0xddad8ece4852ce1eull);

vnx::Hash64 CrawlFrontend_register_parser_return::get_type_hash() const {
	return VNX_TYPE_HASH;
}

std::string CrawlFrontend_register_parser_return::get_type_name() const {
	return "vnx.search.CrawlFrontend.register_parser.return";
}

const vnx::TypeCode* CrawlFrontend_register_parser_return::get_type_code() const {
	return vnx::search::vnx_native_type_code_CrawlFrontend_register_parser_return;
}

std::shared_ptr<CrawlFrontend_register_parser_return> CrawlFrontend_register_parser_return::create() {
	return std::make_shared<CrawlFrontend_register_parser_return>();
}

std::shared_ptr<vnx::Value> CrawlFrontend_register_parser_return::clone() const {
	return std::make_shared<CrawlFrontend_register_parser_return>(*this);
}

void CrawlFrontend_register_parser_return::read(vnx::TypeInput& _in, const vnx::TypeCode* _type_code, const uint16_t* _code) {
	vnx::read(_in, *this, _type_code, _code);
}

void CrawlFrontend_register_parser_return::write(vnx::TypeOutput& _out, const vnx::TypeCode* _type_code, const uint16_t* _code) const {
	vnx::write(_out, *this, _type_code, _code);
}

void CrawlFrontend_register_parser_return::accept(vnx::Visitor& _visitor) const {
	const vnx::TypeCode* _type_code = vnx::search::vnx_native_type_code_CrawlFrontend_register_parser_return;
	_visitor.type_begin(*_type_code);
	_visitor.type_end(*_type_code);
}

void CrawlFrontend_register_parser_return::write(std::ostream& _out) const {
	_out << "{\"__type\": \"vnx.search.CrawlFrontend.register_parser.return\"";
	_out << "}";
}

void CrawlFrontend_register_parser_return::read(std::istream& _in) {
	if(auto _json = vnx::read_json(_in)) {
		from_object(_json->to_object());
	}
}

vnx::Object CrawlFrontend_register_parser_return::to_object() const {
	vnx::Object _object;
	_object["__type"] = "vnx.search.CrawlFrontend.register_parser.return";
	return _object;
}

void CrawlFrontend_register_parser_return::from_object(const vnx::Object& _object) {
}

vnx::Variant CrawlFrontend_register_parser_return::get_field(const std::string& _name) const {
	return vnx::Variant();
}

void CrawlFrontend_register_parser_return::set_field(const std::string& _name, const vnx::Variant& _value) {
	throw std::logic_error("no such field: '" + _name + "'");
}

/// \private
std::ostream& operator<<(std::ostream& _out, const CrawlFrontend_register_parser_return& _value) {
	_value.write(_out);
	return _out;
}

/// \private
std::istream& operator>>(std::istream& _in, CrawlFrontend_register_parser_return& _value) {
	_value.read(_in);
	return _in;
}

const vnx::TypeCode* CrawlFrontend_register_parser_return::static_get_type_code() {
	const vnx::TypeCode* type_code = vnx::get_type_code(VNX_TYPE_HASH);
	if(!type_code) {
		type_code = vnx::register_type_code(static_create_type_code());
	}
	return type_code;
}

std::shared_ptr<vnx::TypeCode> CrawlFrontend_register_parser_return::static_create_type_code() {
	std::shared_ptr<vnx::TypeCode> type_code = std::make_shared<vnx::TypeCode>();
	type_code->name = "vnx.search.CrawlFrontend.register_parser.return";
	type_code->type_hash = vnx::Hash64(0x8ced9de5e282cceaull);
	type_code->code_hash = vnx::Hash64(0xddad8ece4852ce1eull);
	type_code->is_native = true;
	type_code->is_class = true;
	type_code->is_return = true;
	type_code->create_value = []() -> std::shared_ptr<vnx::Value> { return std::make_shared<CrawlFrontend_register_parser_return>(); };
	type_code->build();
	return type_code;
}


} // namespace vnx
} // namespace search


namespace vnx {

void read(TypeInput& in, ::vnx::search::CrawlFrontend_register_parser_return& value, const TypeCode* type_code, const uint16_t* code) {
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
			default: vnx::skip(in, type_code, _field->code.data());
		}
	}
}

void write(TypeOutput& out, const ::vnx::search::CrawlFrontend_register_parser_return& value, const TypeCode* type_code, const uint16_t* code) {
	if(code && code[0] == CODE_OBJECT) {
		vnx::write(out, value.to_object(), nullptr, code);
		return;
	}
	if(!type_code || (code && code[0] == CODE_ANY)) {
		type_code = vnx::search::vnx_native_type_code_CrawlFrontend_register_parser_return;
		out.write_type_code(type_code);
		vnx::write_class_header<::vnx::search::CrawlFrontend_register_parser_return>(out);
	}
	else if(code && code[0] == CODE_STRUCT) {
		type_code = type_code->depends[code[1]];
	}
}

void read(std::istream& in, ::vnx::search::CrawlFrontend_register_parser_return& value) {
	value.read(in);
}

void write(std::ostream& out, const ::vnx::search::CrawlFrontend_register_parser_return& value) {
	value.write(out);
}

void accept(Visitor& visitor, const ::vnx::search::CrawlFrontend_register_parser_return& value) {
	value.accept(visitor);
}

} // vnx
