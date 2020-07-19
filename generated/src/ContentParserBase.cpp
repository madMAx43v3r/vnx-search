
// AUTO GENERATED by vnxcppcodegen

#include <vnx/search/package.hxx>
#include <vnx/search/ContentParserBase.hxx>
#include <vnx/NoSuchMethod.hxx>
#include <vnx/Module.h>
#include <vnx/ModuleInterface_vnx_get_type_code.hxx>
#include <vnx/ModuleInterface_vnx_get_type_code_return.hxx>
#include <vnx/search/ContentParser_parse.hxx>
#include <vnx/search/ContentParser_parse_return.hxx>
#include <vnx/search/HttpResponse.hxx>
#include <vnx/search/TextResponse.hxx>

#include <vnx/vnx.h>


namespace vnx {
namespace search {


const vnx::Hash64 ContentParserBase::VNX_TYPE_HASH(0xbe968e62c4bea207ull);
const vnx::Hash64 ContentParserBase::VNX_CODE_HASH(0x7711f34b6e9a4060ull);

ContentParserBase::ContentParserBase(const std::string& _vnx_name)
	:	Module::Module(_vnx_name)
{
	vnx::read_config(vnx_name + ".frontend_server", frontend_server);
}

vnx::Hash64 ContentParserBase::get_type_hash() const {
	return VNX_TYPE_HASH;
}

const char* ContentParserBase::get_type_name() const {
	return "vnx.search.ContentParser";
}
const vnx::TypeCode* ContentParserBase::get_type_code() const {
	return vnx::search::vnx_native_type_code_ContentParserBase;
}

void ContentParserBase::accept(vnx::Visitor& _visitor) const {
	const vnx::TypeCode* _type_code = vnx::search::vnx_native_type_code_ContentParserBase;
	_visitor.type_begin(*_type_code);
	_visitor.type_field(_type_code->fields[0], 0); vnx::accept(_visitor, frontend_server);
	_visitor.type_end(*_type_code);
}

void ContentParserBase::write(std::ostream& _out) const {
	_out << "{";
	_out << "\"frontend_server\": "; vnx::write(_out, frontend_server);
	_out << "}";
}

void ContentParserBase::read(std::istream& _in) {
	std::map<std::string, std::string> _object;
	vnx::read_object(_in, _object);
	for(const auto& _entry : _object) {
		if(_entry.first == "frontend_server") {
			vnx::from_string(_entry.second, frontend_server);
		}
	}
}

vnx::Object ContentParserBase::to_object() const {
	vnx::Object _object;
	_object["frontend_server"] = frontend_server;
	return _object;
}

void ContentParserBase::from_object(const vnx::Object& _object) {
	for(const auto& _entry : _object.field) {
		if(_entry.first == "frontend_server") {
			_entry.second.to(frontend_server);
		}
	}
}

/// \private
std::ostream& operator<<(std::ostream& _out, const ContentParserBase& _value) {
	_value.write(_out);
	return _out;
}

/// \private
std::istream& operator>>(std::istream& _in, ContentParserBase& _value) {
	_value.read(_in);
	return _in;
}

const vnx::TypeCode* ContentParserBase::static_get_type_code() {
	const vnx::TypeCode* type_code = vnx::get_type_code(VNX_TYPE_HASH);
	if(!type_code) {
		type_code = vnx::register_type_code(static_create_type_code());
	}
	return type_code;
}

std::shared_ptr<vnx::TypeCode> ContentParserBase::static_create_type_code() {
	std::shared_ptr<vnx::TypeCode> type_code = std::make_shared<vnx::TypeCode>();
	type_code->name = "vnx.search.ContentParser";
	type_code->type_hash = vnx::Hash64(0xbe968e62c4bea207ull);
	type_code->code_hash = vnx::Hash64(0x7711f34b6e9a4060ull);
	type_code->is_native = true;
	type_code->methods.resize(2);
	type_code->methods[0] = ::vnx::ModuleInterface_vnx_get_type_code::static_get_type_code();
	type_code->methods[1] = ::vnx::search::ContentParser_parse::static_get_type_code();
	type_code->fields.resize(1);
	{
		vnx::TypeField& field = type_code->fields[0];
		field.is_extended = true;
		field.name = "frontend_server";
		field.value = vnx::to_string("CrawlFrontend");
		field.code = {32};
	}
	type_code->build();
	return type_code;
}

void ContentParserBase::vnx_handle_switch(std::shared_ptr<const vnx::Sample> _sample) {
}

std::shared_ptr<vnx::Value> ContentParserBase::vnx_call_switch(std::shared_ptr<const vnx::Value> _method, const vnx::request_id_t& _request_id) {
	const auto _type_hash = _method->get_type_hash();
	if(_type_hash == vnx::Hash64(0x305ec4d628960e5dull)) {
		auto _args = std::dynamic_pointer_cast<const ::vnx::ModuleInterface_vnx_get_type_code>(_method);
		if(!_args) {
			throw std::logic_error("vnx_call_switch(): !_args");
		}
		auto _return_value = ::vnx::ModuleInterface_vnx_get_type_code_return::create();
		_return_value->_ret_0 = vnx_get_type_code();
		return _return_value;
	} else if(_type_hash == vnx::Hash64(0x3a3496c5361fbf35ull)) {
		auto _args = std::dynamic_pointer_cast<const ::vnx::search::ContentParser_parse>(_method);
		if(!_args) {
			throw std::logic_error("vnx_call_switch(): !_args");
		}
		auto _return_value = ::vnx::search::ContentParser_parse_return::create();
		_return_value->_ret_0 = parse(_args->response);
		return _return_value;
	}
	auto _ex = vnx::NoSuchMethod::create();
	_ex->dst_mac = vnx_request ? vnx_request->dst_mac : 0;
	_ex->method = _method->get_type_name();
	return _ex;
}


} // namespace vnx
} // namespace search


namespace vnx {

void read(TypeInput& in, ::vnx::search::ContentParserBase& value, const TypeCode* type_code, const uint16_t* code) {
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
			case 0: vnx::read(in, value.frontend_server, type_code, _field->code.data()); break;
			default: vnx::skip(in, type_code, _field->code.data());
		}
	}
}

void write(TypeOutput& out, const ::vnx::search::ContentParserBase& value, const TypeCode* type_code, const uint16_t* code) {
	if(!type_code || (code && code[0] == CODE_ANY)) {
		type_code = vnx::search::vnx_native_type_code_ContentParserBase;
		out.write_type_code(type_code);
		vnx::write_class_header<::vnx::search::ContentParserBase>(out);
	}
	if(code && code[0] == CODE_STRUCT) {
		type_code = type_code->depends[code[1]];
	}
	vnx::write(out, value.frontend_server, type_code, type_code->fields[0].code.data());
}

void read(std::istream& in, ::vnx::search::ContentParserBase& value) {
	value.read(in);
}

void write(std::ostream& out, const ::vnx::search::ContentParserBase& value) {
	value.write(out);
}

void accept(Visitor& visitor, const ::vnx::search::ContentParserBase& value) {
	value.accept(visitor);
}

} // vnx
