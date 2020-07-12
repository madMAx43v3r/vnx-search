
// AUTO GENERATED by vnxcppcodegen

#include <vnx/search/package.hxx>
#include <vnx/search/SearchEngineBase.hxx>
#include <vnx/NoSuchMethod.hxx>
#include <vnx/Module.h>
#include <vnx/ModuleInterface_vnx_get_type_code.hxx>
#include <vnx/ModuleInterface_vnx_get_type_code_return.hxx>
#include <vnx/Object.hpp>
#include <vnx/TopicPtr.hpp>
#include <vnx/keyvalue/SyncInfo.hxx>
#include <vnx/keyvalue/SyncUpdate.hxx>
#include <vnx/search/SearchEngine_get_domain_info.hxx>
#include <vnx/search/SearchEngine_get_domain_info_return.hxx>
#include <vnx/search/SearchEngine_get_domain_list.hxx>
#include <vnx/search/SearchEngine_get_domain_list_return.hxx>
#include <vnx/search/SearchEngine_get_page_info.hxx>
#include <vnx/search/SearchEngine_get_page_info_return.hxx>
#include <vnx/search/SearchEngine_query.hxx>
#include <vnx/search/SearchEngine_query_return.hxx>
#include <vnx/search/SearchEngine_reverse_domain_lookup.hxx>
#include <vnx/search/SearchEngine_reverse_domain_lookup_return.hxx>
#include <vnx/search/SearchEngine_reverse_lookup.hxx>
#include <vnx/search/SearchEngine_reverse_lookup_return.hxx>
#include <vnx/search/SearchEngine_suggest_domains.hxx>
#include <vnx/search/SearchEngine_suggest_domains_return.hxx>
#include <vnx/search/SearchEngine_suggest_words.hxx>
#include <vnx/search/SearchEngine_suggest_words_return.hxx>
#include <vnx/search/SearchResult.hxx>
#include <vnx/search/search_flags_e.hxx>

#include <vnx/vnx.h>


