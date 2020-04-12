
// AUTO GENERATED by vnxcppcodegen

#include <vnx/search/package.hxx>
#include <vnx/search/CrawlProcessorBase.hxx>
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


const vnx::Hash64 CrawlProcessorBase::VNX_TYPE_HASH(0x508848d1f9d97d9full);
const vnx::Hash64 CrawlProcessorBase::VNX_CODE_HASH(0x228d1feb7a0bb8ecull);

CrawlProcessorBase::CrawlProcessorBase(const std::string& _vnx_name)
	:	Module::Module(_vnx_name)
{
	vnx::read_config(vnx_name + ".crawl_frontend_server", crawl_frontend_server);
	vnx::read_config(vnx_name + ".input_page_index", input_page_index);
	vnx::read_config(vnx_name + ".input_url_index", input_url_index);
	vnx::read_config(vnx_name + ".jump_cost", jump_cost);
	vnx::read_config(vnx_name + ".max_depth", max_depth);
	vnx::read_config(vnx_name + ".max_num_pending", max_num_pending);
	vnx::read_config(vnx_name + ".max_per_minute", max_per_minute);
	vnx::read_config(vnx_name + ".max_queue_ms", max_queue_ms);
	vnx::read_config(vnx_name + ".max_url_length", max_url_length);
	vnx::read_config(vnx_name + ".output_crawl_stats", output_crawl_stats);
	vnx::read_config(vnx_name + ".reload_interval", reload_interval);
	vnx::read_config(vnx_name + ".reload_power", reload_power);
	vnx::read_config(vnx_name + ".root_urls", root_urls);
	vnx::read_config(vnx_name + ".sync_interval", sync_interval);
	vnx::read_config(vnx_name + ".update_interval_ms", update_interval_ms);
	vnx::read_config(vnx_name + ".url_index_server", url_index_server);
}

vnx::Hash64 CrawlProcessorBase::get_type_hash() const {
	return VNX_TYPE_HASH;
}

const char* CrawlProcessorBase::get_type_name() const {
	return "vnx.search.CrawlProcessor";
}
const vnx::TypeCode* CrawlProcessorBase::get_type_code() const {
	return vnx::search::vnx_native_type_code_CrawlProcessor;
}

void CrawlProcessorBase::accept(vnx::Visitor& _visitor) const {
	const vnx::TypeCode* _type_code = vnx::search::vnx_native_type_code_CrawlProcessor;
	_visitor.type_begin(*_type_code);
	_visitor.type_field(_type_code->fields[0], 0); vnx::accept(_visitor, input_url_index);
	_visitor.type_field(_type_code->fields[1], 1); vnx::accept(_visitor, input_page_index);
	_visitor.type_field(_type_code->fields[2], 2); vnx::accept(_visitor, output_crawl_stats);
	_visitor.type_field(_type_code->fields[3], 3); vnx::accept(_visitor, jump_cost);
	_visitor.type_field(_type_code->fields[4], 4); vnx::accept(_visitor, max_depth);
	_visitor.type_field(_type_code->fields[5], 5); vnx::accept(_visitor, reload_interval);
	_visitor.type_field(_type_code->fields[6], 6); vnx::accept(_visitor, sync_interval);
	_visitor.type_field(_type_code->fields[7], 7); vnx::accept(_visitor, max_per_minute);
	_visitor.type_field(_type_code->fields[8], 8); vnx::accept(_visitor, max_num_pending);
	_visitor.type_field(_type_code->fields[9], 9); vnx::accept(_visitor, max_url_length);
	_visitor.type_field(_type_code->fields[10], 10); vnx::accept(_visitor, update_interval_ms);
	_visitor.type_field(_type_code->fields[11], 11); vnx::accept(_visitor, max_queue_ms);
	_visitor.type_field(_type_code->fields[12], 12); vnx::accept(_visitor, reload_power);
	_visitor.type_field(_type_code->fields[13], 13); vnx::accept(_visitor, root_urls);
	_visitor.type_field(_type_code->fields[14], 14); vnx::accept(_visitor, url_index_server);
	_visitor.type_field(_type_code->fields[15], 15); vnx::accept(_visitor, crawl_frontend_server);
	_visitor.type_end(*_type_code);
}

