
// AUTO GENERATED by vnxcppcodegen

#include <vnx/search/package.hxx>
#include <vnx/search/PageProcessorBase.hxx>
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


const vnx::Hash64 PageProcessorBase::VNX_TYPE_HASH(0xd130b1014d9ffc6full);
const vnx::Hash64 PageProcessorBase::VNX_CODE_HASH(0xe8a78c0aa0ee4d33ull);

PageProcessorBase::PageProcessorBase(const std::string& _vnx_name)
	:	Module::Module(_vnx_name)
{
	vnx::read_config(vnx_name + ".input", input);
	vnx::read_config(vnx_name + ".max_queue_ms", max_queue_ms);
	vnx::read_config(vnx_name + ".page_content_server", page_content_server);
	vnx::read_config(vnx_name + ".page_index_server", page_index_server);
}

vnx::Hash64 PageProcessorBase::get_type_hash() const {
	return VNX_TYPE_HASH;
}

const char* PageProcessorBase::get_type_name() const {
	return "vnx.search.PageProcessor";
}
const vnx::TypeCode* PageProcessorBase::get_type_code() const {
	return vnx::search::vnx_native_type_code_PageProcessor;
}

void PageProcessorBase::accept(vnx::Visitor& _visitor) const {
	const vnx::TypeCode* _type_code = vnx::search::vnx_native_type_code_PageProcessor;
	_visitor.type_begin(*_type_code);
	_visitor.type_field(_type_code->fields[0], 0); vnx::accept(_visitor, input);
	_visitor.type_field(_type_code->fields[1], 1); vnx::accept(_visitor, page_index_server);
	_visitor.type_field(_type_code->fields[2], 2); vnx::accept(_visitor, page_content_server);
	_visitor.type_field(_type_code->fields[3], 3); vnx::accept(_visitor, max_queue_ms);
	_visitor.type_end(*_type_code);
}

void PageProcessorBase::write(std::ostream& _out) const {
	_out << "{";
	_out << "\"input\": "; vnx::write(_out, input);
	_out << ", \"page_index_server\": "; vnx::write(_out, page_index_server);
	_out << ", \"page_content_server\": "; vnx::write(_out, page_content_server);
	_out << ", \"max_queue_ms\": "; vnx::write(_out, max_queue_ms);
	_out << "}";
}

void PageProcessorBase::read(std::istream& _in) {
	std::map<std::string, std::string> _object;
	vnx::read_object(_in, _object);
	for(const auto& _entry : _object) {
		if(_entry.first == "input") {
			vnx::from_string(_entry.second, input);
		} else if(_entry.first == "max_queue_ms") {
			vnx::from_string(_entry.second, max_queue_ms);
		} else if(_entry.first == "page_content_server") {
			vnx::from_string(_entry.second, page_content_server);
		} else if(_entry.first == "page_index_server") {
			vnx::from_string(_entry.second, page_index_server);
		}
	}
}

vnx::Object PageProcessorBase::to_object() const {
	vnx::Object _object;
	_object["input"] = input;
	_object["page_index_server"] = page_index_server;
	_object["page_content_server"] = page_content_server;
	_object["max_queue_ms"] = max_queue_ms;
	return _object;
}

void PageProcessorBase::from_object(const vnx::Object& _object) {
	for(const auto& _entry : _object.field) {
		if(_entry.first == "input") {
			_entry.second.to(input);
		} else if(_entry.first == "max_queue_ms") {
			_entry.second.to(max_queue_ms);
		} else if(_entry.first == "page_content_server") {
			_entry.second.to(page_content_server);
		} else if(_entry.first == "page_index_server") {
			_entry.second.to(page_index_server);
		}
	}
}

/// \private
std::ostream& operator<<(std::ostream& _out, const PageProcessorBase& _value) {
	_value.write(_out);
	return _out;
}

/// \private
std::istream& operator>>(std::istream& _in, PageProcessorBase& _value) {
	_value.read(_in);
	return _in;
}

const vnx::TypeCode* PageProcessorBase::static_get_type_code() {
	const vnx::TypeCode* type_code = vnx::get_type_code(vnx::Hash64(0xd130b1014d9ffc6full));
	if(!type_code) {
		type_code = vnx::register_type_code(static_create_type_code());
	}
	return type_code;
}

