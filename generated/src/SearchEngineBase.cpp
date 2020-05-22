
// AUTO GENERATED by vnxcppcodegen

#include <vnx/search/package.hxx>
#include <vnx/search/SearchEngineBase.hxx>
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


const vnx::Hash64 SearchEngineBase::VNX_TYPE_HASH(0x4e0f26d3496896a1ull);
const vnx::Hash64 SearchEngineBase::VNX_CODE_HASH(0xae2c259dc7017d04ull);

SearchEngineBase::SearchEngineBase(const std::string& _vnx_name)
	:	Module::Module(_vnx_name)
{
	vnx::read_config(vnx_name + ".commit_interval", commit_interval);
	vnx::read_config(vnx_name + ".input_page_index", input_page_index);
	vnx::read_config(vnx_name + ".input_url_index", input_url_index);
	vnx::read_config(vnx_name + ".max_query_pages", max_query_pages);
	vnx::read_config(vnx_name + ".num_query_threads", num_query_threads);
	vnx::read_config(vnx_name + ".num_update_threads", num_update_threads);
	vnx::read_config(vnx_name + ".page_content_server", page_content_server);
	vnx::read_config(vnx_name + ".page_index_server", page_index_server);
	vnx::read_config(vnx_name + ".protocols", protocols);
	vnx::read_config(vnx_name + ".stats_interval_ms", stats_interval_ms);
	vnx::read_config(vnx_name + ".url_index_server", url_index_server);
	vnx::read_config(vnx_name + ".word_power", word_power);
}

vnx::Hash64 SearchEngineBase::get_type_hash() const {
	return VNX_TYPE_HASH;
}

const char* SearchEngineBase::get_type_name() const {
	return "vnx.search.SearchEngine";
}
const vnx::TypeCode* SearchEngineBase::get_type_code() const {
	return vnx::search::vnx_native_type_code_SearchEngine;
}

void SearchEngineBase::accept(vnx::Visitor& _visitor) const {
	const vnx::TypeCode* _type_code = vnx::search::vnx_native_type_code_SearchEngine;
	_visitor.type_begin(*_type_code);
	_visitor.type_field(_type_code->fields[0], 0); vnx::accept(_visitor, input_url_index);
	_visitor.type_field(_type_code->fields[1], 1); vnx::accept(_visitor, input_page_index);
	_visitor.type_field(_type_code->fields[2], 2); vnx::accept(_visitor, url_index_server);
	_visitor.type_field(_type_code->fields[3], 3); vnx::accept(_visitor, page_index_server);
	_visitor.type_field(_type_code->fields[4], 4); vnx::accept(_visitor, page_content_server);
	_visitor.type_field(_type_code->fields[5], 5); vnx::accept(_visitor, max_query_pages);
	_visitor.type_field(_type_code->fields[6], 6); vnx::accept(_visitor, num_query_threads);
	_visitor.type_field(_type_code->fields[7], 7); vnx::accept(_visitor, num_update_threads);
	_visitor.type_field(_type_code->fields[8], 8); vnx::accept(_visitor, commit_interval);
	_visitor.type_field(_type_code->fields[9], 9); vnx::accept(_visitor, stats_interval_ms);
	_visitor.type_field(_type_code->fields[10], 10); vnx::accept(_visitor, word_power);
	_visitor.type_field(_type_code->fields[11], 11); vnx::accept(_visitor, protocols);
	_visitor.type_end(*_type_code);
}

void SearchEngineBase::write(std::ostream& _out) const {
	_out << "{\"__type\": \"vnx.search.SearchEngine\"";
	_out << ", \"input_url_index\": "; vnx::write(_out, input_url_index);
	_out << ", \"input_page_index\": "; vnx::write(_out, input_page_index);
	_out << ", \"url_index_server\": "; vnx::write(_out, url_index_server);
	_out << ", \"page_index_server\": "; vnx::write(_out, page_index_server);
	_out << ", \"page_content_server\": "; vnx::write(_out, page_content_server);
	_out << ", \"max_query_pages\": "; vnx::write(_out, max_query_pages);
	_out << ", \"num_query_threads\": "; vnx::write(_out, num_query_threads);
	_out << ", \"num_update_threads\": "; vnx::write(_out, num_update_threads);
	_out << ", \"commit_interval\": "; vnx::write(_out, commit_interval);
	_out << ", \"stats_interval_ms\": "; vnx::write(_out, stats_interval_ms);
	_out << ", \"word_power\": "; vnx::write(_out, word_power);
	_out << ", \"protocols\": "; vnx::write(_out, protocols);
	_out << "}";
}