void CrawlProcessorBase::write(std::ostream& _out) const {
	_out << "{";
	_out << "\"input_url_index\": "; vnx::write(_out, input_url_index);
	_out << ", \"input_page_index\": "; vnx::write(_out, input_page_index);
	_out << ", \"output_crawl_stats\": "; vnx::write(_out, output_crawl_stats);
	_out << ", \"jump_cost\": "; vnx::write(_out, jump_cost);
	_out << ", \"max_depth\": "; vnx::write(_out, max_depth);
	_out << ", \"reload_interval\": "; vnx::write(_out, reload_interval);
	_out << ", \"sync_interval\": "; vnx::write(_out, sync_interval);
	_out << ", \"max_per_minute\": "; vnx::write(_out, max_per_minute);
	_out << ", \"max_num_pending\": "; vnx::write(_out, max_num_pending);
	_out << ", \"max_url_length\": "; vnx::write(_out, max_url_length);
	_out << ", \"update_interval_ms\": "; vnx::write(_out, update_interval_ms);
	_out << ", \"max_queue_ms\": "; vnx::write(_out, max_queue_ms);
	_out << ", \"reload_power\": "; vnx::write(_out, reload_power);
	_out << ", \"root_urls\": "; vnx::write(_out, root_urls);
	_out << ", \"url_index_server\": "; vnx::write(_out, url_index_server);
	_out << ", \"crawl_frontend_server\": "; vnx::write(_out, crawl_frontend_server);
	_out << "}";
}

void CrawlProcessorBase::read(std::istream& _in) {
	std::map<std::string, std::string> _object;
	vnx::read_object(_in, _object);
	for(const auto& _entry : _object) {
		if(_entry.first == "crawl_frontend_server") {
			vnx::from_string(_entry.second, crawl_frontend_server);
		} else if(_entry.first == "input_page_index") {
			vnx::from_string(_entry.second, input_page_index);
		} else if(_entry.first == "input_url_index") {
			vnx::from_string(_entry.second, input_url_index);
		} else if(_entry.first == "jump_cost") {
			vnx::from_string(_entry.second, jump_cost);
		} else if(_entry.first == "max_depth") {
			vnx::from_string(_entry.second, max_depth);
		} else if(_entry.first == "max_num_pending") {
			vnx::from_string(_entry.second, max_num_pending);
		} else if(_entry.first == "max_per_minute") {
			vnx::from_string(_entry.second, max_per_minute);
		} else if(_entry.first == "max_queue_ms") {
			vnx::from_string(_entry.second, max_queue_ms);
		} else if(_entry.first == "max_url_length") {
			vnx::from_string(_entry.second, max_url_length);
		} else if(_entry.first == "output_crawl_stats") {
			vnx::from_string(_entry.second, output_crawl_stats);
		} else if(_entry.first == "reload_interval") {
			vnx::from_string(_entry.second, reload_interval);
		} else if(_entry.first == "reload_power") {
			vnx::from_string(_entry.second, reload_power);
		} else if(_entry.first == "root_urls") {
			vnx::from_string(_entry.second, root_urls);
		} else if(_entry.first == "sync_interval") {
			vnx::from_string(_entry.second, sync_interval);
		} else if(_entry.first == "update_interval_ms") {
			vnx::from_string(_entry.second, update_interval_ms);
		} else if(_entry.first == "url_index_server") {
			vnx::from_string(_entry.second, url_index_server);
		}
	}
}

vnx::Object CrawlProcessorBase::to_object() const {
	vnx::Object _object;
	_object["input_url_index"] = input_url_index;
	_object["input_page_index"] = input_page_index;
	_object["output_crawl_stats"] = output_crawl_stats;
	_object["jump_cost"] = jump_cost;
	_object["max_depth"] = max_depth;
	_object["reload_interval"] = reload_interval;
	_object["sync_interval"] = sync_interval;
	_object["max_per_minute"] = max_per_minute;
	_object["max_num_pending"] = max_num_pending;
	_object["max_url_length"] = max_url_length;
	_object["update_interval_ms"] = update_interval_ms;
	_object["max_queue_ms"] = max_queue_ms;
	_object["reload_power"] = reload_power;
	_object["root_urls"] = root_urls;
	_object["url_index_server"] = url_index_server;
	_object["crawl_frontend_server"] = crawl_frontend_server;
	return _object;
}

