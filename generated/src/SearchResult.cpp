
// AUTO GENERATED by vnxcppcodegen

#include <vnx/search/package.hxx>
#include <vnx/search/SearchResult.hxx>
#include <vnx/Value.h>
#include <vnx/search/query_options_t.hxx>
#include <vnx/search/result_item_t.hxx>

#include <vnx/vnx.h>


namespace vnx {
namespace search {


const vnx::Hash64 SearchResult::VNX_TYPE_HASH(0x659ce58d97581ddull);
const vnx::Hash64 SearchResult::VNX_CODE_HASH(0xa4249ffea269ad0cull);

vnx::Hash64 SearchResult::get_type_hash() const {
	return VNX_TYPE_HASH;
}

std::string SearchResult::get_type_name() const {
	return "vnx.search.SearchResult";
}

const vnx::TypeCode* SearchResult::get_type_code() const {
	return vnx::search::vnx_native_type_code_SearchResult;
}

std::shared_ptr<SearchResult> SearchResult::create() {
	return std::make_shared<SearchResult>();
}

std::shared_ptr<vnx::Value> SearchResult::clone() const {
	return std::make_shared<SearchResult>(*this);
}

void SearchResult::read(vnx::TypeInput& _in, const vnx::TypeCode* _type_code, const uint16_t* _code) {
	vnx::read(_in, *this, _type_code, _code);
}

void SearchResult::write(vnx::TypeOutput& _out, const vnx::TypeCode* _type_code, const uint16_t* _code) const {
	vnx::write(_out, *this, _type_code, _code);
}

void SearchResult::accept(vnx::Visitor& _visitor) const {
	const vnx::TypeCode* _type_code = vnx::search::vnx_native_type_code_SearchResult;
	_visitor.type_begin(*_type_code);
	_visitor.type_field(_type_code->fields[0], 0); vnx::accept(_visitor, is_fail);
	_visitor.type_field(_type_code->fields[1], 1); vnx::accept(_visitor, has_more);
	_visitor.type_field(_type_code->fields[2], 2); vnx::accept(_visitor, num_results_total);
	_visitor.type_field(_type_code->fields[3], 3); vnx::accept(_visitor, load_time_us);
	_visitor.type_field(_type_code->fields[4], 4); vnx::accept(_visitor, compute_time_us);
	_visitor.type_field(_type_code->fields[5], 5); vnx::accept(_visitor, error_msg);
	_visitor.type_field(_type_code->fields[6], 6); vnx::accept(_visitor, options);
	_visitor.type_field(_type_code->fields[7], 7); vnx::accept(_visitor, words);
	_visitor.type_field(_type_code->fields[8], 8); vnx::accept(_visitor, items);
	_visitor.type_field(_type_code->fields[9], 9); vnx::accept(_visitor, timing_info);
	_visitor.type_end(*_type_code);
}

void SearchResult::write(std::ostream& _out) const {
	_out << "{\"__type\": \"vnx.search.SearchResult\"";
	_out << ", \"is_fail\": "; vnx::write(_out, is_fail);
	_out << ", \"has_more\": "; vnx::write(_out, has_more);
	_out << ", \"num_results_total\": "; vnx::write(_out, num_results_total);
	_out << ", \"load_time_us\": "; vnx::write(_out, load_time_us);
	_out << ", \"compute_time_us\": "; vnx::write(_out, compute_time_us);
	_out << ", \"error_msg\": "; vnx::write(_out, error_msg);
	_out << ", \"options\": "; vnx::write(_out, options);
	_out << ", \"words\": "; vnx::write(_out, words);
	_out << ", \"items\": "; vnx::write(_out, items);
	_out << ", \"timing_info\": "; vnx::write(_out, timing_info);
	_out << "}";
}

void SearchResult::read(std::istream& _in) {
	if(auto _json = vnx::read_json(_in)) {
		from_object(_json->to_object());
	}
}

vnx::Object SearchResult::to_object() const {
	vnx::Object _object;
	_object["__type"] = "vnx.search.SearchResult";
	_object["is_fail"] = is_fail;
	_object["has_more"] = has_more;
	_object["num_results_total"] = num_results_total;
	_object["load_time_us"] = load_time_us;
	_object["compute_time_us"] = compute_time_us;
	_object["error_msg"] = error_msg;
	_object["options"] = options;
	_object["words"] = words;
	_object["items"] = items;
	_object["timing_info"] = timing_info;
	return _object;
}

void SearchResult::from_object(const vnx::Object& _object) {
	for(const auto& _entry : _object.field) {
		if(_entry.first == "compute_time_us") {
			_entry.second.to(compute_time_us);
		} else if(_entry.first == "error_msg") {
			_entry.second.to(error_msg);
		} else if(_entry.first == "has_more") {
			_entry.second.to(has_more);
		} else if(_entry.first == "is_fail") {
			_entry.second.to(is_fail);
		} else if(_entry.first == "items") {
			_entry.second.to(items);
		} else if(_entry.first == "load_time_us") {
			_entry.second.to(load_time_us);
		} else if(_entry.first == "num_results_total") {
			_entry.second.to(num_results_total);
		} else if(_entry.first == "options") {
			_entry.second.to(options);
		} else if(_entry.first == "timing_info") {
			_entry.second.to(timing_info);
		} else if(_entry.first == "words") {
			_entry.second.to(words);
		}
	}
}

vnx::Variant SearchResult::get_field(const std::string& _name) const {
	if(_name == "is_fail") {
		return vnx::Variant(is_fail);
	}
	if(_name == "has_more") {
		return vnx::Variant(has_more);
	}
	if(_name == "num_results_total") {
		return vnx::Variant(num_results_total);
	}
	if(_name == "load_time_us") {
		return vnx::Variant(load_time_us);
	}
	if(_name == "compute_time_us") {
		return vnx::Variant(compute_time_us);
	}
	if(_name == "error_msg") {
		return vnx::Variant(error_msg);
	}
	if(_name == "options") {
		return vnx::Variant(options);
	}
	if(_name == "words") {
		return vnx::Variant(words);
	}
	if(_name == "items") {
		return vnx::Variant(items);
	}
	if(_name == "timing_info") {
		return vnx::Variant(timing_info);
	}
	return vnx::Variant();
}

void SearchResult::set_field(const std::string& _name, const vnx::Variant& _value) {
	if(_name == "is_fail") {
		_value.to(is_fail);
	} else if(_name == "has_more") {
		_value.to(has_more);
	} else if(_name == "num_results_total") {
		_value.to(num_results_total);
	} else if(_name == "load_time_us") {
		_value.to(load_time_us);
	} else if(_name == "compute_time_us") {
		_value.to(compute_time_us);
	} else if(_name == "error_msg") {
		_value.to(error_msg);
	} else if(_name == "options") {
		_value.to(options);
	} else if(_name == "words") {
		_value.to(words);
	} else if(_name == "items") {
		_value.to(items);
	} else if(_name == "timing_info") {
		_value.to(timing_info);
	} else {
		throw std::logic_error("no such field: '" + _name + "'");
	}
}

/// \private
std::ostream& operator<<(std::ostream& _out, const SearchResult& _value) {
	_value.write(_out);
	return _out;
}

/// \private
std::istream& operator>>(std::istream& _in, SearchResult& _value) {
	_value.read(_in);
	return _in;
}

const vnx::TypeCode* SearchResult::static_get_type_code() {
	const vnx::TypeCode* type_code = vnx::get_type_code(VNX_TYPE_HASH);
	if(!type_code) {
		type_code = vnx::register_type_code(static_create_type_code());
	}
	return type_code;
}

std::shared_ptr<vnx::TypeCode> SearchResult::static_create_type_code() {
	auto type_code = std::make_shared<vnx::TypeCode>();
	type_code->name = "vnx.search.SearchResult";
	type_code->type_hash = vnx::Hash64(0x659ce58d97581ddull);
	type_code->code_hash = vnx::Hash64(0xa4249ffea269ad0cull);
	type_code->is_native = true;
	type_code->is_class = true;
	type_code->native_size = sizeof(::vnx::search::SearchResult);
	type_code->create_value = []() -> std::shared_ptr<vnx::Value> { return std::make_shared<SearchResult>(); };
	type_code->depends.resize(2);
	type_code->depends[0] = ::vnx::search::query_options_t::static_get_type_code();
	type_code->depends[1] = ::vnx::search::result_item_t::static_get_type_code();
	type_code->fields.resize(10);
	{
		auto& field = type_code->fields[0];
		field.data_size = 1;
		field.name = "is_fail";
		field.code = {31};
	}
	{
		auto& field = type_code->fields[1];
		field.data_size = 1;
		field.name = "has_more";
		field.code = {31};
	}
	{
		auto& field = type_code->fields[2];
		field.data_size = 4;
		field.name = "num_results_total";
		field.code = {7};
	}
	{
		auto& field = type_code->fields[3];
		field.data_size = 4;
		field.name = "load_time_us";
		field.code = {7};
	}
	{
		auto& field = type_code->fields[4];
		field.data_size = 4;
		field.name = "compute_time_us";
		field.code = {7};
	}
	{
		auto& field = type_code->fields[5];
		field.is_extended = true;
		field.name = "error_msg";
		field.code = {32};
	}
	{
		auto& field = type_code->fields[6];
		field.is_extended = true;
		field.name = "options";
		field.code = {19, 0};
	}
	{
		auto& field = type_code->fields[7];
		field.is_extended = true;
		field.name = "words";
		field.code = {12, 32};
	}
	{
		auto& field = type_code->fields[8];
		field.is_extended = true;
		field.name = "items";
		field.code = {12, 19, 1};
	}
	{
		auto& field = type_code->fields[9];
		field.is_extended = true;
		field.name = "timing_info";
		field.code = {13, 3, 32, 7};
	}
	type_code->build();
	return type_code;
}


} // namespace vnx
} // namespace search


