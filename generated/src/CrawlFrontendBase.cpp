
// AUTO GENERATED by vnxcppcodegen

#include <vnx/search/package.hxx>
#include <vnx/search/CrawlFrontendBase.hxx>
#include <vnx/NoSuchMethod.hxx>
#include <vnx/Hash64.hpp>
#include <vnx/Module.h>
#include <vnx/ModuleInterface_vnx_get_config.hxx>
#include <vnx/ModuleInterface_vnx_get_config_return.hxx>
#include <vnx/ModuleInterface_vnx_get_config_object.hxx>
#include <vnx/ModuleInterface_vnx_get_config_object_return.hxx>
#include <vnx/ModuleInterface_vnx_get_module_info.hxx>
#include <vnx/ModuleInterface_vnx_get_module_info_return.hxx>
#include <vnx/ModuleInterface_vnx_get_type_code.hxx>
#include <vnx/ModuleInterface_vnx_get_type_code_return.hxx>
#include <vnx/ModuleInterface_vnx_restart.hxx>
#include <vnx/ModuleInterface_vnx_restart_return.hxx>
#include <vnx/ModuleInterface_vnx_self_test.hxx>
#include <vnx/ModuleInterface_vnx_self_test_return.hxx>
#include <vnx/ModuleInterface_vnx_set_config.hxx>
#include <vnx/ModuleInterface_vnx_set_config_return.hxx>
#include <vnx/ModuleInterface_vnx_set_config_object.hxx>
#include <vnx/ModuleInterface_vnx_set_config_object_return.hxx>
#include <vnx/ModuleInterface_vnx_stop.hxx>
#include <vnx/ModuleInterface_vnx_stop_return.hxx>
#include <vnx/TopicPtr.hpp>
#include <vnx/search/CrawlFrontend_fetch.hxx>
#include <vnx/search/CrawlFrontend_fetch_return.hxx>
#include <vnx/search/CrawlFrontend_load.hxx>
#include <vnx/search/CrawlFrontend_load_return.hxx>
#include <vnx/search/CrawlFrontend_register_parser.hxx>
#include <vnx/search/CrawlFrontend_register_parser_return.hxx>
#include <vnx/search/FetchResult.hxx>
#include <vnx/search/LoadResult.hxx>

#include <vnx/vnx.h>