namespace vnx {
namespace search {


const vnx::Hash64 SearchEngineBase::VNX_TYPE_HASH(0x4e0f26d3496896a1ull);
const vnx::Hash64 SearchEngineBase::VNX_CODE_HASH(0x12bc2d4e7c23788aull);

SearchEngineBase::SearchEngineBase(const std::string& _vnx_name)
	:	Module::Module(_vnx_name)
{
	vnx::read_config(vnx_name + ".input_page_index", input_page_index);
	vnx::read_config(vnx_name + ".input_url_index", input_url_index);
	vnx::read_config(vnx_name + ".link_commit_interval", link_commit_interval);
	vnx::read_config(vnx_name + ".lock_timeout", lock_timeout);
	vnx::read_config(vnx_name + ".max_num_pending", max_num_pending);
	vnx::read_config(vnx_name + ".max_query_pages", max_query_pages);
	vnx::read_config(vnx_name + ".max_word_cache", max_word_cache);
	vnx::read_config(vnx_name + ".num_query_threads", num_query_threads);
	vnx::read_config(vnx_name + ".num_update_threads", num_update_threads);
	vnx::read_config(vnx_name + ".page_content_server", page_content_server);
	vnx::read_config(vnx_name + ".page_index_server", page_index_server);
	vnx::read_config(vnx_name + ".protocols", protocols);
	vnx::read_config(vnx_name + ".stats_interval_ms", stats_interval_ms);
	vnx::read_config(vnx_name + ".url_index_server", url_index_server);
	vnx::read_config(vnx_name + ".word_commit_interval", word_commit_interval);
	vnx::read_config(vnx_name + ".word_cutoff", word_cutoff);
	vnx::read_config(vnx_name + ".word_power", word_power);
}

vnx::Hash64 SearchEngineBase::get_type_hash() const {
	return VNX_TYPE_HASH;
}

const char* SearchEngineBase::get_type_name() const {
	return "vnx.search.SearchEngine";
}
const vnx::TypeCode* SearchEngineBase::get_type_code() const {
	return vnx::search::vnx_native_type_code_SearchEngineBase;
}

void SearchEngineBase::accept(vnx::Visitor& _visitor) const {
	const vnx::TypeCode* _type_code = vnx::search::vnx_native_type_code_SearchEngineBase;
	_visitor.type_begin(*_type_code);
	_visitor.type_field(_type_code->fields[0], 0); vnx::accept(_visitor, input_url_index);
	_visitor.type_field(_type_code->fields[1], 1); vnx::accept(_visitor, input_page_index);
	_visitor.type_field(_type_code->fields[2], 2); vnx::accept(_visitor, url_index_server);
	_visitor.type_field(_type_code->fields[3], 3); vnx::accept(_visitor, page_index_server);
	_visitor.type_field(_type_code->fields[4], 4); vnx::accept(_visitor, page_content_server);
	_visitor.type_field(_type_code->fields[5], 5); vnx::accept(_visitor, max_query_pages);
	_visitor.type_field(_type_code->fields[6], 6); vnx::accept(_visitor, max_word_cache);
	_visitor.type_field(_type_code->fields[7], 7); vnx::accept(_visitor, max_num_pending);
	_visitor.type_field(_type_code->fields[8], 8); vnx::accept(_visitor, num_query_threads);
	_visitor.type_field(_type_code->fields[9], 9); vnx::accept(_visitor, num_update_threads);
	_visitor.type_field(_type_code->fields[10], 10); vnx::accept(_visitor, link_commit_interval);
	_visitor.type_field(_type_code->fields[11], 11); vnx::accept(_visitor, word_commit_interval);
	_visitor.type_field(_type_code->fields[12], 12); vnx::accept(_visitor, lock_timeout);
	_visitor.type_field(_type_code->fields[13], 13); vnx::accept(_visitor, stats_interval_ms);
	_visitor.type_field(_type_code->fields[14], 14); vnx::accept(_visitor, word_cutoff);
	_visitor.type_field(_type_code->fields[15], 15); vnx::accept(_visitor, word_power);
	_visitor.type_field(_type_code->fields[16], 16); vnx::accept(_visitor, protocols);
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
	_out << ", \"max_word_cache\": "; vnx::write(_out, max_word_cache);
	_out << ", \"max_num_pending\": "; vnx::write(_out, max_num_pending);
	_out << ", \"num_query_threads\": "; vnx::write(_out, num_query_threads);
	_out << ", \"num_update_threads\": "; vnx::write(_out, num_update_threads);
	_out << ", \"link_commit_interval\": "; vnx::write(_out, link_commit_interval);
	_out << ", \"word_commit_interval\": "; vnx::write(_out, word_commit_interval);
	_out << ", \"lock_timeout\": "; vnx::write(_out, lock_timeout);
	_out << ", \"stats_interval_ms\": "; vnx::write(_out, stats_interval_ms);
	_out << ", \"word_cutoff\": "; vnx::write(_out, word_cutoff);
	_out << ", \"word_power\": "; vnx::write(_out, word_power);
	_out << ", \"protocols\": "; vnx::write(_out, protocols);
	_out << "}";
}

void SearchEngineBase::read(std::istream& _in) {
	std::map<std::string, std::string> _object;
	vnx::read_object(_in, _object);
	for(const auto& _entry : _object) {
		if(_entry.first == "input_page_index") {
			vnx::from_string(_entry.second, input_page_index);
		} else if(_entry.first == "input_url_index") {
			vnx::from_string(_entry.second, input_url_index);
		} else if(_entry.first == "link_commit_interval") {
			vnx::from_string(_entry.second, link_commit_interval);
		} else if(_entry.first == "lock_timeout") {
			vnx::from_string(_entry.second, lock_timeout);
		} else if(_entry.first == "max_num_pending") {
			vnx::from_string(_entry.second, max_num_pending);
		} else if(_entry.first == "max_query_pages") {
			vnx::from_string(_entry.second, max_query_pages);
		} else if(_entry.first == "max_word_cache") {
			vnx::from_string(_entry.second, max_word_cache);
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
		} else if(_entry.first == "word_commit_interval") {
			vnx::from_string(_entry.second, word_commit_interval);
		} else if(_entry.first == "word_cutoff") {
			vnx::from_string(_entry.second, word_cutoff);
		} else if(_entry.first == "word_power") {
			vnx::from_string(_entry.second, word_power);
		}
	}
}

vnx::Object SearchEngineBase::to_object() const {
	vnx::Object _object;
	_object["__type"] = "vnx.search.SearchEngine";
	_object["input_url_index"] = input_url_index;
	_object["input_page_index"] = input_page_index;
	_object["url_index_server"] = url_index_server;
	_object["page_index_server"] = page_index_server;
	_object["page_content_server"] = page_content_server;
	_object["max_query_pages"] = max_query_pages;
	_object["max_word_cache"] = max_word_cache;
	_object["max_num_pending"] = max_num_pending;
	_object["num_query_threads"] = num_query_threads;
	_object["num_update_threads"] = num_update_threads;
	_object["link_commit_interval"] = link_commit_interval;
	_object["word_commit_interval"] = word_commit_interval;
	_object["lock_timeout"] = lock_timeout;
	_object["stats_interval_ms"] = stats_interval_ms;
	_object["word_cutoff"] = word_cutoff;
	_object["word_power"] = word_power;
	_object["protocols"] = protocols;
	return _object;
}

void SearchEngineBase::from_object(const vnx::Object& _object) {
	for(const auto& _entry : _object.field) {
		if(_entry.first == "input_page_index") {
			_entry.second.to(input_page_index);
		} else if(_entry.first == "input_url_index") {
			_entry.second.to(input_url_index);
		} else if(_entry.first == "link_commit_interval") {
			_entry.second.to(link_commit_interval);
		} else if(_entry.first == "lock_timeout") {
			_entry.second.to(lock_timeout);
		} else if(_entry.first == "max_num_pending") {
			_entry.second.to(max_num_pending);
		} else if(_entry.first == "max_query_pages") {
			_entry.second.to(max_query_pages);
		} else if(_entry.first == "max_word_cache") {
			_entry.second.to(max_word_cache);
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
		} else if(_entry.first == "word_commit_interval") {
			_entry.second.to(word_commit_interval);
		} else if(_entry.first == "word_cutoff") {
			_entry.second.to(word_cutoff);
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
	const vnx::TypeCode* type_code = vnx::get_type_code(VNX_TYPE_HASH);
	if(!type_code) {
		type_code = vnx::register_type_code(static_create_type_code());
	}
	return type_code;
}

std::shared_ptr<vnx::TypeCode> SearchEngineBase::static_create_type_code() {
	std::shared_ptr<vnx::TypeCode> type_code = std::make_shared<vnx::TypeCode>();
	type_code->name = "vnx.search.SearchEngine";
	type_code->type_hash = vnx::Hash64(0x4e0f26d3496896a1ull);
	type_code->code_hash = vnx::Hash64(0x12bc2d4e7c23788aull);
	type_code->is_native = true;
	type_code->methods.resize(9);
	type_code->methods[0] = ::vnx::ModuleInterface_vnx_get_type_code::static_get_type_code();
	type_code->methods[1] = ::vnx::search::SearchEngine_query::static_get_type_code();
	type_code->methods[2] = ::vnx::search::SearchEngine_get_domain_info::static_get_type_code();
	type_code->methods[3] = ::vnx::search::SearchEngine_get_page_info::static_get_type_code();
	type_code->methods[4] = ::vnx::search::SearchEngine_get_domain_list::static_get_type_code();
	type_code->methods[5] = ::vnx::search::SearchEngine_reverse_lookup::static_get_type_code();
	type_code->methods[6] = ::vnx::search::SearchEngine_reverse_domain_lookup::static_get_type_code();
	type_code->methods[7] = ::vnx::search::SearchEngine_suggest_words::static_get_type_code();
	type_code->methods[8] = ::vnx::search::SearchEngine_suggest_domains::static_get_type_code();
	type_code->fields.resize(17);
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
		field.value = vnx::to_string("backend.page_index.key_updates");
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
		field.name = "max_word_cache";
		field.value = vnx::to_string(1000000);
		field.code = {7};
	}
	{
		vnx::TypeField& field = type_code->fields[7];
		field.name = "max_num_pending";
		field.value = vnx::to_string(100);
		field.code = {7};
	}
	{
		vnx::TypeField& field = type_code->fields[8];
		field.name = "num_query_threads";
		field.value = vnx::to_string(4);
		field.code = {7};
	}
	{
		vnx::TypeField& field = type_code->fields[9];
		field.name = "num_update_threads";
		field.value = vnx::to_string(4);
		field.code = {7};
	}
	{
		vnx::TypeField& field = type_code->fields[10];
		field.name = "link_commit_interval";
		field.value = vnx::to_string(60);
		field.code = {7};
	}
	{
		vnx::TypeField& field = type_code->fields[11];
		field.name = "word_commit_interval";
		field.value = vnx::to_string(3600);
		field.code = {7};
	}
	{
		vnx::TypeField& field = type_code->fields[12];
		field.name = "lock_timeout";
		field.value = vnx::to_string(100);
		field.code = {7};
	}
	{
		vnx::TypeField& field = type_code->fields[13];
		field.name = "stats_interval_ms";
		field.value = vnx::to_string(10000);
		field.code = {7};
	}
	{
		vnx::TypeField& field = type_code->fields[14];
		field.name = "word_cutoff";
		field.value = vnx::to_string(100);
		field.code = {7};
	}
	{
		vnx::TypeField& field = type_code->fields[15];
		field.name = "word_power";
		field.value = vnx::to_string(1);
		field.code = {9};
	}
	{
		vnx::TypeField& field = type_code->fields[16];
		field.is_extended = true;
		field.name = "protocols";
		field.code = {12, 12, 5};
	}
	type_code->build();
	return type_code;
}

void SearchEngineBase::vnx_handle_switch(std::shared_ptr<const vnx::Sample> _sample) {
	{
		auto _value = std::dynamic_pointer_cast<const ::vnx::keyvalue::SyncUpdate>(_sample->value);
		if(_value) {
			handle(_value, _sample);
			return;
		}
	}
	{
		auto _value = std::dynamic_pointer_cast<const ::vnx::keyvalue::SyncInfo>(_sample->value);
		if(_value) {
			handle(_value, _sample);
			return;
		}
	}
}

std::shared_ptr<vnx::Value> SearchEngineBase::vnx_call_switch(std::shared_ptr<const vnx::Value> _method, const vnx::request_id_t& _request_id) {
	const auto _type_hash = _method->get_type_hash();
	if(_type_hash == vnx::Hash64(0x305ec4d628960e5dull)) {
		auto _args = std::dynamic_pointer_cast<const ::vnx::ModuleInterface_vnx_get_type_code>(_method);
		if(!_args) {
			throw std::logic_error("vnx_call_switch(): !_args");
		}
		auto _return_value = ::vnx::ModuleInterface_vnx_get_type_code_return::create();
		_return_value->_ret_0 = vnx_get_type_code();
		return _return_value;
	} else if(_type_hash == vnx::Hash64(0x14a6c3ff80018ce8ull)) {
		auto _args = std::dynamic_pointer_cast<const ::vnx::search::SearchEngine_query>(_method);
		if(!_args) {
			throw std::logic_error("vnx_call_switch(): !_args");
		}
		query_async(_args->words, _args->limit, _args->offset, _args->flags, _request_id);
		return 0;
	} else if(_type_hash == vnx::Hash64(0xc775a7413dab0511ull)) {
		auto _args = std::dynamic_pointer_cast<const ::vnx::search::SearchEngine_get_domain_info>(_method);
		if(!_args) {
			throw std::logic_error("vnx_call_switch(): !_args");
		}
		auto _return_value = ::vnx::search::SearchEngine_get_domain_info_return::create();
		_return_value->_ret_0 = get_domain_info(_args->host, _args->limit, _args->offset);
		return _return_value;
	} else if(_type_hash == vnx::Hash64(0x8f4b73a7bd7a8effull)) {
		auto _args = std::dynamic_pointer_cast<const ::vnx::search::SearchEngine_get_page_info>(_method);
		if(!_args) {
			throw std::logic_error("vnx_call_switch(): !_args");
		}
		get_page_info_async(_args->url_key, _request_id);
		return 0;
	} else if(_type_hash == vnx::Hash64(0x6852b566cb5e7ba5ull)) {
		auto _args = std::dynamic_pointer_cast<const ::vnx::search::SearchEngine_get_domain_list>(_method);
		if(!_args) {
			throw std::logic_error("vnx_call_switch(): !_args");
		}
		auto _return_value = ::vnx::search::SearchEngine_get_domain_list_return::create();
		_return_value->_ret_0 = get_domain_list(_args->limit, _args->offset);
		return _return_value;
	} else if(_type_hash == vnx::Hash64(0x4e3b1cbd5cbd42afull)) {
		auto _args = std::dynamic_pointer_cast<const ::vnx::search::SearchEngine_reverse_lookup>(_method);
		if(!_args) {
			throw std::logic_error("vnx_call_switch(): !_args");
		}
		reverse_lookup_async(_args->url_key, _request_id);
		return 0;
	} else if(_type_hash == vnx::Hash64(0x70967f585e137c7dull)) {
		auto _args = std::dynamic_pointer_cast<const ::vnx::search::SearchEngine_reverse_domain_lookup>(_method);
		if(!_args) {
			throw std::logic_error("vnx_call_switch(): !_args");
		}
		reverse_domain_lookup_async(_args->url_key, _request_id);
		return 0;
	} else if(_type_hash == vnx::Hash64(0x7124d7bfda1b31f2ull)) {
		auto _args = std::dynamic_pointer_cast<const ::vnx::search::SearchEngine_suggest_words>(_method);
		if(!_args) {
			throw std::logic_error("vnx_call_switch(): !_args");
		}
		auto _return_value = ::vnx::search::SearchEngine_suggest_words_return::create();
		_return_value->_ret_0 = suggest_words(_args->prefix, _args->limit);
		return _return_value;
	} else if(_type_hash == vnx::Hash64(0x1abdd70e75f411b3ull)) {
		auto _args = std::dynamic_pointer_cast<const ::vnx::search::SearchEngine_suggest_domains>(_method);
		if(!_args) {
			throw std::logic_error("vnx_call_switch(): !_args");
		}
		auto _return_value = ::vnx::search::SearchEngine_suggest_domains_return::create();
		_return_value->_ret_0 = suggest_domains(_args->prefix, _args->limit);
		return _return_value;
	}
	auto _ex = vnx::NoSuchMethod::create();
	_ex->dst_mac = vnx_request ? vnx_request->dst_mac : 0;
	_ex->method = _method->get_type_name();
	return _ex;
}

void SearchEngineBase::query_async_return(const vnx::request_id_t& _request_id, const std::shared_ptr<const ::vnx::search::SearchResult>& _ret_0) const {
	auto _return_value = ::vnx::search::SearchEngine_query_return::create();
	_return_value->_ret_0 = _ret_0;
	vnx_async_return(_request_id, _return_value);
}

void SearchEngineBase::get_page_info_async_return(const vnx::request_id_t& _request_id, const ::vnx::Object& _ret_0) const {
	auto _return_value = ::vnx::search::SearchEngine_get_page_info_return::create();
	_return_value->_ret_0 = _ret_0;
	vnx_async_return(_request_id, _return_value);
}

void SearchEngineBase::reverse_lookup_async_return(const vnx::request_id_t& _request_id, const std::vector<std::string>& _ret_0) const {
	auto _return_value = ::vnx::search::SearchEngine_reverse_lookup_return::create();
	_return_value->_ret_0 = _ret_0;
	vnx_async_return(_request_id, _return_value);
}

void SearchEngineBase::reverse_domain_lookup_async_return(const vnx::request_id_t& _request_id, const std::vector<std::pair<std::string, uint32_t>>& _ret_0) const {
	auto _return_value = ::vnx::search::SearchEngine_reverse_domain_lookup_return::create();
	_return_value->_ret_0 = _ret_0;
	vnx_async_return(_request_id, _return_value);
}


} // namespace vnx
} // namespace search


namespace vnx {

void read(TypeInput& in, ::vnx::search::SearchEngineBase& value, const TypeCode* type_code, const uint16_t* code) {
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
				vnx::read_value(_buf + _field->offset, value.max_word_cache, _field->code.data());
			}
		}
		{
			const vnx::TypeField* const _field = type_code->field_map[7];
			if(_field) {
				vnx::read_value(_buf + _field->offset, value.max_num_pending, _field->code.data());
			}
		}
		{
			const vnx::TypeField* const _field = type_code->field_map[8];
			if(_field) {
				vnx::read_value(_buf + _field->offset, value.num_query_threads, _field->code.data());
			}
		}
		{
			const vnx::TypeField* const _field = type_code->field_map[9];
			if(_field) {
				vnx::read_value(_buf + _field->offset, value.num_update_threads, _field->code.data());
			}
		}
		{
			const vnx::TypeField* const _field = type_code->field_map[10];
			if(_field) {
				vnx::read_value(_buf + _field->offset, value.link_commit_interval, _field->code.data());
			}
		}
		{
			const vnx::TypeField* const _field = type_code->field_map[11];
			if(_field) {
				vnx::read_value(_buf + _field->offset, value.word_commit_interval, _field->code.data());
			}
		}
		{
			const vnx::TypeField* const _field = type_code->field_map[12];
			if(_field) {
				vnx::read_value(_buf + _field->offset, value.lock_timeout, _field->code.data());
			}
		}
		{
			const vnx::TypeField* const _field = type_code->field_map[13];
			if(_field) {
				vnx::read_value(_buf + _field->offset, value.stats_interval_ms, _field->code.data());
			}
		}
		{
			const vnx::TypeField* const _field = type_code->field_map[14];
			if(_field) {
				vnx::read_value(_buf + _field->offset, value.word_cutoff, _field->code.data());
			}
		}
		{
			const vnx::TypeField* const _field = type_code->field_map[15];
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
			case 16: vnx::read(in, value.protocols, type_code, _field->code.data()); break;
			default: vnx::skip(in, type_code, _field->code.data());
		}
	}
}