void CrawlProcessorBase::from_object(const vnx::Object& _object) {
	for(const auto& _entry : _object.field) {
		if(_entry.first == "crawl_frontend_server") {
			_entry.second.to(crawl_frontend_server);
		} else if(_entry.first == "input_page_index") {
			_entry.second.to(input_page_index);
		} else if(_entry.first == "input_url_index") {
			_entry.second.to(input_url_index);
		} else if(_entry.first == "jump_cost") {
			_entry.second.to(jump_cost);
		} else if(_entry.first == "max_depth") {
			_entry.second.to(max_depth);
		} else if(_entry.first == "max_num_pending") {
			_entry.second.to(max_num_pending);
		} else if(_entry.first == "max_per_minute") {
			_entry.second.to(max_per_minute);
		} else if(_entry.first == "max_queue_ms") {
			_entry.second.to(max_queue_ms);
		} else if(_entry.first == "max_url_length") {
			_entry.second.to(max_url_length);
		} else if(_entry.first == "output_crawl_stats") {
			_entry.second.to(output_crawl_stats);
		} else if(_entry.first == "reload_interval") {
			_entry.second.to(reload_interval);
		} else if(_entry.first == "reload_power") {
			_entry.second.to(reload_power);
		} else if(_entry.first == "root_urls") {
			_entry.second.to(root_urls);
		} else if(_entry.first == "sync_interval") {
			_entry.second.to(sync_interval);
		} else if(_entry.first == "update_interval_ms") {
			_entry.second.to(update_interval_ms);
		} else if(_entry.first == "url_index_server") {
			_entry.second.to(url_index_server);
		}
	}
}

/// \private
std::ostream& operator<<(std::ostream& _out, const CrawlProcessorBase& _value) {
	_value.write(_out);
	return _out;
}

/// \private
std::istream& operator>>(std::istream& _in, CrawlProcessorBase& _value) {
	_value.read(_in);
	return _in;
}

const vnx::TypeCode* CrawlProcessorBase::static_get_type_code() {
	const vnx::TypeCode* type_code = vnx::get_type_code(vnx::Hash64(0x508848d1f9d97d9full));
	if(!type_code) {
		type_code = vnx::register_type_code(static_create_type_code());
	}
	return type_code;
}