void SearchEngineBase::read(std::istream& _in) {
	std::map<std::string, std::string> _object;
	vnx::read_object(_in, _object);
	for(const auto& _entry : _object) {
		if(_entry.first == "commit_interval") {
			vnx::from_string(_entry.second, commit_interval);
		} else if(_entry.first == "input_page_index") {
			vnx::from_string(_entry.second, input_page_index);
		} else if(_entry.first == "input_url_index") {
			vnx::from_string(_entry.second, input_url_index);
		} else if(_entry.first == "max_query_pages") {
			vnx::from_string(_entry.second, max_query_pages);
		} else if(_entry.first == "num_query_threads") {
			vnx::from_string(_entry.second, num_query_threads);
		} else if(_entry.first == "num_update_threads") {
			vnx::from_string(_entry.second, num_update_threads);
		} else if(_entry.first == "page_content_server") {
			vnx::from_string(_entry.second, page_content_server);
		} else if(_entry.first == "page_index_server") {
			vnx::from_string(_entry.second, page_index_server);
		} else if(_entry.first == "protocols") {
			vnx::from_string(_entry.second, protocols);
		} else if(_entry.first == "stats_interval_ms") {
			vnx::from_string(_entry.second, stats_interval_ms);
		} else if(_entry.first == "url_index_server") {
			vnx::from_string(_entry.second, url_index_server);
		} else if(_entry.first == "word_power") {
			vnx::from_string(_entry.second, word_power);
		}
	}
}

vnx::Object SearchEngineBase::to_object() const {
	vnx::Object _object;
	_object["input_url_index"] = input_url_index;
	_object["input_page_index"] = input_page_index;
	_object["url_index_server"] = url_index_server;
	_object["page_index_server"] = page_index_server;
	_object["page_content_server"] = page_content_server;
	_object["max_query_pages"] = max_query_pages;
	_object["num_query_threads"] = num_query_threads;
	_object["num_update_threads"] = num_update_threads;
	_object["commit_interval"] = commit_interval;
	_object["stats_interval_ms"] = stats_interval_ms;
	_object["word_power"] = word_power;
	_object["protocols"] = protocols;
	return _object;
}

void SearchEngineBase::from_object(const vnx::Object& _object) {
	for(const auto& _entry : _object.field) {
		if(_entry.first == "commit_interval") {
			_entry.second.to(commit_interval);
		} else if(_entry.first == "input_page_index") {
			_entry.second.to(input_page_index);
		} else if(_entry.first == "input_url_index") {
			_entry.second.to(input_url_index);
		} else if(_entry.first == "max_query_pages") {
			_entry.second.to(max_query_pages);
		} else if(_entry.first == "num_query_threads") {
			_entry.second.to(num_query_threads);
		} else if(_entry.first == "num_update_threads") {
			_entry.second.to(num_update_threads);
		} else if(_entry.first == "page_content_server") {
			_entry.second.to(page_content_server);
		} else if(_entry.first == "page_index_server") {
			_entry.second.to(page_index_server);
		} else if(_entry.first == "protocols") {
			_entry.second.to(protocols);
		} else if(_entry.first == "stats_interval_ms") {
			_entry.second.to(stats_interval_ms);
		} else if(_entry.first == "url_index_server") {
			_entry.second.to(url_index_server);
		} else if(_entry.first == "word_power") {
			_entry.second.to(word_power);
		}
	}
}

/// \private
std::ostream& operator<<(std::ostream& _out, const SearchEngineBase& _value) {
	_value.write(_out);
	return _out;
}

/// \private
std::istream& operator>>(std::istream& _in, SearchEngineBase& _value) {
	_value.read(_in);
	return _in;
}

const vnx::TypeCode* SearchEngineBase::static_get_type_code() {
	const vnx::TypeCode* type_code = vnx::get_type_code(vnx::Hash64(0x4e0f26d3496896a1ull));
	if(!type_code) {
		type_code = vnx::register_type_code(static_create_type_code());
	}
	return type_code;
}