void write(TypeOutput& out, const ::vnx::search::SearchEngineBase& value, const TypeCode* type_code, const uint16_t* code) {
	if(!type_code || (code && code[0] == CODE_ANY)) {
		type_code = vnx::search::vnx_native_type_code_SearchEngineBase;
		out.write_type_code(type_code);
		vnx::write_class_header<::vnx::search::SearchEngineBase>(out);
	}
	if(code && code[0] == CODE_STRUCT) {
		type_code = type_code->depends[code[1]];
	}
	char* const _buf = out.write(44);
	vnx::write_value(_buf + 0, value.max_query_pages);
	vnx::write_value(_buf + 4, value.max_word_cache);
	vnx::write_value(_buf + 8, value.max_num_pending);
	vnx::write_value(_buf + 12, value.num_query_threads);
	vnx::write_value(_buf + 16, value.num_update_threads);
	vnx::write_value(_buf + 20, value.link_commit_interval);
	vnx::write_value(_buf + 24, value.word_commit_interval);
	vnx::write_value(_buf + 28, value.lock_timeout);
	vnx::write_value(_buf + 32, value.stats_interval_ms);
	vnx::write_value(_buf + 36, value.word_cutoff);
	vnx::write_value(_buf + 40, value.word_power);
	vnx::write(out, value.input_url_index, type_code, type_code->fields[0].code.data());
	vnx::write(out, value.input_page_index, type_code, type_code->fields[1].code.data());
	vnx::write(out, value.url_index_server, type_code, type_code->fields[2].code.data());
	vnx::write(out, value.page_index_server, type_code, type_code->fields[3].code.data());
	vnx::write(out, value.page_content_server, type_code, type_code->fields[4].code.data());
	vnx::write(out, value.protocols, type_code, type_code->fields[16].code.data());
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