namespace vnx {
namespace search {


const vnx::Hash64 CrawlFrontendBase::VNX_TYPE_HASH(0xd91536edf3f184e2ull);
const vnx::Hash64 CrawlFrontendBase::VNX_CODE_HASH(0x9fc0db083bc886dull);

CrawlFrontendBase::CrawlFrontendBase(const std::string& _vnx_name)
	:	Module::Module(_vnx_name)
{
	vnx::read_config(vnx_name + ".output_http", output_http);
	vnx::read_config(vnx_name + ".output_text", output_text);
	vnx::read_config(vnx_name + ".num_threads", num_threads);
	vnx::read_config(vnx_name + ".response_timeout_ms", response_timeout_ms);
	vnx::read_config(vnx_name + ".stats_interval_ms", stats_interval_ms);
	vnx::read_config(vnx_name + ".max_content_length", max_content_length);
	vnx::read_config(vnx_name + ".max_response_size", max_response_size);
	vnx::read_config(vnx_name + ".user_agent", user_agent);
}

vnx::Hash64 CrawlFrontendBase::get_type_hash() const {
	return VNX_TYPE_HASH;
}

std::string CrawlFrontendBase::get_type_name() const {
	return "vnx.search.CrawlFrontend";
}

const vnx::TypeCode* CrawlFrontendBase::get_type_code() const {
	return vnx::search::vnx_native_type_code_CrawlFrontendBase;
}

void CrawlFrontendBase::accept(vnx::Visitor& _visitor) const {
	const vnx::TypeCode* _type_code = vnx::search::vnx_native_type_code_CrawlFrontendBase;
	_visitor.type_begin(*_type_code);
	_visitor.type_field(_type_code->fields[0], 0); vnx::accept(_visitor, output_http);
	_visitor.type_field(_type_code->fields[1], 1); vnx::accept(_visitor, output_text);
	_visitor.type_field(_type_code->fields[2], 2); vnx::accept(_visitor, num_threads);
	_visitor.type_field(_type_code->fields[3], 3); vnx::accept(_visitor, response_timeout_ms);
	_visitor.type_field(_type_code->fields[4], 4); vnx::accept(_visitor, stats_interval_ms);
	_visitor.type_field(_type_code->fields[5], 5); vnx::accept(_visitor, max_content_length);
	_visitor.type_field(_type_code->fields[6], 6); vnx::accept(_visitor, max_response_size);
	_visitor.type_field(_type_code->fields[7], 7); vnx::accept(_visitor, user_agent);
	_visitor.type_end(*_type_code);
}

void CrawlFrontendBase::write(std::ostream& _out) const {
	_out << "{";
	_out << "\"output_http\": "; vnx::write(_out, output_http);
	_out << ", \"output_text\": "; vnx::write(_out, output_text);
	_out << ", \"num_threads\": "; vnx::write(_out, num_threads);
	_out << ", \"response_timeout_ms\": "; vnx::write(_out, response_timeout_ms);
	_out << ", \"stats_interval_ms\": "; vnx::write(_out, stats_interval_ms);
	_out << ", \"max_content_length\": "; vnx::write(_out, max_content_length);
	_out << ", \"max_response_size\": "; vnx::write(_out, max_response_size);
	_out << ", \"user_agent\": "; vnx::write(_out, user_agent);
	_out << "}";
}

void CrawlFrontendBase::read(std::istream& _in) {
	if(auto _json = vnx::read_json(_in)) {
		from_object(_json->to_object());
	}
}

vnx::Object CrawlFrontendBase::to_object() const {
	vnx::Object _object;
	_object["__type"] = "vnx.search.CrawlFrontend";
	_object["output_http"] = output_http;
	_object["output_text"] = output_text;
	_object["num_threads"] = num_threads;
	_object["response_timeout_ms"] = response_timeout_ms;
	_object["stats_interval_ms"] = stats_interval_ms;
	_object["max_content_length"] = max_content_length;
	_object["max_response_size"] = max_response_size;
	_object["user_agent"] = user_agent;
	return _object;
}

void CrawlFrontendBase::from_object(const vnx::Object& _object) {
	for(const auto& _entry : _object.field) {
		if(_entry.first == "max_content_length") {
			_entry.second.to(max_content_length);
		} else if(_entry.first == "max_response_size") {
			_entry.second.to(max_response_size);
		} else if(_entry.first == "num_threads") {
			_entry.second.to(num_threads);
		} else if(_entry.first == "output_http") {
			_entry.second.to(output_http);
		} else if(_entry.first == "output_text") {
			_entry.second.to(output_text);
		} else if(_entry.first == "response_timeout_ms") {
			_entry.second.to(response_timeout_ms);
		} else if(_entry.first == "stats_interval_ms") {
			_entry.second.to(stats_interval_ms);
		} else if(_entry.first == "user_agent") {
			_entry.second.to(user_agent);
		}
	}
}

vnx::Variant CrawlFrontendBase::get_field(const std::string& _name) const {
	if(_name == "output_http") {
		return vnx::Variant(output_http);
	}
	if(_name == "output_text") {
		return vnx::Variant(output_text);
	}
	if(_name == "num_threads") {
		return vnx::Variant(num_threads);
	}
	if(_name == "response_timeout_ms") {
		return vnx::Variant(response_timeout_ms);
	}
	if(_name == "stats_interval_ms") {
		return vnx::Variant(stats_interval_ms);
	}
	if(_name == "max_content_length") {
		return vnx::Variant(max_content_length);
	}
	if(_name == "max_response_size") {
		return vnx::Variant(max_response_size);
	}
	if(_name == "user_agent") {
		return vnx::Variant(user_agent);
	}
	return vnx::Variant();
}

void CrawlFrontendBase::set_field(const std::string& _name, const vnx::Variant& _value) {
	if(_name == "output_http") {
		_value.to(output_http);
	} else if(_name == "output_text") {
		_value.to(output_text);
	} else if(_name == "num_threads") {
		_value.to(num_threads);
	} else if(_name == "response_timeout_ms") {
		_value.to(response_timeout_ms);
	} else if(_name == "stats_interval_ms") {
		_value.to(stats_interval_ms);
	} else if(_name == "max_content_length") {
		_value.to(max_content_length);
	} else if(_name == "max_response_size") {
		_value.to(max_response_size);
	} else if(_name == "user_agent") {
		_value.to(user_agent);
	} else {
		throw std::logic_error("no such field: '" + _name + "'");
	}
}

/// \private
std::ostream& operator<<(std::ostream& _out, const CrawlFrontendBase& _value) {
	_value.write(_out);
	return _out;
}

/// \private
std::istream& operator>>(std::istream& _in, CrawlFrontendBase& _value) {
	_value.read(_in);
	return _in;
}

const vnx::TypeCode* CrawlFrontendBase::static_get_type_code() {
	const vnx::TypeCode* type_code = vnx::get_type_code(VNX_TYPE_HASH);
	if(!type_code) {
		type_code = vnx::register_type_code(static_create_type_code());
	}
	return type_code;
}

std::shared_ptr<vnx::TypeCode> CrawlFrontendBase::static_create_type_code() {
	auto type_code = std::make_shared<vnx::TypeCode>();
	type_code->name = "vnx.search.CrawlFrontend";
	type_code->type_hash = vnx::Hash64(0xd91536edf3f184e2ull);
	type_code->code_hash = vnx::Hash64(0x9fc0db083bc886dull);
	type_code->is_native = true;
	type_code->native_size = sizeof(::vnx::search::CrawlFrontendBase);
	type_code->methods.resize(12);
	type_code->methods[0] = ::vnx::ModuleInterface_vnx_get_config_object::static_get_type_code();
	type_code->methods[1] = ::vnx::ModuleInterface_vnx_get_config::static_get_type_code();
	type_code->methods[2] = ::vnx::ModuleInterface_vnx_set_config_object::static_get_type_code();
	type_code->methods[3] = ::vnx::ModuleInterface_vnx_set_config::static_get_type_code();
	type_code->methods[4] = ::vnx::ModuleInterface_vnx_get_type_code::static_get_type_code();
	type_code->methods[5] = ::vnx::ModuleInterface_vnx_get_module_info::static_get_type_code();
	type_code->methods[6] = ::vnx::ModuleInterface_vnx_restart::static_get_type_code();
	type_code->methods[7] = ::vnx::ModuleInterface_vnx_stop::static_get_type_code();
	type_code->methods[8] = ::vnx::ModuleInterface_vnx_self_test::static_get_type_code();
	type_code->methods[9] = ::vnx::search::CrawlFrontend_load::static_get_type_code();
	type_code->methods[10] = ::vnx::search::CrawlFrontend_fetch::static_get_type_code();
	type_code->methods[11] = ::vnx::search::CrawlFrontend_register_parser::static_get_type_code();
	type_code->fields.resize(8);
	{
		auto& field = type_code->fields[0];
		field.is_extended = true;
		field.name = "output_http";
		field.value = vnx::to_string("frontend.http_responses");
		field.code = {12, 5};
	}
	{
		auto& field = type_code->fields[1];
		field.is_extended = true;
		field.name = "output_text";
		field.value = vnx::to_string("frontend.text_responses");
		field.code = {12, 5};
	}
	{
		auto& field = type_code->fields[2];
		field.data_size = 4;
		field.name = "num_threads";
		field.value = vnx::to_string(10);
		field.code = {7};
	}
	{
		auto& field = type_code->fields[3];
		field.data_size = 4;
		field.name = "response_timeout_ms";
		field.value = vnx::to_string(30000);
		field.code = {7};
	}
	{
		auto& field = type_code->fields[4];
		field.data_size = 4;
		field.name = "stats_interval_ms";
		field.value = vnx::to_string(10000);
		field.code = {7};
	}
	{
		auto& field = type_code->fields[5];
		field.data_size = 8;
		field.name = "max_content_length";
		field.value = vnx::to_string(1048576);
		field.code = {8};
	}
	{
		auto& field = type_code->fields[6];
		field.data_size = 8;
		field.name = "max_response_size";
		field.value = vnx::to_string(4194304);
		field.code = {8};
	}
	{
		auto& field = type_code->fields[7];
		field.is_extended = true;
		field.name = "user_agent";
		field.value = vnx::to_string("Mozilla/5.0");
		field.code = {32};
	}
	type_code->build();
	return type_code;
}

void CrawlFrontendBase::vnx_handle_switch(std::shared_ptr<const vnx::Value> _value) {
	const auto* _type_code = _value->get_type_code();
	while(_type_code) {
		switch(_type_code->type_hash) {
			default:
				_type_code = _type_code->super;
		}
	}
	handle(std::static_pointer_cast<const vnx::Value>(_value));
}

std::shared_ptr<vnx::Value> CrawlFrontendBase::vnx_call_switch(std::shared_ptr<const vnx::Value> _method, const vnx::request_id_t& _request_id) {
	switch(_method->get_type_hash()) {
		case 0x17f58f68bf83abc0ull: {
			auto _args = std::static_pointer_cast<const ::vnx::ModuleInterface_vnx_get_config_object>(_method);
			auto _return_value = ::vnx::ModuleInterface_vnx_get_config_object_return::create();
			_return_value->_ret_0 = vnx_get_config_object();
			return _return_value;
		}
		case 0xbbc7f1a01044d294ull: {
			auto _args = std::static_pointer_cast<const ::vnx::ModuleInterface_vnx_get_config>(_method);
			auto _return_value = ::vnx::ModuleInterface_vnx_get_config_return::create();
			_return_value->_ret_0 = vnx_get_config(_args->name);
			return _return_value;
		}
		case 0xca30f814f17f322full: {
			auto _args = std::static_pointer_cast<const ::vnx::ModuleInterface_vnx_set_config_object>(_method);
			auto _return_value = ::vnx::ModuleInterface_vnx_set_config_object_return::create();
			vnx_set_config_object(_args->config);
			return _return_value;
		}
		case 0x362aac91373958b7ull: {
			auto _args = std::static_pointer_cast<const ::vnx::ModuleInterface_vnx_set_config>(_method);
			auto _return_value = ::vnx::ModuleInterface_vnx_set_config_return::create();
			vnx_set_config(_args->name, _args->value);
			return _return_value;
		}
		case 0x305ec4d628960e5dull: {
			auto _args = std::static_pointer_cast<const ::vnx::ModuleInterface_vnx_get_type_code>(_method);
			auto _return_value = ::vnx::ModuleInterface_vnx_get_type_code_return::create();
			_return_value->_ret_0 = vnx_get_type_code();
			return _return_value;
		}
		case 0xf6d82bdf66d034a1ull: {
			auto _args = std::static_pointer_cast<const ::vnx::ModuleInterface_vnx_get_module_info>(_method);
			auto _return_value = ::vnx::ModuleInterface_vnx_get_module_info_return::create();
			_return_value->_ret_0 = vnx_get_module_info();
			return _return_value;
		}
		case 0x9e95dc280cecca1bull: {
			auto _args = std::static_pointer_cast<const ::vnx::ModuleInterface_vnx_restart>(_method);
			auto _return_value = ::vnx::ModuleInterface_vnx_restart_return::create();
			vnx_restart();
			return _return_value;
		}
		case 0x7ab49ce3d1bfc0d2ull: {
			auto _args = std::static_pointer_cast<const ::vnx::ModuleInterface_vnx_stop>(_method);
			auto _return_value = ::vnx::ModuleInterface_vnx_stop_return::create();
			vnx_stop();
			return _return_value;
		}
		case 0x6ce3775b41a42697ull: {
			auto _args = std::static_pointer_cast<const ::vnx::ModuleInterface_vnx_self_test>(_method);
			auto _return_value = ::vnx::ModuleInterface_vnx_self_test_return::create();
			_return_value->_ret_0 = vnx_self_test();
			return _return_value;
		}
		case 0x3fc44f9cec7e6237ull: {
			auto _args = std::static_pointer_cast<const ::vnx::search::CrawlFrontend_load>(_method);
			load_async(_args->url, _request_id);
			return nullptr;
		}
		case 0xddf8de1011cf63d2ull: {
			auto _args = std::static_pointer_cast<const ::vnx::search::CrawlFrontend_fetch>(_method);
			fetch_async(_args->url, _request_id);
			return nullptr;
		}
		case 0x4b91d9c8a161bbfdull: {
			auto _args = std::static_pointer_cast<const ::vnx::search::CrawlFrontend_register_parser>(_method);
			auto _return_value = ::vnx::search::CrawlFrontend_register_parser_return::create();
			register_parser(_args->address, _args->mime_types, _args->num_threads);
			return _return_value;
		}
	}
	auto _ex = vnx::NoSuchMethod::create();
	_ex->dst_mac = vnx_request ? vnx_request->dst_mac : vnx::Hash64();
	_ex->method = _method->get_type_name();
	return _ex;
}

void CrawlFrontendBase::load_async_return(const vnx::request_id_t& _request_id, const std::shared_ptr<const ::vnx::search::LoadResult>& _ret_0) const {
	auto _return_value = ::vnx::search::CrawlFrontend_load_return::create();
	_return_value->_ret_0 = _ret_0;
	vnx_async_return(_request_id, _return_value);
}

void CrawlFrontendBase::fetch_async_return(const vnx::request_id_t& _request_id, const std::shared_ptr<const ::vnx::search::FetchResult>& _ret_0) const {
	auto _return_value = ::vnx::search::CrawlFrontend_fetch_return::create();
	_return_value->_ret_0 = _ret_0;
	vnx_async_return(_request_id, _return_value);
}


} // namespace vnx
} // namespace search


