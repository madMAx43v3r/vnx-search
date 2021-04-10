
// AUTO GENERATED by vnxcppcodegen

#include <vnx/search/package.hxx>
#include <vnx/search/QueryEngineBase.hxx>
#include <vnx/NoSuchMethod.hxx>
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
#include <vnx/search/QueryInterface_query.hxx>
#include <vnx/search/QueryInterface_query_return.hxx>
#include <vnx/search/SearchResult.hxx>
#include <vnx/search/query_options_t.hxx>

#include <vnx/vnx.h>


namespace vnx {
namespace search {


const vnx::Hash64 QueryEngineBase::VNX_TYPE_HASH(0x9b39d0de19a6c209ull);
const vnx::Hash64 QueryEngineBase::VNX_CODE_HASH(0x2b46f82b35f88ebeull);

QueryEngineBase::QueryEngineBase(const std::string& _vnx_name)
	:	Module::Module(_vnx_name)
{
	vnx::read_config(vnx_name + ".page_index_server", page_index_server);
	vnx::read_config(vnx_name + ".page_content_server", page_content_server);
	vnx::read_config(vnx_name + ".word_context_server", word_context_server);
	vnx::read_config(vnx_name + ".word_array_server", word_array_server);
	vnx::read_config(vnx_name + ".search_engine_server", search_engine_server);
	vnx::read_config(vnx_name + ".num_threads", num_threads);
	vnx::read_config(vnx_name + ".max_pivot_size", max_pivot_size);
}

vnx::Hash64 QueryEngineBase::get_type_hash() const {
	return VNX_TYPE_HASH;
}

std::string QueryEngineBase::get_type_name() const {
	return "vnx.search.QueryEngine";
}

const vnx::TypeCode* QueryEngineBase::get_type_code() const {
	return vnx::search::vnx_native_type_code_QueryEngineBase;
}

void QueryEngineBase::accept(vnx::Visitor& _visitor) const {
	const vnx::TypeCode* _type_code = vnx::search::vnx_native_type_code_QueryEngineBase;
	_visitor.type_begin(*_type_code);
	_visitor.type_field(_type_code->fields[0], 0); vnx::accept(_visitor, page_index_server);
	_visitor.type_field(_type_code->fields[1], 1); vnx::accept(_visitor, page_content_server);
	_visitor.type_field(_type_code->fields[2], 2); vnx::accept(_visitor, word_context_server);
	_visitor.type_field(_type_code->fields[3], 3); vnx::accept(_visitor, word_array_server);
	_visitor.type_field(_type_code->fields[4], 4); vnx::accept(_visitor, search_engine_server);
	_visitor.type_field(_type_code->fields[5], 5); vnx::accept(_visitor, num_threads);
	_visitor.type_field(_type_code->fields[6], 6); vnx::accept(_visitor, max_pivot_size);
	_visitor.type_end(*_type_code);
}

void QueryEngineBase::write(std::ostream& _out) const {
	_out << "{";
	_out << "\"page_index_server\": "; vnx::write(_out, page_index_server);
	_out << ", \"page_content_server\": "; vnx::write(_out, page_content_server);
	_out << ", \"word_context_server\": "; vnx::write(_out, word_context_server);
	_out << ", \"word_array_server\": "; vnx::write(_out, word_array_server);
	_out << ", \"search_engine_server\": "; vnx::write(_out, search_engine_server);
	_out << ", \"num_threads\": "; vnx::write(_out, num_threads);
	_out << ", \"max_pivot_size\": "; vnx::write(_out, max_pivot_size);
	_out << "}";
}

void QueryEngineBase::read(std::istream& _in) {
	if(auto _json = vnx::read_json(_in)) {
		from_object(_json->to_object());
	}
}

vnx::Object QueryEngineBase::to_object() const {
	vnx::Object _object;
	_object["__type"] = "vnx.search.QueryEngine";
	_object["page_index_server"] = page_index_server;
	_object["page_content_server"] = page_content_server;
	_object["word_context_server"] = word_context_server;
	_object["word_array_server"] = word_array_server;
	_object["search_engine_server"] = search_engine_server;
	_object["num_threads"] = num_threads;
	_object["max_pivot_size"] = max_pivot_size;
	return _object;
}

void QueryEngineBase::from_object(const vnx::Object& _object) {
	for(const auto& _entry : _object.field) {
		if(_entry.first == "max_pivot_size") {
			_entry.second.to(max_pivot_size);
		} else if(_entry.first == "num_threads") {
			_entry.second.to(num_threads);
		} else if(_entry.first == "page_content_server") {
			_entry.second.to(page_content_server);
		} else if(_entry.first == "page_index_server") {
			_entry.second.to(page_index_server);
		} else if(_entry.first == "search_engine_server") {
			_entry.second.to(search_engine_server);
		} else if(_entry.first == "word_array_server") {
			_entry.second.to(word_array_server);
		} else if(_entry.first == "word_context_server") {
			_entry.second.to(word_context_server);
		}
	}
}

vnx::Variant QueryEngineBase::get_field(const std::string& _name) const {
	if(_name == "page_index_server") {
		return vnx::Variant(page_index_server);
	}
	if(_name == "page_content_server") {
		return vnx::Variant(page_content_server);
	}
	if(_name == "word_context_server") {
		return vnx::Variant(word_context_server);
	}
	if(_name == "word_array_server") {
		return vnx::Variant(word_array_server);
	}
	if(_name == "search_engine_server") {
		return vnx::Variant(search_engine_server);
	}
	if(_name == "num_threads") {
		return vnx::Variant(num_threads);
	}
	if(_name == "max_pivot_size") {
		return vnx::Variant(max_pivot_size);
	}
	return vnx::Variant();
}

void QueryEngineBase::set_field(const std::string& _name, const vnx::Variant& _value) {
	if(_name == "page_index_server") {
		_value.to(page_index_server);
	} else if(_name == "page_content_server") {
		_value.to(page_content_server);
	} else if(_name == "word_context_server") {
		_value.to(word_context_server);
	} else if(_name == "word_array_server") {
		_value.to(word_array_server);
	} else if(_name == "search_engine_server") {
		_value.to(search_engine_server);
	} else if(_name == "num_threads") {
		_value.to(num_threads);
	} else if(_name == "max_pivot_size") {
		_value.to(max_pivot_size);
	} else {
		throw std::logic_error("no such field: '" + _name + "'");
	}
}

/// \private
std::ostream& operator<<(std::ostream& _out, const QueryEngineBase& _value) {
	_value.write(_out);
	return _out;
}

/// \private
std::istream& operator>>(std::istream& _in, QueryEngineBase& _value) {
	_value.read(_in);
	return _in;
}

const vnx::TypeCode* QueryEngineBase::static_get_type_code() {
	const vnx::TypeCode* type_code = vnx::get_type_code(VNX_TYPE_HASH);
	if(!type_code) {
		type_code = vnx::register_type_code(static_create_type_code());
	}
	return type_code;
}

std::shared_ptr<vnx::TypeCode> QueryEngineBase::static_create_type_code() {
	auto type_code = std::make_shared<vnx::TypeCode>();
	type_code->name = "vnx.search.QueryEngine";
	type_code->type_hash = vnx::Hash64(0x9b39d0de19a6c209ull);
	type_code->code_hash = vnx::Hash64(0x2b46f82b35f88ebeull);
	type_code->is_native = true;
	type_code->native_size = sizeof(::vnx::search::QueryEngineBase);
	type_code->methods.resize(10);
	type_code->methods[0] = ::vnx::ModuleInterface_vnx_get_config_object::static_get_type_code();
	type_code->methods[1] = ::vnx::ModuleInterface_vnx_get_config::static_get_type_code();
	type_code->methods[2] = ::vnx::ModuleInterface_vnx_set_config_object::static_get_type_code();
	type_code->methods[3] = ::vnx::ModuleInterface_vnx_set_config::static_get_type_code();
	type_code->methods[4] = ::vnx::ModuleInterface_vnx_get_type_code::static_get_type_code();
	type_code->methods[5] = ::vnx::ModuleInterface_vnx_get_module_info::static_get_type_code();
	type_code->methods[6] = ::vnx::ModuleInterface_vnx_restart::static_get_type_code();
	type_code->methods[7] = ::vnx::ModuleInterface_vnx_stop::static_get_type_code();
	type_code->methods[8] = ::vnx::ModuleInterface_vnx_self_test::static_get_type_code();
	type_code->methods[9] = ::vnx::search::QueryInterface_query::static_get_type_code();
	type_code->fields.resize(7);
	{
		auto& field = type_code->fields[0];
		field.is_extended = true;
		field.name = "page_index_server";
		field.value = vnx::to_string("PageIndex");
		field.code = {32};
	}
	{
		auto& field = type_code->fields[1];
		field.is_extended = true;
		field.name = "page_content_server";
		field.value = vnx::to_string("PageContent");
		field.code = {32};
	}
	{
		auto& field = type_code->fields[2];
		field.is_extended = true;
		field.name = "word_context_server";
		field.value = vnx::to_string("WordContext");
		field.code = {32};
	}
	{
		auto& field = type_code->fields[3];
		field.is_extended = true;
		field.name = "word_array_server";
		field.value = vnx::to_string("WordArray");
		field.code = {32};
	}
	{
		auto& field = type_code->fields[4];
		field.is_extended = true;
		field.name = "search_engine_server";
		field.value = vnx::to_string("SearchEngine");
		field.code = {32};
	}
	{
		auto& field = type_code->fields[5];
		field.data_size = 4;
		field.name = "num_threads";
		field.value = vnx::to_string(8);
		field.code = {7};
	}
	{
		auto& field = type_code->fields[6];
		field.data_size = 4;
		field.name = "max_pivot_size";
		field.value = vnx::to_string(65536);
		field.code = {7};
	}
	type_code->build();
	return type_code;
}

void QueryEngineBase::vnx_handle_switch(std::shared_ptr<const vnx::Value> _value) {
	const auto* _type_code = _value->get_type_code();
	while(_type_code) {
		switch(_type_code->type_hash) {
			default:
				_type_code = _type_code->super;
		}
	}
	handle(std::static_pointer_cast<const vnx::Value>(_value));
}

std::shared_ptr<vnx::Value> QueryEngineBase::vnx_call_switch(std::shared_ptr<const vnx::Value> _method, const vnx::request_id_t& _request_id) {
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
		case 0xd7ca13b33b457bbaull: {
			auto _args = std::static_pointer_cast<const ::vnx::search::QueryInterface_query>(_method);
			query_async(_args->words, _args->options, _request_id);
			return nullptr;
		}
	}
	auto _ex = vnx::NoSuchMethod::create();
	_ex->dst_mac = vnx_request ? vnx_request->dst_mac : vnx::Hash64();
	_ex->method = _method->get_type_name();
	return _ex;
}

void QueryEngineBase::query_async_return(const vnx::request_id_t& _request_id, const std::shared_ptr<const ::vnx::search::SearchResult>& _ret_0) const {
	auto _return_value = ::vnx::search::QueryInterface_query_return::create();
	_return_value->_ret_0 = _ret_0;
	vnx_async_return(_request_id, _return_value);
}


} // namespace vnx
} // namespace search


