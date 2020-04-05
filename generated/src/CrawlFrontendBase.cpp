
// AUTO GENERATED by vnxcppcodegen

#include <vnx/search/package.hxx>
#include <vnx/search/CrawlFrontendBase.hxx>
#include <vnx/Input.h>
#include <vnx/Output.h>
#include <vnx/Visitor.h>
#include <vnx/Object.h>
#include <vnx/Struct.h>
#include <vnx/Config.h>
#include <vnx/Binary.h>
#include <vnx/NoSuchMethod.hxx>


namespace vnx {
namespace search {


const vnx::Hash64 CrawlFrontendBase::VNX_TYPE_HASH(0xd91536edf3f184e2ull);
const vnx::Hash64 CrawlFrontendBase::VNX_CODE_HASH(0xee76df62f6f51e0aull);

CrawlFrontendBase::CrawlFrontendBase(const std::string& _vnx_name)
	:	Module::Module(_vnx_name)
{
	vnx::read_config(vnx_name + ".crawl_processor_server", crawl_processor_server);
	vnx::read_config(vnx_name + ".max_content_length", max_content_length);
	vnx::read_config(vnx_name + ".max_response_size", max_response_size);
	vnx::read_config(vnx_name + ".num_threads", num_threads);
	vnx::read_config(vnx_name + ".output_http", output_http);
	vnx::read_config(vnx_name + ".output_text", output_text);
	vnx::read_config(vnx_name + ".response_timeout_ms", response_timeout_ms);
	vnx::read_config(vnx_name + ".stats_interval_ms", stats_interval_ms);
}

vnx::Hash64 CrawlFrontendBase::get_type_hash() const {
	return VNX_TYPE_HASH;
}

const char* CrawlFrontendBase::get_type_name() const {
	return "vnx.search.CrawlFrontend";
}
const vnx::TypeCode* CrawlFrontendBase::get_type_code() const {
	return vnx::search::vnx_native_type_code_CrawlFrontend;
}

void CrawlFrontendBase::accept(vnx::Visitor& _visitor) const {
	const vnx::TypeCode* _type_code = vnx::search::vnx_native_type_code_CrawlFrontend;
	_visitor.type_begin(*_type_code);
	_visitor.type_field(_type_code->fields[0], 0); vnx::accept(_visitor, output_http);
	_visitor.type_field(_type_code->fields[1], 1); vnx::accept(_visitor, output_text);
	_visitor.type_field(_type_code->fields[2], 2); vnx::accept(_visitor, num_threads);
	_visitor.type_field(_type_code->fields[3], 3); vnx::accept(_visitor, response_timeout_ms);
	_visitor.type_field(_type_code->fields[4], 4); vnx::accept(_visitor, stats_interval_ms);
	_visitor.type_field(_type_code->fields[5], 5); vnx::accept(_visitor, max_content_length);
	_visitor.type_field(_type_code->fields[6], 6); vnx::accept(_visitor, max_response_size);
	_visitor.type_field(_type_code->fields[7], 7); vnx::accept(_visitor, crawl_processor_server);
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
	_out << ", \"crawl_processor_server\": "; vnx::write(_out, crawl_processor_server);
	_out << "}";
}

void CrawlFrontendBase::read(std::istream& _in) {
	std::map<std::string, std::string> _object;
	vnx::read_object(_in, _object);
	for(const auto& _entry : _object) {
		if(_entry.first == "crawl_processor_server") {
			vnx::from_string(_entry.second, crawl_processor_server);
		} else if(_entry.first == "max_content_length") {
			vnx::from_string(_entry.second, max_content_length);
		} else if(_entry.first == "max_response_size") {
			vnx::from_string(_entry.second, max_response_size);
		} else if(_entry.first == "num_threads") {
			vnx::from_string(_entry.second, num_threads);
		} else if(_entry.first == "output_http") {
			vnx::from_string(_entry.second, output_http);
		} else if(_entry.first == "output_text") {
			vnx::from_string(_entry.second, output_text);
		} else if(_entry.first == "response_timeout_ms") {
			vnx::from_string(_entry.second, response_timeout_ms);
		} else if(_entry.first == "stats_interval_ms") {
			vnx::from_string(_entry.second, stats_interval_ms);
		}
	}
}

vnx::Object CrawlFrontendBase::to_object() const {
	vnx::Object _object;
	_object["output_http"] = output_http;
	_object["output_text"] = output_text;
	_object["num_threads"] = num_threads;
	_object["response_timeout_ms"] = response_timeout_ms;
	_object["stats_interval_ms"] = stats_interval_ms;
	_object["max_content_length"] = max_content_length;
	_object["max_response_size"] = max_response_size;
	_object["crawl_processor_server"] = crawl_processor_server;
	return _object;
}

void CrawlFrontendBase::from_object(const vnx::Object& _object) {
	for(const auto& _entry : _object.field) {
		if(_entry.first == "crawl_processor_server") {
			_entry.second.to(crawl_processor_server);
		} else if(_entry.first == "max_content_length") {
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
		}
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
	const vnx::TypeCode* type_code = vnx::get_type_code(vnx::Hash64(0xd91536edf3f184e2ull));
	if(!type_code) {
		type_code = vnx::register_type_code(static_create_type_code());
	}
	return type_code;
}

std::shared_ptr<vnx::TypeCode> CrawlFrontendBase::static_create_type_code() {
	std::shared_ptr<vnx::TypeCode> type_code = std::make_shared<vnx::TypeCode>(true);
	type_code->name = "vnx.search.CrawlFrontend";
	type_code->type_hash = vnx::Hash64(0xd91536edf3f184e2ull);
	type_code->code_hash = vnx::Hash64(0xee76df62f6f51e0aull);
	type_code->methods.resize(3);
	{
		std::shared_ptr<vnx::TypeCode> call_type = std::make_shared<vnx::TypeCode>(true);
		call_type->name = "vnx.search.CrawlFrontend.fetch";
		call_type->type_hash = vnx::Hash64(0xddf8de1011cf63d2ull);
		call_type->code_hash = vnx::Hash64(0xb13552111e9746c5ull);
		call_type->is_method = true;
		{
			std::shared_ptr<vnx::TypeCode> return_type = std::make_shared<vnx::TypeCode>(true);
			return_type->name = "vnx.search.CrawlFrontend.fetch.return";
			return_type->type_hash = vnx::Hash64(0xce1d8d53217a282eull);
			return_type->code_hash = vnx::Hash64(0xa1652afbbce2358ull);
			return_type->is_return = true;
			return_type->fields.resize(1);
			{
				vnx::TypeField& field = return_type->fields[0];
				field.is_extended = true;
				field.name = "_ret_0";
				field.code = {16};
			}
			return_type->build();
			call_type->return_type = vnx::register_type_code(return_type);
		}
		call_type->fields.resize(1);
		{
			vnx::TypeField& field = call_type->fields[0];
			field.is_extended = true;
			field.name = "url";
			field.code = {12, 5};
		}
		call_type->build();
		type_code->methods[0] = vnx::register_type_code(call_type);
	}
	{
		std::shared_ptr<vnx::TypeCode> call_type = std::make_shared<vnx::TypeCode>(true);
		call_type->name = "vnx.search.CrawlFrontend.handle_vnx_search_HttpResponse";
		call_type->type_hash = vnx::Hash64(0x3c451150d4eaa9ebull);
		call_type->code_hash = vnx::Hash64(0x7a3a3e7bc193321cull);
		call_type->is_method = true;
		{
			std::shared_ptr<vnx::TypeCode> return_type = std::make_shared<vnx::TypeCode>(true);
			return_type->name = "vnx.search.CrawlFrontend.handle_vnx_search_HttpResponse.return";
			return_type->type_hash = vnx::Hash64(0x1d67c2d26b1211d3ull);
			return_type->code_hash = vnx::Hash64(0xf94225336e826da7ull);
			return_type->is_return = true;
			return_type->build();
			call_type->return_type = vnx::register_type_code(return_type);
		}
		call_type->fields.resize(1);
		{
			vnx::TypeField& field = call_type->fields[0];
			field.is_extended = true;
			field.name = "sample";
			field.code = {16};
		}
		call_type->build();
		type_code->methods[1] = vnx::register_type_code(call_type);
	}
	{
		std::shared_ptr<vnx::TypeCode> call_type = std::make_shared<vnx::TypeCode>(true);
		call_type->name = "vnx.search.CrawlFrontend.register_parser";
		call_type->type_hash = vnx::Hash64(0x4b91d9c8a161bbfdull);
		call_type->code_hash = vnx::Hash64(0xd67e0fde56679bdcull);
		call_type->is_method = true;
		{
			std::shared_ptr<vnx::TypeCode> return_type = std::make_shared<vnx::TypeCode>(true);
			return_type->name = "vnx.search.CrawlFrontend.register_parser.return";
			return_type->type_hash = vnx::Hash64(0x8ced9de5e282cceaull);
			return_type->code_hash = vnx::Hash64(0x2ad7f1773ea15af0ull);
			return_type->is_return = true;
			return_type->build();
			call_type->return_type = vnx::register_type_code(return_type);
		}
		call_type->fields.resize(3);
		{
			vnx::TypeField& field = call_type->fields[0];
			field.is_extended = true;
			field.name = "address";
			field.code = {4};
		}
		{
			vnx::TypeField& field = call_type->fields[1];
			field.is_extended = true;
			field.name = "mime_types";
			field.code = {12, 12, 5};
		}
		{
			vnx::TypeField& field = call_type->fields[2];
			field.name = "num_threads";
			field.code = {7};
		}
		call_type->build();
		type_code->methods[2] = vnx::register_type_code(call_type);
	}
	type_code->fields.resize(8);
	{
		vnx::TypeField& field = type_code->fields[0];
		field.is_extended = true;
		field.name = "output_http";
		field.value = vnx::to_string("frontend.http_responses");
		field.code = {12, 5};
	}
	{
		vnx::TypeField& field = type_code->fields[1];
		field.is_extended = true;
		field.name = "output_text";
		field.value = vnx::to_string("frontend.text_responses");
		field.code = {12, 5};
	}
	{
		vnx::TypeField& field = type_code->fields[2];
		field.name = "num_threads";
		field.value = vnx::to_string(10);
		field.code = {7};
	}
	{
		vnx::TypeField& field = type_code->fields[3];
		field.name = "response_timeout_ms";
		field.value = vnx::to_string(30000);
		field.code = {7};
	}
	{
		vnx::TypeField& field = type_code->fields[4];
		field.name = "stats_interval_ms";
		field.value = vnx::to_string(10000);
		field.code = {7};
	}
	{
		vnx::TypeField& field = type_code->fields[5];
		field.name = "max_content_length";
		field.value = vnx::to_string(4194304);
		field.code = {8};
	}
	{
		vnx::TypeField& field = type_code->fields[6];
		field.name = "max_response_size";
		field.value = vnx::to_string(16777216);
		field.code = {8};
	}
	{
		vnx::TypeField& field = type_code->fields[7];
		field.is_extended = true;
		field.name = "crawl_processor_server";
		field.value = vnx::to_string("CrawlProcessor");
		field.code = {12, 5};
	}
	type_code->build();
	return type_code;
}

void CrawlFrontendBase::vnx_handle_switch(std::shared_ptr<const ::vnx::Sample> _sample) {
	const uint64_t _type_hash = _sample->value->get_type_hash();
	if(_type_hash == 0xd6552db423d70e21ull) {
		std::shared_ptr<const vnx::search::HttpResponse> _value = std::dynamic_pointer_cast<const vnx::search::HttpResponse>(_sample->value);
		if(_value) {
			handle(_value, _sample);
		}
	}
}

std::shared_ptr<vnx::Value> CrawlFrontendBase::vnx_call_switch(vnx::TypeInput& _in, const vnx::TypeCode* _call_type, const vnx::request_id_t& _request_id) {
	if(_call_type->type_hash == vnx::Hash64(0xddf8de1011cf63d2ull)) {
		::std::string url;
		{
			const char* const _buf = _in.read(_call_type->total_field_size);
			if(_call_type->is_matched) {
			}
			for(const vnx::TypeField* _field : _call_type->ext_fields) {
				switch(_field->native_index) {
					case 0: vnx::read(_in, url, _call_type, _field->code.data()); break;
					default: vnx::skip(_in, _call_type, _field->code.data());
				}
			}
		}
		fetch_async(url, std::bind(&CrawlFrontendBase::fetch_async_return, this, _request_id, std::placeholders::_1), _request_id);
		return 0;
	} else if(_call_type->type_hash == vnx::Hash64(0x3c451150d4eaa9ebull)) {
		::std::shared_ptr<const ::vnx::search::HttpResponse> sample;
		{
			const char* const _buf = _in.read(_call_type->total_field_size);
			if(_call_type->is_matched) {
			}
			for(const vnx::TypeField* _field : _call_type->ext_fields) {
				switch(_field->native_index) {
					case 0: vnx::read(_in, sample, _call_type, _field->code.data()); break;
					default: vnx::skip(_in, _call_type, _field->code.data());
				}
			}
		}
		handle(sample);
		std::shared_ptr<vnx::Binary> _return_value;
		{
			const vnx::TypeCode* _return_type = vnx::search::vnx_native_type_code_CrawlFrontend_handle_vnx_search_HttpResponse_return;
			_return_value = vnx::Binary::create();
			_return_value->type_code = _return_type;
		}
		return _return_value;
	} else if(_call_type->type_hash == vnx::Hash64(0x4b91d9c8a161bbfdull)) {
		::vnx::Hash64 address;
		::std::vector<::std::string> mime_types;
		::int32_t num_threads = 0;
		{
			const char* const _buf = _in.read(_call_type->total_field_size);
			if(_call_type->is_matched) {
				{
					const vnx::TypeField* const _field = _call_type->field_map[2];
					if(_field) {
						vnx::read_value(_buf + _field->offset, num_threads, _field->code.data());
					}
				}
			}
			for(const vnx::TypeField* _field : _call_type->ext_fields) {
				switch(_field->native_index) {
					case 0: vnx::read(_in, address, _call_type, _field->code.data()); break;
					case 1: vnx::read(_in, mime_types, _call_type, _field->code.data()); break;
					default: vnx::skip(_in, _call_type, _field->code.data());
				}
			}
		}
		register_parser(address, mime_types, num_threads);
		std::shared_ptr<vnx::Binary> _return_value;
		{
			const vnx::TypeCode* _return_type = vnx::search::vnx_native_type_code_CrawlFrontend_register_parser_return;
			_return_value = vnx::Binary::create();
			_return_value->type_code = _return_type;
		}
		return _return_value;
	}
	auto _ex = vnx::NoSuchMethod::create();
	_ex->dst_mac = vnx_request ? vnx_request->dst_mac : 0;
	_ex->method = _call_type->name;
	return _ex;
}

void CrawlFrontendBase::fetch_async_return(const vnx::request_id_t& _request_id, const ::std::shared_ptr<const ::vnx::search::UrlIndex>& _ret_0) {
	std::shared_ptr<vnx::Binary> _return_value;
	const vnx::TypeCode* _return_type = vnx::search::vnx_native_type_code_CrawlFrontend_fetch_return;
	_return_value = vnx::Binary::create();
	_return_value->type_code = _return_type;
	vnx::BinaryOutputStream _stream_out(_return_value.get());
	vnx::TypeOutput _out(&_stream_out);
	vnx::write(_out, _ret_0, _return_type, _return_type->fields[0].code.data());
	_out.flush();
	vnx_async_return(_request_id, _return_value);
}


} // namespace vnx
} // namespace search


namespace vnx {

void read(TypeInput& in, ::vnx::search::CrawlFrontendBase& value, const TypeCode* type_code, const uint16_t* code) {
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
				vnx::read_value(_buf + _field->offset, value.num_threads, _field->code.data());
			}
		}
		{
			const vnx::TypeField* const _field = type_code->field_map[3];
			if(_field) {
				vnx::read_value(_buf + _field->offset, value.response_timeout_ms, _field->code.data());
			}
		}
		{
			const vnx::TypeField* const _field = type_code->field_map[4];
			if(_field) {
				vnx::read_value(_buf + _field->offset, value.stats_interval_ms, _field->code.data());
			}
		}
		{
			const vnx::TypeField* const _field = type_code->field_map[5];
			if(_field) {
				vnx::read_value(_buf + _field->offset, value.max_content_length, _field->code.data());
			}
		}
		{
			const vnx::TypeField* const _field = type_code->field_map[6];
			if(_field) {
				vnx::read_value(_buf + _field->offset, value.max_response_size, _field->code.data());
			}
		}
	}
	for(const vnx::TypeField* _field : type_code->ext_fields) {
		switch(_field->native_index) {
			case 0: vnx::read(in, value.output_http, type_code, _field->code.data()); break;
			case 1: vnx::read(in, value.output_text, type_code, _field->code.data()); break;
			case 7: vnx::read(in, value.crawl_processor_server, type_code, _field->code.data()); break;
			default: vnx::skip(in, type_code, _field->code.data());
		}
	}
}

void write(TypeOutput& out, const ::vnx::search::CrawlFrontendBase& value, const TypeCode* type_code, const uint16_t* code) {
	if(!type_code || (code && code[0] == CODE_ANY)) {
		type_code = vnx::search::vnx_native_type_code_CrawlFrontend;
		out.write_type_code(type_code);
		vnx::write_class_header<::vnx::search::CrawlFrontendBase>(out);
	}
	if(code && code[0] == CODE_STRUCT) {
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
	vnx::write(out, value.crawl_processor_server, type_code, type_code->fields[7].code.data());
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