namespace vnx {

void read(TypeInput& in, ::vnx::search::CrawlFrontendBase& value, const TypeCode* type_code, const uint16_t* code) {
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
		if(const auto* const _field = type_code->field_map[2]) {
			vnx::read_value(_buf + _field->offset, value.num_threads, _field->code.data());
		}
		if(const auto* const _field = type_code->field_map[3]) {
			vnx::read_value(_buf + _field->offset, value.response_timeout_ms, _field->code.data());
		}
		if(const auto* const _field = type_code->field_map[4]) {
			vnx::read_value(_buf + _field->offset, value.stats_interval_ms, _field->code.data());
		}
		if(const auto* const _field = type_code->field_map[5]) {
			vnx::read_value(_buf + _field->offset, value.max_content_length, _field->code.data());
		}
		if(const auto* const _field = type_code->field_map[6]) {
			vnx::read_value(_buf + _field->offset, value.max_response_size, _field->code.data());
		}
	}
	for(const auto* _field : type_code->ext_fields) {
		switch(_field->native_index) {
			case 0: vnx::read(in, value.output_http, type_code, _field->code.data()); break;
			case 1: vnx::read(in, value.output_text, type_code, _field->code.data()); break;
			case 7: vnx::read(in, value.user_agent, type_code, _field->code.data()); break;
			default: vnx::skip(in, type_code, _field->code.data());
		}
	}
}