std::shared_ptr<vnx::TypeCode> PageProcessorBase::static_create_type_code() {
	std::shared_ptr<vnx::TypeCode> type_code = std::make_shared<vnx::TypeCode>(true);
	type_code->name = "vnx.search.PageProcessor";
	type_code->type_hash = vnx::Hash64(0xd130b1014d9ffc6full);
	type_code->code_hash = vnx::Hash64(0xe8a78c0aa0ee4d33ull);
	type_code->methods.resize(1);
	{
		std::shared_ptr<vnx::TypeCode> call_type = std::make_shared<vnx::TypeCode>(true);
		call_type->name = "vnx.search.PageProcessor.handle_vnx_search_TextResponse";
		call_type->type_hash = vnx::Hash64(0x1566b79c7e096d3ull);
		call_type->code_hash = vnx::Hash64(0x8e5f5dace99b2ffbull);
		call_type->is_method = true;
		{
			std::shared_ptr<vnx::TypeCode> return_type = std::make_shared<vnx::TypeCode>(true);
			return_type->name = "vnx.search.PageProcessor.handle_vnx_search_TextResponse.return";
			return_type->type_hash = vnx::Hash64(0x4118b7a166ff96c1ull);
			return_type->code_hash = vnx::Hash64(0x1817a5b3263fd1a5ull);
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
	type_code->fields.resize(4);
	{
		vnx::TypeField& field = type_code->fields[0];
		field.is_extended = true;
		field.name = "input";
		field.value = vnx::to_string("frontend.text_responses");
		field.code = {12, 5};
	}
	{
		vnx::TypeField& field = type_code->fields[1];
		field.is_extended = true;
		field.name = "page_index_server";
		field.value = vnx::to_string("PageIndex");
		field.code = {12, 5};
	}
	{
		vnx::TypeField& field = type_code->fields[2];
		field.is_extended = true;
		field.name = "page_content_server";
		field.value = vnx::to_string("PageContent");
		field.code = {12, 5};
	}
	{
		vnx::TypeField& field = type_code->fields[3];
		field.name = "max_queue_ms";
		field.value = vnx::to_string(1000);
		field.code = {7};
	}
	type_code->build();
	return type_code;
}

void PageProcessorBase::vnx_handle_switch(std::shared_ptr<const ::vnx::Sample> _sample) {
	const uint64_t _type_hash = _sample->value->get_type_hash();
	if(_type_hash == 0x7cee1cd5b88ec569ull) {
		std::shared_ptr<const vnx::search::TextResponse> _value = std::dynamic_pointer_cast<const vnx::search::TextResponse>(_sample->value);
		if(_value) {
			handle(_value, _sample);
		}
	}
}

std::shared_ptr<vnx::Value> PageProcessorBase::vnx_call_switch(vnx::TypeInput& _in, const vnx::TypeCode* _call_type, const vnx::request_id_t& _request_id) {
	if(_call_type->type_hash == vnx::Hash64(0x1566b79c7e096d3ull)) {
		::std::shared_ptr<const ::vnx::search::TextResponse> sample;
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
			const vnx::TypeCode* _return_type = vnx::search::vnx_native_type_code_PageProcessor_handle_vnx_search_TextResponse_return;
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

void read(TypeInput& in, ::vnx::search::PageProcessorBase& value, const TypeCode* type_code, const uint16_t* code) {
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
				vnx::read_value(_buf + _field->offset, value.max_queue_ms, _field->code.data());
			}
		}
	}
	for(const vnx::TypeField* _field : type_code->ext_fields) {
		switch(_field->native_index) {
			case 0: vnx::read(in, value.input, type_code, _field->code.data()); break;
			case 1: vnx::read(in, value.page_index_server, type_code, _field->code.data()); break;
			case 2: vnx::read(in, value.page_content_server, type_code, _field->code.data()); break;
			default: vnx::skip(in, type_code, _field->code.data());
		}
	}
}

void write(TypeOutput& out, const ::vnx::search::PageProcessorBase& value, const TypeCode* type_code, const uint16_t* code) {
	if(!type_code || (code && code[0] == CODE_ANY)) {
		type_code = vnx::search::vnx_native_type_code_PageProcessor;
		out.write_type_code(type_code);
		vnx::write_class_header<::vnx::search::PageProcessorBase>(out);
	}
	if(code && code[0] == CODE_STRUCT) {
		type_code = type_code->depends[code[1]];
	}
	char* const _buf = out.write(4);
	vnx::write_value(_buf + 0, value.max_queue_ms);
	vnx::write(out, value.input, type_code, type_code->fields[0].code.data());
	vnx::write(out, value.page_index_server, type_code, type_code->fields[1].code.data());
	vnx::write(out, value.page_content_server, type_code, type_code->fields[2].code.data());
}

void read(std::istream& in, ::vnx::search::PageProcessorBase& value) {
	value.read(in);
}

void write(std::ostream& out, const ::vnx::search::PageProcessorBase& value) {
	value.write(out);
}

void accept(Visitor& visitor, const ::vnx::search::PageProcessorBase& value) {
	value.accept(visitor);
}

} // vnx