namespace vnx {

void read(TypeInput& in, ::vnx::search::QueryEngineBase& value, const TypeCode* type_code, const uint16_t* code) {
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
		if(const auto* const _field = type_code->field_map[5]) {
			vnx::read_value(_buf + _field->offset, value.num_threads, _field->code.data());
		}
		if(const auto* const _field = type_code->field_map[6]) {
			vnx::read_value(_buf + _field->offset, value.max_pivot_size, _field->code.data());
		}
	}
	for(const auto* _field : type_code->ext_fields) {
		switch(_field->native_index) {
			case 0: vnx::read(in, value.page_index_server, type_code, _field->code.data()); break;
			case 1: vnx::read(in, value.page_content_server, type_code, _field->code.data()); break;
			case 2: vnx::read(in, value.word_context_server, type_code, _field->code.data()); break;
			case 3: vnx::read(in, value.word_array_server, type_code, _field->code.data()); break;
			case 4: vnx::read(in, value.search_engine_server, type_code, _field->code.data()); break;
			default: vnx::skip(in, type_code, _field->code.data());
		}
	}
}

void write(TypeOutput& out, const ::vnx::search::QueryEngineBase& value, const TypeCode* type_code, const uint16_t* code) {
	if(code && code[0] == CODE_OBJECT) {
		vnx::write(out, value.to_object(), nullptr, code);
		return;
	}
	if(!type_code || (code && code[0] == CODE_ANY)) {
		type_code = vnx::search::vnx_native_type_code_QueryEngineBase;
		out.write_type_code(type_code);
		vnx::write_class_header<::vnx::search::QueryEngineBase>(out);
	}
	else if(code && code[0] == CODE_STRUCT) {
		type_code = type_code->depends[code[1]];
	}
	char* const _buf = out.write(8);
	vnx::write_value(_buf + 0, value.num_threads);
	vnx::write_value(_buf + 4, value.max_pivot_size);
	vnx::write(out, value.page_index_server, type_code, type_code->fields[0].code.data());
	vnx::write(out, value.page_content_server, type_code, type_code->fields[1].code.data());
	vnx::write(out, value.word_context_server, type_code, type_code->fields[2].code.data());
	vnx::write(out, value.word_array_server, type_code, type_code->fields[3].code.data());
	vnx::write(out, value.search_engine_server, type_code, type_code->fields[4].code.data());
}

void read(std::istream& in, ::vnx::search::QueryEngineBase& value) {
	value.read(in);
}

void write(std::ostream& out, const ::vnx::search::QueryEngineBase& value) {
	value.write(out);
}

void accept(Visitor& visitor, const ::vnx::search::QueryEngineBase& value) {
	value.accept(visitor);
}

} // vnx