namespace vnx {

void read(TypeInput& in, ::vnx::search::SearchResult& value, const TypeCode* type_code, const uint16_t* code) {
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
		if(const auto* const _field = type_code->field_map[0]) {
			vnx::read_value(_buf + _field->offset, value.is_fail, _field->code.data());
		}
		if(const auto* const _field = type_code->field_map[1]) {
			vnx::read_value(_buf + _field->offset, value.has_more, _field->code.data());
		}
		if(const auto* const _field = type_code->field_map[2]) {
			vnx::read_value(_buf + _field->offset, value.num_results_total, _field->code.data());
		}
		if(const auto* const _field = type_code->field_map[3]) {
			vnx::read_value(_buf + _field->offset, value.load_time_us, _field->code.data());
		}
		if(const auto* const _field = type_code->field_map[4]) {
			vnx::read_value(_buf + _field->offset, value.compute_time_us, _field->code.data());
		}
	}
	for(const auto* _field : type_code->ext_fields) {
		switch(_field->native_index) {
			case 5: vnx::read(in, value.error_msg, type_code, _field->code.data()); break;
			case 6: vnx::read(in, value.options, type_code, _field->code.data()); break;
			case 7: vnx::read(in, value.words, type_code, _field->code.data()); break;
			case 8: vnx::read(in, value.items, type_code, _field->code.data()); break;
			case 9: vnx::read(in, value.timing_info, type_code, _field->code.data()); break;
			default: vnx::skip(in, type_code, _field->code.data());
		}
	}
}

