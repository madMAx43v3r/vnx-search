
// AUTO GENERATED by vnxcppcodegen

#include <vnx/search/package.hxx>
#include <vnx/search/CrawlProcessor_get_stats.hxx>
#include <vnx/Value.h>
#include <vnx/search/CrawlProcessor_get_stats_return.hxx>

#include <vnx/vnx.h>


namespace vnx {
namespace search {


const vnx::Hash64 CrawlProcessor_get_stats::VNX_TYPE_HASH(0x7b8fefbbb0397a0dull);
const vnx::Hash64 CrawlProcessor_get_stats::VNX_CODE_HASH(0xa7f0061aeb6189a1ull);

vnx::Hash64 CrawlProcessor_get_stats::get_type_hash() const {
	return VNX_TYPE_HASH;
}

const char* CrawlProcessor_get_stats::get_type_name() const {
	return "vnx.search.CrawlProcessor.get_stats";
}

const vnx::TypeCode* CrawlProcessor_get_stats::get_type_code() const {
	return vnx::search::vnx_native_type_code_CrawlProcessor_get_stats;
}

std::shared_ptr<CrawlProcessor_get_stats> CrawlProcessor_get_stats::create() {
	return std::make_shared<CrawlProcessor_get_stats>();
}

std::shared_ptr<vnx::Value> CrawlProcessor_get_stats::clone() const {
	return std::make_shared<CrawlProcessor_get_stats>(*this);
}

void CrawlProcessor_get_stats::read(vnx::TypeInput& _in, const vnx::TypeCode* _type_code, const uint16_t* _code) {
	vnx::read(_in, *this, _type_code, _code);
}

void CrawlProcessor_get_stats::write(vnx::TypeOutput& _out, const vnx::TypeCode* _type_code, const uint16_t* _code) const {
	vnx::write(_out, *this, _type_code, _code);
}

void CrawlProcessor_get_stats::accept(vnx::Visitor& _visitor) const {
	const vnx::TypeCode* _type_code = vnx::search::vnx_native_type_code_CrawlProcessor_get_stats;
	_visitor.type_begin(*_type_code);
	_visitor.type_field(_type_code->fields[0], 0); vnx::accept(_visitor, limit);
	_visitor.type_end(*_type_code);
}

void CrawlProcessor_get_stats::write(std::ostream& _out) const {
	_out << "{\"__type\": \"vnx.search.CrawlProcessor.get_stats\"";
	_out << ", \"limit\": "; vnx::write(_out, limit);
	_out << "}";
}

void CrawlProcessor_get_stats::read(std::istream& _in) {
	std::map<std::string, std::string> _object;
	vnx::read_object(_in, _object);
	for(const auto& _entry : _object) {
		if(_entry.first == "limit") {
			vnx::from_string(_entry.second, limit);
		}
	}
}

vnx::Object CrawlProcessor_get_stats::to_object() const {
	vnx::Object _object;
	_object["__type"] = "vnx.search.CrawlProcessor.get_stats";
	_object["limit"] = limit;
	return _object;
}

void CrawlProcessor_get_stats::from_object(const vnx::Object& _object) {
	for(const auto& _entry : _object.field) {
		if(_entry.first == "limit") {
			_entry.second.to(limit);
		}
	}
}

vnx::Variant CrawlProcessor_get_stats::get_field(const std::string& _name) const {
	if(_name == "limit") {
		return vnx::Variant(limit);
	}
	return vnx::Variant();
}

void CrawlProcessor_get_stats::set_field(const std::string& _name, const vnx::Variant& _value) {
	if(_name == "limit") {
		_value.to(limit);
	} else {
		throw std::logic_error("no such field: '" + _name + "'");
	}
}

/// \private
std::ostream& operator<<(std::ostream& _out, const CrawlProcessor_get_stats& _value) {
	_value.write(_out);
	return _out;
}

/// \private
std::istream& operator>>(std::istream& _in, CrawlProcessor_get_stats& _value) {
	_value.read(_in);
	return _in;
}

const vnx::TypeCode* CrawlProcessor_get_stats::static_get_type_code() {
	const vnx::TypeCode* type_code = vnx::get_type_code(VNX_TYPE_HASH);
	if(!type_code) {
		type_code = vnx::register_type_code(static_create_type_code());
	}
	return type_code;
}

std::shared_ptr<vnx::TypeCode> CrawlProcessor_get_stats::static_create_type_code() {
	std::shared_ptr<vnx::TypeCode> type_code = std::make_shared<vnx::TypeCode>();
	type_code->name = "vnx.search.CrawlProcessor.get_stats";
	type_code->type_hash = vnx::Hash64(0x7b8fefbbb0397a0dull);
	type_code->code_hash = vnx::Hash64(0xa7f0061aeb6189a1ull);
	type_code->is_native = true;
	type_code->is_class = true;
	type_code->is_method = true;
	type_code->create_value = []() -> std::shared_ptr<vnx::Value> { return std::make_shared<CrawlProcessor_get_stats>(); };
	type_code->return_type = ::vnx::search::CrawlProcessor_get_stats_return::static_get_type_code();
	type_code->fields.resize(1);
	{
		vnx::TypeField& field = type_code->fields[0];
		field.name = "limit";
		field.code = {7};
	}
	type_code->build();
	return type_code;
}


} // namespace vnx
} // namespace search


namespace vnx {

void read(TypeInput& in, ::vnx::search::CrawlProcessor_get_stats& value, const TypeCode* type_code, const uint16_t* code) {
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
			const vnx::TypeField* const _field = type_code->field_map[0];
			if(_field) {
				vnx::read_value(_buf + _field->offset, value.limit, _field->code.data());
			}
		}
	}
	for(const vnx::TypeField* _field : type_code->ext_fields) {
		switch(_field->native_index) {
			default: vnx::skip(in, type_code, _field->code.data());
		}
	}
}

void write(TypeOutput& out, const ::vnx::search::CrawlProcessor_get_stats& value, const TypeCode* type_code, const uint16_t* code) {
	if(code && code[0] == CODE_OBJECT) {
		vnx::write(out, value.to_object(), nullptr, code);
		return;
	}
	if(!type_code || (code && code[0] == CODE_ANY)) {
		type_code = vnx::search::vnx_native_type_code_CrawlProcessor_get_stats;
		out.write_type_code(type_code);
		vnx::write_class_header<::vnx::search::CrawlProcessor_get_stats>(out);
	}
	if(code && code[0] == CODE_STRUCT) {
		type_code = type_code->depends[code[1]];
	}
	char* const _buf = out.write(4);
	vnx::write_value(_buf + 0, value.limit);
}

void read(std::istream& in, ::vnx::search::CrawlProcessor_get_stats& value) {
	value.read(in);
}

void write(std::ostream& out, const ::vnx::search::CrawlProcessor_get_stats& value) {
	value.write(out);
}

void accept(Visitor& visitor, const ::vnx::search::CrawlProcessor_get_stats& value) {
	value.accept(visitor);
}

} // vnx