std::shared_ptr<vnx::TypeCode> SearchEngineBase::static_create_type_code() {
	std::shared_ptr<vnx::TypeCode> type_code = std::make_shared<vnx::TypeCode>();
	type_code->name = "vnx.search.SearchEngine";
	type_code->type_hash = vnx::Hash64(0x4e0f26d3496896a1ull);
	type_code->code_hash = vnx::Hash64(0xae2c259dc7017d04ull);
	type_code->is_native = true;
	type_code->methods.resize(9);
	{
		std::shared_ptr<vnx::TypeCode> call_type = std::make_shared<vnx::TypeCode>();
		call_type->name = "vnx.search.SearchEngine.get_domain_info";
		call_type->type_hash = vnx::Hash64(0xc775a7413dab0511ull);
		call_type->code_hash = vnx::Hash64(0xe9963befdbd05cd9ull);
		call_type->is_native = true;
		call_type->is_method = true;
		{
			std::shared_ptr<vnx::TypeCode> return_type = std::make_shared<vnx::TypeCode>();
			return_type->name = "vnx.search.SearchEngine.get_domain_info.return";
			return_type->type_hash = vnx::Hash64(0xdd84ef2259be7eull);
			return_type->code_hash = vnx::Hash64(0xfc6d8d20c1c22d5aull);
			return_type->is_native = true;
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
		call_type->fields.resize(3);
		{
			vnx::TypeField& field = call_type->fields[0];
			field.is_extended = true;
			field.name = "host";
			field.code = {12, 5};
		}
		{
			vnx::TypeField& field = call_type->fields[1];
			field.name = "limit";
			field.code = {7};
		}
		{
			vnx::TypeField& field = call_type->fields[2];
			field.name = "offset";
			field.code = {3};
		}
		call_type->build();
		type_code->methods[0] = vnx::register_type_code(call_type);
	}
	{
		std::shared_ptr<vnx::TypeCode> call_type = std::make_shared<vnx::TypeCode>();
		call_type->name = "vnx.search.SearchEngine.get_domain_list";
		call_type->type_hash = vnx::Hash64(0x6852b566cb5e7ba5ull);
		call_type->code_hash = vnx::Hash64(0xcbdee5cd0a36010bull);
		call_type->is_native = true;
		call_type->is_method = true;
		{
			std::shared_ptr<vnx::TypeCode> return_type = std::make_shared<vnx::TypeCode>();
			return_type->name = "vnx.search.SearchEngine.get_domain_list.return";
			return_type->type_hash = vnx::Hash64(0xa06392efe70f0679ull);
			return_type->code_hash = vnx::Hash64(0xedabdeebc18c97e7ull);
			return_type->is_native = true;
			return_type->is_return = true;
			return_type->depends.resize(1);
			return_type->depends[0] = ::vnx::search::DomainIndex::static_get_type_code();
			return_type->fields.resize(1);
			{
				vnx::TypeField& field = return_type->fields[0];
				field.is_extended = true;
				field.name = "_ret_0";
				field.code = {12, 19, 0};
			}
			return_type->build();
			call_type->return_type = vnx::register_type_code(return_type);
		}
		call_type->fields.resize(2);
		{
			vnx::TypeField& field = call_type->fields[0];
			field.name = "limit";
			field.code = {7};
		}
		{
			vnx::TypeField& field = call_type->fields[1];
			field.name = "offset";
			field.code = {3};
		}
		call_type->build();
		type_code->methods[1] = vnx::register_type_code(call_type);
	}
	{
		std::shared_ptr<vnx::TypeCode> call_type = std::make_shared<vnx::TypeCode>();
		call_type->name = "vnx.search.SearchEngine.handle_vnx_keyvalue_KeyValuePair";
		call_type->type_hash = vnx::Hash64(0xd31e7941dec9d83aull);
		call_type->code_hash = vnx::Hash64(0x7b64d9ce23fb7bbbull);
		call_type->is_native = true;
		call_type->is_method = true;
		{
			std::shared_ptr<vnx::TypeCode> return_type = std::make_shared<vnx::TypeCode>();
			return_type->name = "vnx.search.SearchEngine.handle_vnx_keyvalue_KeyValuePair.return";
			return_type->type_hash = vnx::Hash64(0x65dba7428d26148bull);
			return_type->code_hash = vnx::Hash64(0x357d3ffaa9c5d422ull);
			return_type->is_native = true;
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
		type_code->methods[2] = vnx::register_type_code(call_type);
	}
	{
		std::shared_ptr<vnx::TypeCode> call_type = std::make_shared<vnx::TypeCode>();
		call_type->name = "vnx.search.SearchEngine.handle_vnx_keyvalue_SyncInfo";
		call_type->type_hash = vnx::Hash64(0x358a7aca8a9f1e94ull);
		call_type->code_hash = vnx::Hash64(0x1ca0a4a37d7c94b5ull);
		call_type->is_native = true;
		call_type->is_method = true;
		{
			std::shared_ptr<vnx::TypeCode> return_type = std::make_shared<vnx::TypeCode>();
			return_type->name = "vnx.search.SearchEngine.handle_vnx_keyvalue_SyncInfo.return";
			return_type->type_hash = vnx::Hash64(0xac7ffe12b596e74ull);
			return_type->code_hash = vnx::Hash64(0x629ac618b3b14668ull);
			return_type->is_native = true;
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
		type_code->methods[3] = vnx::register_type_code(call_type);
	}
	{
		std::shared_ptr<vnx::TypeCode> call_type = std::make_shared<vnx::TypeCode>();
		call_type->name = "vnx.search.SearchEngine.query";
		call_type->type_hash = vnx::Hash64(0x14a6c3ff80018ce8ull);
		call_type->code_hash = vnx::Hash64(0x2b2f92fb22bdefa5ull);
		call_type->is_native = true;
		call_type->is_method = true;
		call_type->depends.resize(1);
		call_type->depends[0] = ::vnx::search::search_flags_e::static_get_type_code();
		{
			std::shared_ptr<vnx::TypeCode> return_type = std::make_shared<vnx::TypeCode>();
			return_type->name = "vnx.search.SearchEngine.query.return";
			return_type->type_hash = vnx::Hash64(0xc4adbd01e9504960ull);
			return_type->code_hash = vnx::Hash64(0xf6398e0c698422aaull);
			return_type->is_native = true;
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
		call_type->fields.resize(4);
		{
			vnx::TypeField& field = call_type->fields[0];
			field.is_extended = true;
			field.name = "words";
			field.code = {12, 12, 5};
		}
		{
			vnx::TypeField& field = call_type->fields[1];
			field.name = "limit";
			field.code = {7};
		}
		{
			vnx::TypeField& field = call_type->fields[2];
			field.name = "offset";
			field.code = {3};
		}
		{
			vnx::TypeField& field = call_type->fields[3];
			field.is_extended = true;
			field.name = "flags";
			field.code = {12, 19, 0};
		}
		call_type->build();
		type_code->methods[4] = vnx::register_type_code(call_type);
	}
	{
		std::shared_ptr<vnx::TypeCode> call_type = std::make_shared<vnx::TypeCode>();
		call_type->name = "vnx.search.SearchEngine.reverse_domain_lookup";
		call_type->type_hash = vnx::Hash64(0x70967f585e137c7dull);
		call_type->code_hash = vnx::Hash64(0x9d296f650060e3cfull);
		call_type->is_native = true;
		call_type->is_method = true;
		{
			std::shared_ptr<vnx::TypeCode> return_type = std::make_shared<vnx::TypeCode>();
			return_type->name = "vnx.search.SearchEngine.reverse_domain_lookup.return";
			return_type->type_hash = vnx::Hash64(0x8f0160842cd83465ull);
			return_type->code_hash = vnx::Hash64(0xe38707770baf6ef1ull);
			return_type->is_native = true;
			return_type->is_return = true;
			return_type->fields.resize(1);
			{
				vnx::TypeField& field = return_type->fields[0];
				field.is_extended = true;
				field.name = "_ret_0";
				field.code = {12, 12, 5};
			}
			return_type->build();
			call_type->return_type = vnx::register_type_code(return_type);
		}
		call_type->fields.resize(1);
		{
			vnx::TypeField& field = call_type->fields[0];
			field.is_extended = true;
			field.name = "url_key";
			field.code = {12, 5};
		}
		call_type->build();
		type_code->methods[5] = vnx::register_type_code(call_type);
	}
	{
		std::shared_ptr<vnx::TypeCode> call_type = std::make_shared<vnx::TypeCode>();
		call_type->name = "vnx.search.SearchEngine.reverse_lookup";
		call_type->type_hash = vnx::Hash64(0x4e3b1cbd5cbd42afull);
		call_type->code_hash = vnx::Hash64(0x6ed0d85be579ded7ull);
		call_type->is_native = true;
		call_type->is_method = true;
		{
			std::shared_ptr<vnx::TypeCode> return_type = std::make_shared<vnx::TypeCode>();
			return_type->name = "vnx.search.SearchEngine.reverse_lookup.return";
			return_type->type_hash = vnx::Hash64(0x12b287ac93bf5253ull);
			return_type->code_hash = vnx::Hash64(0xd006c2696de1ec86ull);
			return_type->is_native = true;
			return_type->is_return = true;
			return_type->fields.resize(1);
			{
				vnx::TypeField& field = return_type->fields[0];
				field.is_extended = true;
				field.name = "_ret_0";
				field.code = {12, 12, 5};
			}
			return_type->build();
			call_type->return_type = vnx::register_type_code(return_type);
		}
		call_type->fields.resize(1);
		{
			vnx::TypeField& field = call_type->fields[0];
			field.is_extended = true;
			field.name = "url_key";
			field.code = {12, 5};
		}
		call_type->build();
		type_code->methods[6] = vnx::register_type_code(call_type);
	}
	{
		std::shared_ptr<vnx::TypeCode> call_type = std::make_shared<vnx::TypeCode>();
		call_type->name = "vnx.search.SearchEngine.suggest_domains";
		call_type->type_hash = vnx::Hash64(0x1abdd70e75f411b3ull);
		call_type->code_hash = vnx::Hash64(0x95c1d8013664b51cull);
		call_type->is_native = true;
		call_type->is_method = true;
		{
			std::shared_ptr<vnx::TypeCode> return_type = std::make_shared<vnx::TypeCode>();
			return_type->name = "vnx.search.SearchEngine.suggest_domains.return";
			return_type->type_hash = vnx::Hash64(0x385aae88fe3df753ull);
			return_type->code_hash = vnx::Hash64(0x8a0fbe919ddca0d8ull);
			return_type->is_native = true;
			return_type->is_return = true;
			return_type->fields.resize(1);
			{
				vnx::TypeField& field = return_type->fields[0];
				field.is_extended = true;
				field.name = "_ret_0";
				field.code = {12, 12, 5};
			}
			return_type->build();
			call_type->return_type = vnx::register_type_code(return_type);
		}
		call_type->fields.resize(2);
		{
			vnx::TypeField& field = call_type->fields[0];
			field.is_extended = true;
			field.name = "prefix";
			field.code = {12, 5};
		}
		{
			vnx::TypeField& field = call_type->fields[1];
			field.name = "limit";
			field.code = {7};
		}
		call_type->build();
		type_code->methods[7] = vnx::register_type_code(call_type);
	}
	{
		std::shared_ptr<vnx::TypeCode> call_type = std::make_shared<vnx::TypeCode>();
		call_type->name = "vnx.search.SearchEngine.suggest_words";
		call_type->type_hash = vnx::Hash64(0x7124d7bfda1b31f2ull);
		call_type->code_hash = vnx::Hash64(0xd1e53a9be6fe4469ull);
		call_type->is_native = true;
		call_type->is_method = true;
		{
			std::shared_ptr<vnx::TypeCode> return_type = std::make_shared<vnx::TypeCode>();
			return_type->name = "vnx.search.SearchEngine.suggest_words.return";
			return_type->type_hash = vnx::Hash64(0xcf74c35d3594cc90ull);
			return_type->code_hash = vnx::Hash64(0x212693bdbecedcd1ull);
			return_type->is_native = true;
			return_type->is_return = true;
			return_type->fields.resize(1);
			{
				vnx::TypeField& field = return_type->fields[0];
				field.is_extended = true;
				field.name = "_ret_0";
				field.code = {12, 12, 5};
			}
			return_type->build();
			call_type->return_type = vnx::register_type_code(return_type);
		}
		call_type->fields.resize(2);
		{
			vnx::TypeField& field = call_type->fields[0];
			field.is_extended = true;
			field.name = "prefix";
			field.code = {12, 5};
		}
		{
			vnx::TypeField& field = call_type->fields[1];
			field.name = "limit";
			field.code = {7};
		}
		call_type->build();
		type_code->methods[8] = vnx::register_type_code(call_type);
	}
	type_code->fields.resize(12);
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
		field.name = "url_index_server";
		field.value = vnx::to_string("UrlIndex");
		field.code = {12, 5};
	}
	{
		vnx::TypeField& field = type_code->fields[3];
		field.is_extended = true;
		field.name = "page_index_server";
		field.value = vnx::to_string("PageIndex");
		field.code = {12, 5};
	}
	{
		vnx::TypeField& field = type_code->fields[4];
		field.is_extended = true;
		field.name = "page_content_server";
		field.value = vnx::to_string("PageContent");
		field.code = {12, 5};
	}
	{
		vnx::TypeField& field = type_code->fields[5];
		field.name = "max_query_pages";
		field.value = vnx::to_string(1000);
		field.code = {7};
	}
	{
		vnx::TypeField& field = type_code->fields[6];
		field.name = "num_query_threads";
		field.value = vnx::to_string(4);
		field.code = {7};
	}
	{
		vnx::TypeField& field = type_code->fields[7];
		field.name = "num_update_threads";
		field.value = vnx::to_string(4);
		field.code = {7};
	}
	{
		vnx::TypeField& field = type_code->fields[8];
		field.name = "commit_interval";
		field.value = vnx::to_string(3600);
		field.code = {7};
	}
	{
		vnx::TypeField& field = type_code->fields[9];
		field.name = "stats_interval_ms";
		field.value = vnx::to_string(10000);
		field.code = {7};
	}
	{
		vnx::TypeField& field = type_code->fields[10];
		field.name = "word_power";
		field.value = vnx::to_string(1);
		field.code = {9};
	}
	{
		vnx::TypeField& field = type_code->fields[11];
		field.is_extended = true;
		field.name = "protocols";
		field.code = {12, 12, 5};
	}
	type_code->build();
	return type_code;
}

void SearchEngineBase::vnx_handle_switch(std::shared_ptr<const ::vnx::Sample> _sample) {
	const uint64_t _type_hash = _sample->value->get_type_hash();
	if(_type_hash == 0xf87436237449d8afull) {
		std::shared_ptr<const vnx::keyvalue::KeyValuePair> _value = std::dynamic_pointer_cast<const vnx::keyvalue::KeyValuePair>(_sample->value);
		if(_value) {
			handle(_value, _sample);
		}
	} else if(_type_hash == 0x4f9820ae95813502ull) {
		std::shared_ptr<const vnx::keyvalue::SyncInfo> _value = std::dynamic_pointer_cast<const vnx::keyvalue::SyncInfo>(_sample->value);
		if(_value) {
			handle(_value, _sample);
		}
	}
}

std::shared_ptr<vnx::Value> SearchEngineBase::vnx_call_switch(vnx::TypeInput& _in, const vnx::TypeCode* _call_type, const vnx::request_id_t& _request_id) {
	if(_call_type->type_hash == vnx::Hash64(0xc775a7413dab0511ull)) {
		::std::string host;
		::int32_t limit = 0;
		::uint32_t offset = 0;
		{
			const char* const _buf = _in.read(_call_type->total_field_size);
			if(_call_type->is_matched) {
				{
					const vnx::TypeField* const _field = _call_type->field_map[1];
					if(_field) {
						vnx::read_value(_buf + _field->offset, limit, _field->code.data());
					}
				}
				{
					const vnx::TypeField* const _field = _call_type->field_map[2];
					if(_field) {
						vnx::read_value(_buf + _field->offset, offset, _field->code.data());
					}
				}
			}
			for(const vnx::TypeField* _field : _call_type->ext_fields) {
				switch(_field->native_index) {
					case 0: vnx::read(_in, host, _call_type, _field->code.data()); break;
					default: vnx::skip(_in, _call_type, _field->code.data());
				}
			}
		}
		::std::shared_ptr<const ::vnx::search::DomainIndex> _ret_0;
		_ret_0 = get_domain_info(host, limit, offset);
		std::shared_ptr<vnx::Binary> _return_value;
		{
			const vnx::TypeCode* _return_type = vnx::search::vnx_native_type_code_SearchEngine_get_domain_info_return;
			_return_value = vnx::Binary::create();
			_return_value->type_code = _return_type;
			vnx::BinaryOutputStream _stream_out(_return_value.get());
			vnx::TypeOutput _out(&_stream_out);
			vnx::write(_out, _ret_0, _return_type, _return_type->fields[0].code.data());
			_out.flush();
		}
		return _return_value;
	} else if(_call_type->type_hash == vnx::Hash64(0x6852b566cb5e7ba5ull)) {
		::int32_t limit = 0;
		::uint32_t offset = 0;
		{
			const char* const _buf = _in.read(_call_type->total_field_size);
			if(_call_type->is_matched) {
				{
					const vnx::TypeField* const _field = _call_type->field_map[0];
					if(_field) {
						vnx::read_value(_buf + _field->offset, limit, _field->code.data());
					}
				}
				{
					const vnx::TypeField* const _field = _call_type->field_map[1];
					if(_field) {
						vnx::read_value(_buf + _field->offset, offset, _field->code.data());
					}
				}
			}
			for(const vnx::TypeField* _field : _call_type->ext_fields) {
				switch(_field->native_index) {
					default: vnx::skip(_in, _call_type, _field->code.data());
				}
			}
		}
		::std::vector<::vnx::search::DomainIndex> _ret_0;
		_ret_0 = get_domain_list(limit, offset);
		std::shared_ptr<vnx::Binary> _return_value;
		{
			const vnx::TypeCode* _return_type = vnx::search::vnx_native_type_code_SearchEngine_get_domain_list_return;
			_return_value = vnx::Binary::create();
			_return_value->type_code = _return_type;
			vnx::BinaryOutputStream _stream_out(_return_value.get());
			vnx::TypeOutput _out(&_stream_out);
			vnx::write(_out, _ret_0, _return_type, _return_type->fields[0].code.data());
			_out.flush();
		}
		return _return_value;
	} else if(_call_type->type_hash == vnx::Hash64(0xd31e7941dec9d83aull)) {
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
			const vnx::TypeCode* _return_type = vnx::search::vnx_native_type_code_SearchEngine_handle_vnx_keyvalue_KeyValuePair_return;
			_return_value = vnx::Binary::create();
			_return_value->type_code = _return_type;
		}
		return _return_value;
	} else if(_call_type->type_hash == vnx::Hash64(0x358a7aca8a9f1e94ull)) {
		::std::shared_ptr<const ::vnx::keyvalue::SyncInfo> sample;
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
			const vnx::TypeCode* _return_type = vnx::search::vnx_native_type_code_SearchEngine_handle_vnx_keyvalue_SyncInfo_return;
			_return_value = vnx::Binary::create();
			_return_value->type_code = _return_type;
		}
		return _return_value;
	} else if(_call_type->type_hash == vnx::Hash64(0x14a6c3ff80018ce8ull)) {
		::std::vector<::std::string> words;
		::int32_t limit = 0;
		::uint32_t offset = 0;
		::std::vector<::vnx::search::search_flags_e> flags;
		{
			const char* const _buf = _in.read(_call_type->total_field_size);
			if(_call_type->is_matched) {
				{
					const vnx::TypeField* const _field = _call_type->field_map[1];
					if(_field) {
						vnx::read_value(_buf + _field->offset, limit, _field->code.data());
					}
				}
				{
					const vnx::TypeField* const _field = _call_type->field_map[2];
					if(_field) {
						vnx::read_value(_buf + _field->offset, offset, _field->code.data());
					}
				}
			}
			for(const vnx::TypeField* _field : _call_type->ext_fields) {
				switch(_field->native_index) {
					case 0: vnx::read(_in, words, _call_type, _field->code.data()); break;
					case 3: vnx::read(_in, flags, _call_type, _field->code.data()); break;
					default: vnx::skip(_in, _call_type, _field->code.data());
				}
			}
		}
		query_async(words, limit, offset, flags, std::bind(&SearchEngineBase::query_async_return, this, _request_id, std::placeholders::_1), _request_id);
		return 0;
	} else if(_call_type->type_hash == vnx::Hash64(0x70967f585e137c7dull)) {
		::std::string url_key;
		{
			const char* const _buf = _in.read(_call_type->total_field_size);
			if(_call_type->is_matched) {
			}
			for(const vnx::TypeField* _field : _call_type->ext_fields) {
				switch(_field->native_index) {
					case 0: vnx::read(_in, url_key, _call_type, _field->code.data()); break;
					default: vnx::skip(_in, _call_type, _field->code.data());
				}
			}
		}
		::std::vector<::std::string> _ret_0;
		_ret_0 = reverse_domain_lookup(url_key);
		std::shared_ptr<vnx::Binary> _return_value;
		{
			const vnx::TypeCode* _return_type = vnx::search::vnx_native_type_code_SearchEngine_reverse_domain_lookup_return;
			_return_value = vnx::Binary::create();
			_return_value->type_code = _return_type;
			vnx::BinaryOutputStream _stream_out(_return_value.get());
			vnx::TypeOutput _out(&_stream_out);
			vnx::write(_out, _ret_0, _return_type, _return_type->fields[0].code.data());
			_out.flush();
		}
		return _return_value;
	} else if(_call_type->type_hash == vnx::Hash64(0x4e3b1cbd5cbd42afull)) {
		::std::string url_key;
		{
			const char* const _buf = _in.read(_call_type->total_field_size);
			if(_call_type->is_matched) {
			}
			for(const vnx::TypeField* _field : _call_type->ext_fields) {
				switch(_field->native_index) {
					case 0: vnx::read(_in, url_key, _call_type, _field->code.data()); break;
					default: vnx::skip(_in, _call_type, _field->code.data());
				}
			}
		}
		::std::vector<::std::string> _ret_0;
		_ret_0 = reverse_lookup(url_key);
		std::shared_ptr<vnx::Binary> _return_value;
		{
			const vnx::TypeCode* _return_type = vnx::search::vnx_native_type_code_SearchEngine_reverse_lookup_return;
			_return_value = vnx::Binary::create();
			_return_value->type_code = _return_type;
			vnx::BinaryOutputStream _stream_out(_return_value.get());
			vnx::TypeOutput _out(&_stream_out);
			vnx::write(_out, _ret_0, _return_type, _return_type->fields[0].code.data());
			_out.flush();
		}
		return _return_value;
	} else if(_call_type->type_hash == vnx::Hash64(0x1abdd70e75f411b3ull)) {
		::std::string prefix;
		::int32_t limit = 0;
		{
			const char* const _buf = _in.read(_call_type->total_field_size);
			if(_call_type->is_matched) {
				{
					const vnx::TypeField* const _field = _call_type->field_map[1];
					if(_field) {
						vnx::read_value(_buf + _field->offset, limit, _field->code.data());
					}
				}
			}
			for(const vnx::TypeField* _field : _call_type->ext_fields) {
				switch(_field->native_index) {
					case 0: vnx::read(_in, prefix, _call_type, _field->code.data()); break;
					default: vnx::skip(_in, _call_type, _field->code.data());
				}
			}
		}
		::std::vector<::std::string> _ret_0;
		_ret_0 = suggest_domains(prefix, limit);
		std::shared_ptr<vnx::Binary> _return_value;
		{
			const vnx::TypeCode* _return_type = vnx::search::vnx_native_type_code_SearchEngine_suggest_domains_return;
			_return_value = vnx::Binary::create();
			_return_value->type_code = _return_type;
			vnx::BinaryOutputStream _stream_out(_return_value.get());
			vnx::TypeOutput _out(&_stream_out);
			vnx::write(_out, _ret_0, _return_type, _return_type->fields[0].code.data());
			_out.flush();
		}
		return _return_value;
	} else if(_call_type->type_hash == vnx::Hash64(0x7124d7bfda1b31f2ull)) {
		::std::string prefix;
		::int32_t limit = 0;
		{
			const char* const _buf = _in.read(_call_type->total_field_size);
			if(_call_type->is_matched) {
				{
					const vnx::TypeField* const _field = _call_type->field_map[1];
					if(_field) {
						vnx::read_value(_buf + _field->offset, limit, _field->code.data());
					}
				}
			}
			for(const vnx::TypeField* _field : _call_type->ext_fields) {
				switch(_field->native_index) {
					case 0: vnx::read(_in, prefix, _call_type, _field->code.data()); break;
					default: vnx::skip(_in, _call_type, _field->code.data());
				}
			}
		}
		::std::vector<::std::string> _ret_0;
		_ret_0 = suggest_words(prefix, limit);
		std::shared_ptr<vnx::Binary> _return_value;
		{
			const vnx::TypeCode* _return_type = vnx::search::vnx_native_type_code_SearchEngine_suggest_words_return;
			_return_value = vnx::Binary::create();
			_return_value->type_code = _return_type;
			vnx::BinaryOutputStream _stream_out(_return_value.get());
			vnx::TypeOutput _out(&_stream_out);
			vnx::write(_out, _ret_0, _return_type, _return_type->fields[0].code.data());
			_out.flush();
		}
		return _return_value;
	}
	auto _ex = vnx::NoSuchMethod::create();
	_ex->dst_mac = vnx_request ? vnx_request->dst_mac : 0;
	_ex->method = _call_type->name;
	return _ex;
}

void SearchEngineBase::query_async_return(const vnx::request_id_t& _request_id, const ::std::shared_ptr<const ::vnx::search::SearchResult>& _ret_0) {
	std::shared_ptr<vnx::Binary> _return_value;
	const vnx::TypeCode* _return_type = vnx::search::vnx_native_type_code_SearchEngine_query_return;
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

void read(TypeInput& in, ::vnx::search::SearchEngineBase& value, const TypeCode* type_code, const uint16_t* code) {
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
			const vnx::TypeField* const _field = type_code->field_map[5];
			if(_field) {
				vnx::read_value(_buf + _field->offset, value.max_query_pages, _field->code.data());
			}
		}
		{
			const vnx::TypeField* const _field = type_code->field_map[6];
			if(_field) {
				vnx::read_value(_buf + _field->offset, value.num_query_threads, _field->code.data());
			}
		}
		{
			const vnx::TypeField* const _field = type_code->field_map[7];
			if(_field) {
				vnx::read_value(_buf + _field->offset, value.num_update_threads, _field->code.data());
			}
		}
		{
			const vnx::TypeField* const _field = type_code->field_map[8];
			if(_field) {
				vnx::read_value(_buf + _field->offset, value.commit_interval, _field->code.data());
			}
		}
		{
			const vnx::TypeField* const _field = type_code->field_map[9];
			if(_field) {
				vnx::read_value(_buf + _field->offset, value.stats_interval_ms, _field->code.data());
			}
		}
		{
			const vnx::TypeField* const _field = type_code->field_map[10];
			if(_field) {
				vnx::read_value(_buf + _field->offset, value.word_power, _field->code.data());
			}
		}
	}
	for(const vnx::TypeField* _field : type_code->ext_fields) {
		switch(_field->native_index) {
			case 0: vnx::read(in, value.input_url_index, type_code, _field->code.data()); break;
			case 1: vnx::read(in, value.input_page_index, type_code, _field->code.data()); break;
			case 2: vnx::read(in, value.url_index_server, type_code, _field->code.data()); break;
			case 3: vnx::read(in, value.page_index_server, type_code, _field->code.data()); break;
			case 4: vnx::read(in, value.page_content_server, type_code, _field->code.data()); break;
			case 11: vnx::read(in, value.protocols, type_code, _field->code.data()); break;
			default: vnx::skip(in, type_code, _field->code.data());
		}
	}
}