void write(TypeOutput& out, const ::vnx::search::SearchResult& value, const TypeCode* type_code, const uint16_t* code) {
	if(code && code[0] == CODE_OBJECT) {
		vnx::write(out, value.to_object(), nullptr, code);
		return;
	}
	if(!type_code || (code && code[0] == CODE_ANY)) {
		type_code = vnx::search::vnx_native_type_code_SearchResult;
		out.write_type_code(type_code);
		vnx::write_class_header<::vnx::search::SearchResult>(out);
	}
	else if(code && code[0] == CODE_STRUCT) {
		type_code = type_code->depends[code[1]];
	}
	char* const _buf = out.write(14);
	vnx::write_value(_buf + 0, value.is_fail);
	vnx::write_value(_buf + 1, value.has_more);
	vnx::write_value(_buf + 2, value.num_results_total);
	vnx::write_value(_buf + 6, value.load_time_us);
	vnx::write_value(_buf + 10, value.compute_time_us);
	vnx::write(out, value.error_msg, type_code, type_code->fields[5].code.data());
	vnx::write(out, value.options, type_code, type_code->fields[6].code.data());
	vnx::write(out, value.words, type_code, type_code->fields[7].code.data());
	vnx::write(out, value.items, type_code, type_code->fields[8].code.data());
	vnx::write(out, value.timing_info, type_code, type_code->fields[9].code.data());
}

void read(std::istream& in, ::vnx::search::SearchResult& value) {
	value.read(in);
}

void write(std::ostream& out, const ::vnx::search::SearchResult& value) {
	value.write(out);
}

void accept(Visitor& visitor, const ::vnx::search::SearchResult& value) {
	value.accept(visitor);
}

} // vnx