void write(TypeOutput& out, const ::vnx::search::CrawlFrontendBase& value, const TypeCode* type_code, const uint16_t* code) {
	if(code && code[0] == CODE_OBJECT) {
		vnx::write(out, value.to_object(), nullptr, code);
		return;
	}
	if(!type_code || (code && code[0] == CODE_ANY)) {
		type_code = vnx::search::vnx_native_type_code_CrawlFrontendBase;
		out.write_type_code(type_code);
		vnx::write_class_header<::vnx::search::CrawlFrontendBase>(out);
	}
	else if(code && code[0] == CODE_STRUCT) {
		type_code = type_code->depends[code[1]];
	}
	char* const _buf = out.write(28);
	vnx::write_value(_buf + 0, value.num_threads);
	vnx::write_value(_buf + 4, value.response_timeout_ms);
	vnx::write_value(_buf + 8, value.stats_interval_ms);
	vnx::write_value(_buf + 12, value.max_content_length);
	vnx::write_value(_buf + 20, value.max_response_size);
	vnx::write(out, value.output_http, type_code, type_code->fields[0].code.data());
	vnx::write(out, value.output_text, type_code, type_code->fields[1].code.data());
	vnx::write(out, value.user_agent, type_code, type_code->fields[7].code.data());
}

void read(std::istream& in, ::vnx::search::CrawlFrontendBase& value) {
	value.read(in);
}

void write(std::ostream& out, const ::vnx::search::CrawlFrontendBase& value) {
	value.write(out);
}

void accept(Visitor& visitor, const ::vnx::search::CrawlFrontendBase& value) {
	value.accept(visitor);
}

} // vnx