void write(TypeOutput& out, const ::vnx::search::SearchEngineBase& value, const TypeCode* type_code, const uint16_t* code) {
	if(!type_code || (code && code[0] == CODE_ANY)) {
		type_code = vnx::search::vnx_native_type_code_SearchEngine;
		out.write_type_code(type_code);
		vnx::write_class_header<::vnx::search::SearchEngineBase>(out);
	}
	if(code && code[0] == CODE_STRUCT) {
		type_code = type_code->depends[code[1]];
	}
	char* const _buf = out.write(24);
	vnx::write_value(_buf + 0, value.max_query_pages);
	vnx::write_value(_buf + 4, value.num_query_threads);
	vnx::write_value(_buf + 8, value.num_update_threads);
	vnx::write_value(_buf + 12, value.commit_interval);
	vnx::write_value(_buf + 16, value.stats_interval_ms);
	vnx::write_value(_buf + 20, value.word_power);
	vnx::write(out, value.input_url_index, type_code, type_code->fields[0].code.data());
	vnx::write(out, value.input_page_index, type_code, type_code->fields[1].code.data());
	vnx::write(out, value.url_index_server, type_code, type_code->fields[2].code.data());
	vnx::write(out, value.page_index_server, type_code, type_code->fields[3].code.data());
	vnx::write(out, value.page_content_server, type_code, type_code->fields[4].code.data());
	vnx::write(out, value.protocols, type_code, type_code->fields[11].code.data());
}

void read(std::istream& in, ::vnx::search::SearchEngineBase& value) {
	value.read(in);
}

void write(std::ostream& out, const ::vnx::search::SearchEngineBase& value) {
	value.write(out);
}

void accept(Visitor& visitor, const ::vnx::search::SearchEngineBase& value) {
	value.accept(visitor);
}

} // vnx