std::shared_ptr<vnx::TypeCode> CrawlProcessorBase::static_create_type_code() {
	std::shared_ptr<vnx::TypeCode> type_code = std::make_shared<vnx::TypeCode>(true);
	type_code->name = "vnx.search.CrawlProcessor";
	type_code->type_hash = vnx::Hash64(0x508848d1f9d97d9full);
	type_code->code_hash = vnx::Hash64(0x228d1feb7a0bb8ecull);
	type_code->methods.resize(1);
	{
		std::shared_ptr<vnx::TypeCode> call_type = std::make_shared<vnx::TypeCode>(true);
		call_type->name = "vnx.search.CrawlProcessor.handle_vnx_keyvalue_KeyValuePair";
		call_type->type_hash = vnx::Hash64(0x197f24cf8e40d6aull);
		call_type->code_hash = vnx::Hash64(0x864568088e8836f2ull);
		call_type->is_method = true;
		{
			std::shared_ptr<vnx::TypeCode> return_type = std::make_shared<vnx::TypeCode>(true);
			return_type->name = "vnx.search.CrawlProcessor.handle_vnx_keyvalue_KeyValuePair.return";
			return_type->type_hash = vnx::Hash64(0x5468ec5275268163ull);
			return_type->code_hash = vnx::Hash64(0x68ec355b007c7516ull);
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
		type_code->methods[0] = vnx::register_type_code(call_type);
	}
	type_code->fields.resize(16);
	{
		vnx::TypeField& field = type_code->fields[0];
		field.is_extended = true;
		field.name = "input_url_index";
		field.value = vnx::to_string("backend.url_index.updates");
		field.code = {12, 5};
	}
	{
		vnx::TypeField& field = type_code->fields[1];
		field.is_extended = true;
		field.name = "input_page_index";
		field.value = vnx::to_string("backend.page_index.updates");
		field.code = {12, 5};
	}
	{
		vnx::TypeField& field = type_code->fields[2];
		field.is_extended = true;
		field.name = "output_crawl_stats";
		field.value = vnx::to_string("backend.crawl_stats");
		field.code = {12, 5};
	}
	{
		vnx::TypeField& field = type_code->fields[3];
		field.name = "jump_cost";
		field.value = vnx::to_string(3);
		field.code = {7};
	}
	{
		vnx::TypeField& field = type_code->fields[4];
		field.name = "max_depth";
		field.value = vnx::to_string(9);
		field.code = {7};
	}
	{
		vnx::TypeField& field = type_code->fields[5];
		field.name = "reload_interval";
		field.value = vnx::to_string(10800);
		field.code = {7};
	}
	{
		vnx::TypeField& field = type_code->fields[6];
		field.name = "sync_interval";
		field.value = vnx::to_string(3600);
		field.code = {7};
	}
	{
		vnx::TypeField& field = type_code->fields[7];
		field.name = "max_per_minute";
		field.value = vnx::to_string(30);
		field.code = {7};
	}
	{
		vnx::TypeField& field = type_code->fields[8];
		field.name = "max_num_pending";
		field.value = vnx::to_string(50);
		field.code = {7};
	}
	{
		vnx::TypeField& field = type_code->fields[9];
		field.name = "max_url_length";
		field.value = vnx::to_string(256);
		field.code = {7};
	}
	{
		vnx::TypeField& field = type_code->fields[10];
		field.name = "update_interval_ms";
		field.value = vnx::to_string(200);
		field.code = {7};
	}
	{
		vnx::TypeField& field = type_code->fields[11];
		field.name = "max_queue_ms";
		field.value = vnx::to_string(1000);
		field.code = {7};
	}
	{
		vnx::TypeField& field = type_code->fields[12];
		field.name = "reload_power";
		field.value = vnx::to_string(4);
		field.code = {9};
	}
	{
		vnx::TypeField& field = type_code->fields[13];
		field.is_extended = true;
		field.name = "root_urls";
		field.code = {12, 12, 5};
	}
	{
		vnx::TypeField& field = type_code->fields[14];
		field.is_extended = true;
		field.name = "url_index_server";
		field.value = vnx::to_string("UrlIndex");
		field.code = {12, 5};
	}
	{
		vnx::TypeField& field = type_code->fields[15];
		field.is_extended = true;
		field.name = "crawl_frontend_server";
		field.value = vnx::to_string("CrawlFrontend");
		field.code = {12, 5};
	}
	type_code->build();
	return type_code;
}

void CrawlProcessorBase::vnx_handle_switch(std::shared_ptr<const ::vnx::Sample> _sample) {
	const uint64_t _type_hash = _sample->value->get_type_hash();
	if(_type_hash == 0xf87436237449d8afull) {
		std::shared_ptr<const vnx::keyvalue::KeyValuePair> _value = std::dynamic_pointer_cast<const vnx::keyvalue::KeyValuePair>(_sample->value);
		if(_value) {
			handle(_value, _sample);
		}
	}
}

std::shared_ptr<vnx::Value> CrawlProcessorBase::vnx_call_switch(vnx::TypeInput& _in, const vnx::TypeCode* _call_type, const vnx::request_id_t& _request_id) {
	if(_call_type->type_hash == vnx::Hash64(0x197f24cf8e40d6aull)) {
		::std::shared_ptr<const ::vnx::keyvalue::KeyValuePair> sample;
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
			const vnx::TypeCode* _return_type = vnx::search::vnx_native_type_code_CrawlProcessor_handle_vnx_keyvalue_KeyValuePair_return;
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


} // namespace vnx
} // namespace search


namespace vnx {

void read(TypeInput& in, ::vnx::search::CrawlProcessorBase& value, const TypeCode* type_code, const uint16_t* code) {
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
			const vnx::TypeField* const _field = type_code->field_map[3];
			if(_field) {
				vnx::read_value(_buf + _field->offset, value.jump_cost, _field->code.data());
			}
		}
		{
			const vnx::TypeField* const _field = type_code->field_map[4];
			if(_field) {
				vnx::read_value(_buf + _field->offset, value.max_depth, _field->code.data());
			}
		}
		{
			const vnx::TypeField* const _field = type_code->field_map[5];
			if(_field) {
				vnx::read_value(_buf + _field->offset, value.reload_interval, _field->code.data());
			}
		}
		{
			const vnx::TypeField* const _field = type_code->field_map[6];
			if(_field) {
				vnx::read_value(_buf + _field->offset, value.sync_interval, _field->code.data());
			}
		}
		{
			const vnx::TypeField* const _field = type_code->field_map[7];
			if(_field) {
				vnx::read_value(_buf + _field->offset, value.max_per_minute, _field->code.data());
			}
		}
		{
			const vnx::TypeField* const _field = type_code->field_map[8];
			if(_field) {
				vnx::read_value(_buf + _field->offset, value.max_num_pending, _field->code.data());
			}
		}
		{
			const vnx::TypeField* const _field = type_code->field_map[9];
			if(_field) {
				vnx::read_value(_buf + _field->offset, value.max_url_length, _field->code.data());
			}
		}
		{
			const vnx::TypeField* const _field = type_code->field_map[10];
			if(_field) {
				vnx::read_value(_buf + _field->offset, value.update_interval_ms, _field->code.data());
			}
		}
		{
			const vnx::TypeField* const _field = type_code->field_map[11];
			if(_field) {
				vnx::read_value(_buf + _field->offset, value.max_queue_ms, _field->code.data());
			}
		}
		{
			const vnx::TypeField* const _field = type_code->field_map[12];
			if(_field) {
				vnx::read_value(_buf + _field->offset, value.reload_power, _field->code.data());
			}
		}
	}
	for(const vnx::TypeField* _field : type_code->ext_fields) {
		switch(_field->native_index) {
			case 0: vnx::read(in, value.input_url_index, type_code, _field->code.data()); break;
			case 1: vnx::read(in, value.input_page_index, type_code, _field->code.data()); break;
			case 2: vnx::read(in, value.output_crawl_stats, type_code, _field->code.data()); break;
			case 13: vnx::read(in, value.root_urls, type_code, _field->code.data()); break;
			case 14: vnx::read(in, value.url_index_server, type_code, _field->code.data()); break;
			case 15: vnx::read(in, value.crawl_frontend_server, type_code, _field->code.data()); break;
			default: vnx::skip(in, type_code, _field->code.data());
		}
	}
}

