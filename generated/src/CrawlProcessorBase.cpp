
// AUTO GENERATED by vnxcppcodegen

#include <vnx/vnx.h>
#include <vnx/search/package.hxx>
#include <vnx/search/CrawlProcessorBase.hxx>
#include <vnx/NoSuchMethod.hxx>
#include <vnx/Module.h>
#include <vnx/Object.hpp>
#include <vnx/TopicPtr.hpp>
#include <vnx/keyvalue/KeyValuePair.hxx>
#include <vnx/search/CrawlProcessor__page_process_callback.hxx>
#include <vnx/search/CrawlProcessor__page_process_callback_return.hxx>
#include <vnx/search/CrawlProcessor_get_stats.hxx>
#include <vnx/search/CrawlProcessor_get_stats_return.hxx>
#include <vnx/search/PageIndex.hxx>
#include <vnx/search/TextResponse.hxx>



namespace vnx {
namespace search {


const vnx::Hash64 CrawlProcessorBase::VNX_TYPE_HASH(0x508848d1f9d97d9full);
const vnx::Hash64 CrawlProcessorBase::VNX_CODE_HASH(0x9363606ffbee9f19ull);

CrawlProcessorBase::CrawlProcessorBase(const std::string& _vnx_name)
	:	Module::Module(_vnx_name)
{
	vnx::read_config(vnx_name + ".check_interval_ms", check_interval_ms);
	vnx::read_config(vnx_name + ".crawl_frontend_server", crawl_frontend_server);
	vnx::read_config(vnx_name + ".do_reprocess", do_reprocess);
	vnx::read_config(vnx_name + ".domain_blacklist", domain_blacklist);
	vnx::read_config(vnx_name + ".error_reload_interval", error_reload_interval);
	vnx::read_config(vnx_name + ".index_version", index_version);
	vnx::read_config(vnx_name + ".input_url_index", input_url_index);
	vnx::read_config(vnx_name + ".jump_cost", jump_cost);
	vnx::read_config(vnx_name + ".max_depth", max_depth);
	vnx::read_config(vnx_name + ".max_num_pending", max_num_pending);
	vnx::read_config(vnx_name + ".max_per_minute", max_per_minute);
	vnx::read_config(vnx_name + ".max_queue_size", max_queue_size);
	vnx::read_config(vnx_name + ".max_url_length", max_url_length);
	vnx::read_config(vnx_name + ".max_word_length", max_word_length);
	vnx::read_config(vnx_name + ".num_worker_threads", num_worker_threads);
	vnx::read_config(vnx_name + ".output_crawl_stats", output_crawl_stats);
	vnx::read_config(vnx_name + ".page_content_server", page_content_server);
	vnx::read_config(vnx_name + ".page_index_server", page_index_server);
	vnx::read_config(vnx_name + ".path_blacklist", path_blacklist);
	vnx::read_config(vnx_name + ".protocols", protocols);
	vnx::read_config(vnx_name + ".regex_blacklist", regex_blacklist);
	vnx::read_config(vnx_name + ".reload_interval", reload_interval);
	vnx::read_config(vnx_name + ".reload_power", reload_power);
	vnx::read_config(vnx_name + ".robots_txt_timeout", robots_txt_timeout);
	vnx::read_config(vnx_name + ".root_urls", root_urls);
	vnx::read_config(vnx_name + ".sync_interval", sync_interval);
	vnx::read_config(vnx_name + ".update_interval_ms", update_interval_ms);
	vnx::read_config(vnx_name + ".url_index_server", url_index_server);
	vnx::read_config(vnx_name + ".user_agent", user_agent);
}

vnx::Hash64 CrawlProcessorBase::get_type_hash() const {
	return VNX_TYPE_HASH;
}

const char* CrawlProcessorBase::get_type_name() const {
	return "vnx.search.CrawlProcessor";
}
const vnx::TypeCode* CrawlProcessorBase::get_type_code() const {
	return vnx::search::vnx_native_type_code_CrawlProcessorBase;
}

void CrawlProcessorBase::accept(vnx::Visitor& _visitor) const {
	const vnx::TypeCode* _type_code = vnx::search::vnx_native_type_code_CrawlProcessorBase;
	_visitor.type_begin(*_type_code);
	_visitor.type_field(_type_code->fields[0], 0); vnx::accept(_visitor, input_url_index);
	_visitor.type_field(_type_code->fields[1], 1); vnx::accept(_visitor, output_crawl_stats);
	_visitor.type_field(_type_code->fields[2], 2); vnx::accept(_visitor, url_index_server);
	_visitor.type_field(_type_code->fields[3], 3); vnx::accept(_visitor, page_index_server);
	_visitor.type_field(_type_code->fields[4], 4); vnx::accept(_visitor, page_content_server);
	_visitor.type_field(_type_code->fields[5], 5); vnx::accept(_visitor, crawl_frontend_server);
	_visitor.type_field(_type_code->fields[6], 6); vnx::accept(_visitor, jump_cost);
	_visitor.type_field(_type_code->fields[7], 7); vnx::accept(_visitor, max_depth);
	_visitor.type_field(_type_code->fields[8], 8); vnx::accept(_visitor, reload_interval);
	_visitor.type_field(_type_code->fields[9], 9); vnx::accept(_visitor, error_reload_interval);
	_visitor.type_field(_type_code->fields[10], 10); vnx::accept(_visitor, sync_interval);
	_visitor.type_field(_type_code->fields[11], 11); vnx::accept(_visitor, max_per_minute);
	_visitor.type_field(_type_code->fields[12], 12); vnx::accept(_visitor, max_num_pending);
	_visitor.type_field(_type_code->fields[13], 13); vnx::accept(_visitor, max_queue_size);
	_visitor.type_field(_type_code->fields[14], 14); vnx::accept(_visitor, max_url_length);
	_visitor.type_field(_type_code->fields[15], 15); vnx::accept(_visitor, max_word_length);
	_visitor.type_field(_type_code->fields[16], 16); vnx::accept(_visitor, num_worker_threads);
	_visitor.type_field(_type_code->fields[17], 17); vnx::accept(_visitor, check_interval_ms);
	_visitor.type_field(_type_code->fields[18], 18); vnx::accept(_visitor, update_interval_ms);
	_visitor.type_field(_type_code->fields[19], 19); vnx::accept(_visitor, robots_txt_timeout);
	_visitor.type_field(_type_code->fields[20], 20); vnx::accept(_visitor, reload_power);
	_visitor.type_field(_type_code->fields[21], 21); vnx::accept(_visitor, index_version);
	_visitor.type_field(_type_code->fields[22], 22); vnx::accept(_visitor, do_reprocess);
	_visitor.type_field(_type_code->fields[23], 23); vnx::accept(_visitor, user_agent);
	_visitor.type_field(_type_code->fields[24], 24); vnx::accept(_visitor, protocols);
	_visitor.type_field(_type_code->fields[25], 25); vnx::accept(_visitor, root_urls);
	_visitor.type_field(_type_code->fields[26], 26); vnx::accept(_visitor, domain_blacklist);
	_visitor.type_field(_type_code->fields[27], 27); vnx::accept(_visitor, path_blacklist);
	_visitor.type_field(_type_code->fields[28], 28); vnx::accept(_visitor, regex_blacklist);
	_visitor.type_end(*_type_code);
}

void CrawlProcessorBase::write(std::ostream& _out) const {
	_out << "{\"__type\": \"vnx.search.CrawlProcessor\"";
	_out << ", \"input_url_index\": "; vnx::write(_out, input_url_index);
	_out << ", \"output_crawl_stats\": "; vnx::write(_out, output_crawl_stats);
	_out << ", \"url_index_server\": "; vnx::write(_out, url_index_server);
	_out << ", \"page_index_server\": "; vnx::write(_out, page_index_server);
	_out << ", \"page_content_server\": "; vnx::write(_out, page_content_server);
	_out << ", \"crawl_frontend_server\": "; vnx::write(_out, crawl_frontend_server);
	_out << ", \"jump_cost\": "; vnx::write(_out, jump_cost);
	_out << ", \"max_depth\": "; vnx::write(_out, max_depth);
	_out << ", \"reload_interval\": "; vnx::write(_out, reload_interval);
	_out << ", \"error_reload_interval\": "; vnx::write(_out, error_reload_interval);
	_out << ", \"sync_interval\": "; vnx::write(_out, sync_interval);
	_out << ", \"max_per_minute\": "; vnx::write(_out, max_per_minute);
	_out << ", \"max_num_pending\": "; vnx::write(_out, max_num_pending);
	_out << ", \"max_queue_size\": "; vnx::write(_out, max_queue_size);
	_out << ", \"max_url_length\": "; vnx::write(_out, max_url_length);
	_out << ", \"max_word_length\": "; vnx::write(_out, max_word_length);
	_out << ", \"num_worker_threads\": "; vnx::write(_out, num_worker_threads);
	_out << ", \"check_interval_ms\": "; vnx::write(_out, check_interval_ms);
	_out << ", \"update_interval_ms\": "; vnx::write(_out, update_interval_ms);
	_out << ", \"robots_txt_timeout\": "; vnx::write(_out, robots_txt_timeout);
	_out << ", \"reload_power\": "; vnx::write(_out, reload_power);
	_out << ", \"index_version\": "; vnx::write(_out, index_version);
	_out << ", \"do_reprocess\": "; vnx::write(_out, do_reprocess);
	_out << ", \"user_agent\": "; vnx::write(_out, user_agent);
	_out << ", \"protocols\": "; vnx::write(_out, protocols);
	_out << ", \"root_urls\": "; vnx::write(_out, root_urls);
	_out << ", \"domain_blacklist\": "; vnx::write(_out, domain_blacklist);
	_out << ", \"path_blacklist\": "; vnx::write(_out, path_blacklist);
	_out << ", \"regex_blacklist\": "; vnx::write(_out, regex_blacklist);
	_out << "}";
}

void CrawlProcessorBase::read(std::istream& _in) {
	std::map<std::string, std::string> _object;
	vnx::read_object(_in, _object);
	for(const auto& _entry : _object) {
		if(_entry.first == "check_interval_ms") {
			vnx::from_string(_entry.second, check_interval_ms);
		} else if(_entry.first == "crawl_frontend_server") {
			vnx::from_string(_entry.second, crawl_frontend_server);
		} else if(_entry.first == "do_reprocess") {
			vnx::from_string(_entry.second, do_reprocess);
		} else if(_entry.first == "domain_blacklist") {
			vnx::from_string(_entry.second, domain_blacklist);
		} else if(_entry.first == "error_reload_interval") {
			vnx::from_string(_entry.second, error_reload_interval);
		} else if(_entry.first == "index_version") {
			vnx::from_string(_entry.second, index_version);
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
		} else if(_entry.first == "max_queue_size") {
			vnx::from_string(_entry.second, max_queue_size);
		} else if(_entry.first == "max_url_length") {
			vnx::from_string(_entry.second, max_url_length);
		} else if(_entry.first == "max_word_length") {
			vnx::from_string(_entry.second, max_word_length);
		} else if(_entry.first == "num_worker_threads") {
			vnx::from_string(_entry.second, num_worker_threads);
		} else if(_entry.first == "output_crawl_stats") {
			vnx::from_string(_entry.second, output_crawl_stats);
		} else if(_entry.first == "page_content_server") {
			vnx::from_string(_entry.second, page_content_server);
		} else if(_entry.first == "page_index_server") {
			vnx::from_string(_entry.second, page_index_server);
		} else if(_entry.first == "path_blacklist") {
			vnx::from_string(_entry.second, path_blacklist);
		} else if(_entry.first == "protocols") {
			vnx::from_string(_entry.second, protocols);
		} else if(_entry.first == "regex_blacklist") {
			vnx::from_string(_entry.second, regex_blacklist);
		} else if(_entry.first == "reload_interval") {
			vnx::from_string(_entry.second, reload_interval);
		} else if(_entry.first == "reload_power") {
			vnx::from_string(_entry.second, reload_power);
		} else if(_entry.first == "robots_txt_timeout") {
			vnx::from_string(_entry.second, robots_txt_timeout);
		} else if(_entry.first == "root_urls") {
			vnx::from_string(_entry.second, root_urls);
		} else if(_entry.first == "sync_interval") {
			vnx::from_string(_entry.second, sync_interval);
		} else if(_entry.first == "update_interval_ms") {
			vnx::from_string(_entry.second, update_interval_ms);
		} else if(_entry.first == "url_index_server") {
			vnx::from_string(_entry.second, url_index_server);
		} else if(_entry.first == "user_agent") {
			vnx::from_string(_entry.second, user_agent);
		}
	}
}

vnx::Object CrawlProcessorBase::to_object() const {
	vnx::Object _object;
	_object["__type"] = "vnx.search.CrawlProcessor";
	_object["input_url_index"] = input_url_index;
	_object["output_crawl_stats"] = output_crawl_stats;
	_object["url_index_server"] = url_index_server;
	_object["page_index_server"] = page_index_server;
	_object["page_content_server"] = page_content_server;
	_object["crawl_frontend_server"] = crawl_frontend_server;
	_object["jump_cost"] = jump_cost;
	_object["max_depth"] = max_depth;
	_object["reload_interval"] = reload_interval;
	_object["error_reload_interval"] = error_reload_interval;
	_object["sync_interval"] = sync_interval;
	_object["max_per_minute"] = max_per_minute;
	_object["max_num_pending"] = max_num_pending;
	_object["max_queue_size"] = max_queue_size;
	_object["max_url_length"] = max_url_length;
	_object["max_word_length"] = max_word_length;
	_object["num_worker_threads"] = num_worker_threads;
	_object["check_interval_ms"] = check_interval_ms;
	_object["update_interval_ms"] = update_interval_ms;
	_object["robots_txt_timeout"] = robots_txt_timeout;
	_object["reload_power"] = reload_power;
	_object["index_version"] = index_version;
	_object["do_reprocess"] = do_reprocess;
	_object["user_agent"] = user_agent;
	_object["protocols"] = protocols;
	_object["root_urls"] = root_urls;
	_object["domain_blacklist"] = domain_blacklist;
	_object["path_blacklist"] = path_blacklist;
	_object["regex_blacklist"] = regex_blacklist;
	return _object;
}

void CrawlProcessorBase::from_object(const vnx::Object& _object) {
	for(const auto& _entry : _object.field) {
		if(_entry.first == "check_interval_ms") {
			_entry.second.to(check_interval_ms);
		} else if(_entry.first == "crawl_frontend_server") {
			_entry.second.to(crawl_frontend_server);
		} else if(_entry.first == "do_reprocess") {
			_entry.second.to(do_reprocess);
		} else if(_entry.first == "domain_blacklist") {
			_entry.second.to(domain_blacklist);
		} else if(_entry.first == "error_reload_interval") {
			_entry.second.to(error_reload_interval);
		} else if(_entry.first == "index_version") {
			_entry.second.to(index_version);
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
		} else if(_entry.first == "max_queue_size") {
			_entry.second.to(max_queue_size);
		} else if(_entry.first == "max_url_length") {
			_entry.second.to(max_url_length);
		} else if(_entry.first == "max_word_length") {
			_entry.second.to(max_word_length);
		} else if(_entry.first == "num_worker_threads") {
			_entry.second.to(num_worker_threads);
		} else if(_entry.first == "output_crawl_stats") {
			_entry.second.to(output_crawl_stats);
		} else if(_entry.first == "page_content_server") {
			_entry.second.to(page_content_server);
		} else if(_entry.first == "page_index_server") {
			_entry.second.to(page_index_server);
		} else if(_entry.first == "path_blacklist") {
			_entry.second.to(path_blacklist);
		} else if(_entry.first == "protocols") {
			_entry.second.to(protocols);
		} else if(_entry.first == "regex_blacklist") {
			_entry.second.to(regex_blacklist);
		} else if(_entry.first == "reload_interval") {
			_entry.second.to(reload_interval);
		} else if(_entry.first == "reload_power") {
			_entry.second.to(reload_power);
		} else if(_entry.first == "robots_txt_timeout") {
			_entry.second.to(robots_txt_timeout);
		} else if(_entry.first == "root_urls") {
			_entry.second.to(root_urls);
		} else if(_entry.first == "sync_interval") {
			_entry.second.to(sync_interval);
		} else if(_entry.first == "update_interval_ms") {
			_entry.second.to(update_interval_ms);
		} else if(_entry.first == "url_index_server") {
			_entry.second.to(url_index_server);
		} else if(_entry.first == "user_agent") {
			_entry.second.to(user_agent);
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
	const vnx::TypeCode* type_code = vnx::get_type_code(VNX_TYPE_HASH);
	if(!type_code) {
		type_code = vnx::register_type_code(static_create_type_code());
	}
	return type_code;
}

std::shared_ptr<vnx::TypeCode> CrawlProcessorBase::static_create_type_code() {
	std::shared_ptr<vnx::TypeCode> type_code = std::make_shared<vnx::TypeCode>();
	type_code->name = "vnx.search.CrawlProcessor";
	type_code->type_hash = vnx::Hash64(0x508848d1f9d97d9full);
	type_code->code_hash = vnx::Hash64(0x9363606ffbee9f19ull);
	type_code->is_native = true;
	type_code->methods.resize(2);
	type_code->methods[0] = ::vnx::search::CrawlProcessor__page_process_callback::static_get_type_code();
	type_code->methods[1] = ::vnx::search::CrawlProcessor_get_stats::static_get_type_code();
	type_code->fields.resize(29);
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
		field.name = "output_crawl_stats";
		field.value = vnx::to_string("backend.crawl_stats");
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
		field.is_extended = true;
		field.name = "crawl_frontend_server";
		field.value = vnx::to_string("CrawlFrontend");
		field.code = {12, 5};
	}
	{
		vnx::TypeField& field = type_code->fields[6];
		field.name = "jump_cost";
		field.value = vnx::to_string(3);
		field.code = {7};
	}
	{
		vnx::TypeField& field = type_code->fields[7];
		field.name = "max_depth";
		field.value = vnx::to_string(5);
		field.code = {7};
	}
	{
		vnx::TypeField& field = type_code->fields[8];
		field.name = "reload_interval";
		field.value = vnx::to_string(10000);
		field.code = {7};
	}
	{
		vnx::TypeField& field = type_code->fields[9];
		field.name = "error_reload_interval";
		field.value = vnx::to_string(100000);
		field.code = {7};
	}
	{
		vnx::TypeField& field = type_code->fields[10];
		field.name = "sync_interval";
		field.value = vnx::to_string(10000);
		field.code = {7};
	}
	{
		vnx::TypeField& field = type_code->fields[11];
		field.name = "max_per_minute";
		field.value = vnx::to_string(12);
		field.code = {7};
	}
	{
		vnx::TypeField& field = type_code->fields[12];
		field.name = "max_num_pending";
		field.value = vnx::to_string(100);
		field.code = {7};
	}
	{
		vnx::TypeField& field = type_code->fields[13];
		field.name = "max_queue_size";
		field.value = vnx::to_string(1000);
		field.code = {7};
	}
	{
		vnx::TypeField& field = type_code->fields[14];
		field.name = "max_url_length";
		field.value = vnx::to_string(256);
		field.code = {7};
	}
	{
		vnx::TypeField& field = type_code->fields[15];
		field.name = "max_word_length";
		field.value = vnx::to_string(64);
		field.code = {7};
	}
	{
		vnx::TypeField& field = type_code->fields[16];
		field.name = "num_worker_threads";
		field.value = vnx::to_string(4);
		field.code = {7};
	}
	{
		vnx::TypeField& field = type_code->fields[17];
		field.name = "check_interval_ms";
		field.value = vnx::to_string(500);
		field.code = {7};
	}
	{
		vnx::TypeField& field = type_code->fields[18];
		field.name = "update_interval_ms";
		field.value = vnx::to_string(5000);
		field.code = {7};
	}
	{
		vnx::TypeField& field = type_code->fields[19];
		field.name = "robots_txt_timeout";
		field.value = vnx::to_string(1000);
		field.code = {7};
	}
	{
		vnx::TypeField& field = type_code->fields[20];
		field.name = "reload_power";
		field.value = vnx::to_string(4);
		field.code = {9};
	}
	{
		vnx::TypeField& field = type_code->fields[21];
		field.name = "index_version";
		field.value = vnx::to_string(0);
		field.code = {3};
	}
	{
		vnx::TypeField& field = type_code->fields[22];
		field.name = "do_reprocess";
		field.value = vnx::to_string(false);
		field.code = {1};
	}
	{
		vnx::TypeField& field = type_code->fields[23];
		field.is_extended = true;
		field.name = "user_agent";
		field.value = vnx::to_string("Googlebot");
		field.code = {12, 5};
	}
	{
		vnx::TypeField& field = type_code->fields[24];
		field.is_extended = true;
		field.name = "protocols";
		field.code = {12, 12, 5};
	}
	{
		vnx::TypeField& field = type_code->fields[25];
		field.is_extended = true;
		field.name = "root_urls";
		field.code = {12, 12, 5};
	}
	{
		vnx::TypeField& field = type_code->fields[26];
		field.is_extended = true;
		field.name = "domain_blacklist";
		field.code = {12, 12, 5};
	}
	{
		vnx::TypeField& field = type_code->fields[27];
		field.is_extended = true;
		field.name = "path_blacklist";
		field.code = {12, 12, 5};
	}
	{
		vnx::TypeField& field = type_code->fields[28];
		field.is_extended = true;
		field.name = "regex_blacklist";
		field.code = {12, 12, 5};
	}
	type_code->build();
	return type_code;
}

void CrawlProcessorBase::vnx_handle_switch(std::shared_ptr<const vnx::Sample> _sample) {
	const auto _type_hash = _sample->value->get_type_hash();
	if(_type_hash == 0xf87436237449d8afull) {
		auto _value = std::dynamic_pointer_cast<const ::vnx::keyvalue::KeyValuePair>(_sample->value);
		if(_value) {
			handle(_value, _sample);
		}
	} else if(_type_hash == 0x7cee1cd5b88ec569ull) {
		auto _value = std::dynamic_pointer_cast<const ::vnx::search::TextResponse>(_sample->value);
		if(_value) {
			handle(_value, _sample);
		}
	}
}

std::shared_ptr<vnx::Value> CrawlProcessorBase::vnx_call_switch(std::shared_ptr<const vnx::Value> _value, const vnx::request_id_t& _request_id) {
	const auto _type_hash = _value->get_type_hash();
	if(_type_hash == vnx::Hash64(0x7756d364f264da2bull)) {
		auto _args = std::dynamic_pointer_cast<const ::vnx::search::CrawlProcessor__page_process_callback>(_value);
		if(!_args) {
			throw std::logic_error("vnx_call_switch(): !_args");
		}
		auto _return_value = ::vnx::search::CrawlProcessor__page_process_callback_return::create();
		_page_process_callback(_args->url_key, _args->index, _args->is_reprocess);
		return _return_value;
	} else if(_type_hash == vnx::Hash64(0x7b8fefbbb0397a0dull)) {
		auto _args = std::dynamic_pointer_cast<const ::vnx::search::CrawlProcessor_get_stats>(_value);
		if(!_args) {
			throw std::logic_error("vnx_call_switch(): !_args");
		}
		auto _return_value = ::vnx::search::CrawlProcessor_get_stats_return::create();
		_return_value->_ret_0 = get_stats(_args->limit);
		return _return_value;
	}
	auto _ex = vnx::NoSuchMethod::create();
	_ex->dst_mac = vnx_request ? vnx_request->dst_mac : 0;
	_ex->method = _value->get_type_name();
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
			const vnx::TypeField* const _field = type_code->field_map[6];
			if(_field) {
				vnx::read_value(_buf + _field->offset, value.jump_cost, _field->code.data());
			}
		}
		{
			const vnx::TypeField* const _field = type_code->field_map[7];
			if(_field) {
				vnx::read_value(_buf + _field->offset, value.max_depth, _field->code.data());
			}
		}
		{
			const vnx::TypeField* const _field = type_code->field_map[8];
			if(_field) {
				vnx::read_value(_buf + _field->offset, value.reload_interval, _field->code.data());
			}
		}
		{
			const vnx::TypeField* const _field = type_code->field_map[9];
			if(_field) {
				vnx::read_value(_buf + _field->offset, value.error_reload_interval, _field->code.data());
			}
		}
		{
			const vnx::TypeField* const _field = type_code->field_map[10];
			if(_field) {
				vnx::read_value(_buf + _field->offset, value.sync_interval, _field->code.data());
			}
		}
		{
			const vnx::TypeField* const _field = type_code->field_map[11];
			if(_field) {
				vnx::read_value(_buf + _field->offset, value.max_per_minute, _field->code.data());
			}
		}
		{
			const vnx::TypeField* const _field = type_code->field_map[12];
			if(_field) {
				vnx::read_value(_buf + _field->offset, value.max_num_pending, _field->code.data());
			}
		}
		{
			const vnx::TypeField* const _field = type_code->field_map[13];
			if(_field) {
				vnx::read_value(_buf + _field->offset, value.max_queue_size, _field->code.data());
			}
		}
		{
			const vnx::TypeField* const _field = type_code->field_map[14];
			if(_field) {
				vnx::read_value(_buf + _field->offset, value.max_url_length, _field->code.data());
			}
		}
		{
			const vnx::TypeField* const _field = type_code->field_map[15];
			if(_field) {
				vnx::read_value(_buf + _field->offset, value.max_word_length, _field->code.data());
			}
		}
		{
			const vnx::TypeField* const _field = type_code->field_map[16];
			if(_field) {
				vnx::read_value(_buf + _field->offset, value.num_worker_threads, _field->code.data());
			}
		}
		{
			const vnx::TypeField* const _field = type_code->field_map[17];
			if(_field) {
				vnx::read_value(_buf + _field->offset, value.check_interval_ms, _field->code.data());
			}
		}
		{
			const vnx::TypeField* const _field = type_code->field_map[18];
			if(_field) {
				vnx::read_value(_buf + _field->offset, value.update_interval_ms, _field->code.data());
			}
		}
		{
			const vnx::TypeField* const _field = type_code->field_map[19];
			if(_field) {
				vnx::read_value(_buf + _field->offset, value.robots_txt_timeout, _field->code.data());
			}
		}
		{
			const vnx::TypeField* const _field = type_code->field_map[20];
			if(_field) {
				vnx::read_value(_buf + _field->offset, value.reload_power, _field->code.data());
			}
		}
		{
			const vnx::TypeField* const _field = type_code->field_map[21];
			if(_field) {
				vnx::read_value(_buf + _field->offset, value.index_version, _field->code.data());
			}
		}
		{
			const vnx::TypeField* const _field = type_code->field_map[22];
			if(_field) {
				vnx::read_value(_buf + _field->offset, value.do_reprocess, _field->code.data());
			}
		}
	}
	for(const vnx::TypeField* _field : type_code->ext_fields) {
		switch(_field->native_index) {
			case 0: vnx::read(in, value.input_url_index, type_code, _field->code.data()); break;
			case 1: vnx::read(in, value.output_crawl_stats, type_code, _field->code.data()); break;
			case 2: vnx::read(in, value.url_index_server, type_code, _field->code.data()); break;
			case 3: vnx::read(in, value.page_index_server, type_code, _field->code.data()); break;
			case 4: vnx::read(in, value.page_content_server, type_code, _field->code.data()); break;
			case 5: vnx::read(in, value.crawl_frontend_server, type_code, _field->code.data()); break;
			case 23: vnx::read(in, value.user_agent, type_code, _field->code.data()); break;
			case 24: vnx::read(in, value.protocols, type_code, _field->code.data()); break;
			case 25: vnx::read(in, value.root_urls, type_code, _field->code.data()); break;
			case 26: vnx::read(in, value.domain_blacklist, type_code, _field->code.data()); break;
			case 27: vnx::read(in, value.path_blacklist, type_code, _field->code.data()); break;
			case 28: vnx::read(in, value.regex_blacklist, type_code, _field->code.data()); break;
			default: vnx::skip(in, type_code, _field->code.data());
		}
	}
}

void write(TypeOutput& out, const ::vnx::search::CrawlProcessorBase& value, const TypeCode* type_code, const uint16_t* code) {
	if(!type_code || (code && code[0] == CODE_ANY)) {
		type_code = vnx::search::vnx_native_type_code_CrawlProcessorBase;
		out.write_type_code(type_code);
		vnx::write_class_header<::vnx::search::CrawlProcessorBase>(out);
	}
	if(code && code[0] == CODE_STRUCT) {
		type_code = type_code->depends[code[1]];
	}
	char* const _buf = out.write(65);
	vnx::write_value(_buf + 0, value.jump_cost);
	vnx::write_value(_buf + 4, value.max_depth);
	vnx::write_value(_buf + 8, value.reload_interval);
	vnx::write_value(_buf + 12, value.error_reload_interval);
	vnx::write_value(_buf + 16, value.sync_interval);
	vnx::write_value(_buf + 20, value.max_per_minute);
	vnx::write_value(_buf + 24, value.max_num_pending);
	vnx::write_value(_buf + 28, value.max_queue_size);
	vnx::write_value(_buf + 32, value.max_url_length);
	vnx::write_value(_buf + 36, value.max_word_length);
	vnx::write_value(_buf + 40, value.num_worker_threads);
	vnx::write_value(_buf + 44, value.check_interval_ms);
	vnx::write_value(_buf + 48, value.update_interval_ms);
	vnx::write_value(_buf + 52, value.robots_txt_timeout);
	vnx::write_value(_buf + 56, value.reload_power);
	vnx::write_value(_buf + 60, value.index_version);
	vnx::write_value(_buf + 64, value.do_reprocess);
	vnx::write(out, value.input_url_index, type_code, type_code->fields[0].code.data());
	vnx::write(out, value.output_crawl_stats, type_code, type_code->fields[1].code.data());
	vnx::write(out, value.url_index_server, type_code, type_code->fields[2].code.data());
	vnx::write(out, value.page_index_server, type_code, type_code->fields[3].code.data());
	vnx::write(out, value.page_content_server, type_code, type_code->fields[4].code.data());
	vnx::write(out, value.crawl_frontend_server, type_code, type_code->fields[5].code.data());
	vnx::write(out, value.user_agent, type_code, type_code->fields[23].code.data());
	vnx::write(out, value.protocols, type_code, type_code->fields[24].code.data());
	vnx::write(out, value.root_urls, type_code, type_code->fields[25].code.data());
	vnx::write(out, value.domain_blacklist, type_code, type_code->fields[26].code.data());
	vnx::write(out, value.path_blacklist, type_code, type_code->fields[27].code.data());
	vnx::write(out, value.regex_blacklist, type_code, type_code->fields[28].code.data());
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