void write(TypeOutput& out, const ::vnx::search::CrawlProcessorBase& value, const TypeCode* type_code, const uint16_t* code) {
	if(!type_code || (code && code[0] == CODE_ANY)) {
		type_code = vnx::search::vnx_native_type_code_CrawlProcessor;
		out.write_type_code(type_code);
		vnx::write_class_header<::vnx::search::CrawlProcessorBase>(out);
	}
	if(code && code[0] == CODE_STRUCT) {
		type_code = type_code->depends[code[1]];
	}
	char* const _buf = out.write(40);
	vnx::write_value(_buf + 0, value.jump_cost);
	vnx::write_value(_buf + 4, value.max_depth);
	vnx::write_value(_buf + 8, value.reload_interval);
	vnx::write_value(_buf + 12, value.sync_interval);
	vnx::write_value(_buf + 16, value.max_per_minute);
	vnx::write_value(_buf + 20, value.max_num_pending);
	vnx::write_value(_buf + 24, value.max_url_length);
	vnx::write_value(_buf + 28, value.update_interval_ms);
	vnx::write_value(_buf + 32, value.max_queue_ms);
	vnx::write_value(_buf + 36, value.reload_power);
	vnx::write(out, value.input_url_index, type_code, type_code->fields[0].code.data());
	vnx::write(out, value.input_page_index, type_code, type_code->fields[1].code.data());
	vnx::write(out, value.output_crawl_stats, type_code, type_code->fields[2].code.data());
	vnx::write(out, value.root_urls, type_code, type_code->fields[13].code.data());
	vnx::write(out, value.url_index_server, type_code, type_code->fields[14].code.data());
	vnx::write(out, value.crawl_frontend_server, type_code, type_code->fields[15].code.data());
}

void read(std::istream& in, ::vnx::search::CrawlProcessorBase& value) {
	value.read(in);
}

void write(std::ostream& out, const ::vnx::search::CrawlProcessorBase& value) {
	value.write(out);
}

void accept(Visitor& visitor, const ::vnx::search::CrawlProcessorBase& value) {
	value.accept(visitor);
}

} // vnx
