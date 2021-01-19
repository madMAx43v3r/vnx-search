
// AUTO GENERATED by vnxcppcodegen

#include <vnx/search/package.hxx>
#include <vnx/search/SearchInterface_suggest_domains.hxx>
#include <vnx/Value.h>
#include <vnx/search/SearchInterface_suggest_domains_return.hxx>

#include <vnx/vnx.h>


namespace vnx {
namespace search {


const vnx::Hash64 SearchInterface_suggest_domains::VNX_TYPE_HASH(0x658934ab6e353dfeull);
const vnx::Hash64 SearchInterface_suggest_domains::VNX_CODE_HASH(0x55df9f66fc8503ull);

vnx::Hash64 SearchInterface_suggest_domains::get_type_hash() const {
	return VNX_TYPE_HASH;
}

std::string SearchInterface_suggest_domains::get_type_name() const {
	return "vnx.search.SearchInterface.suggest_domains";
}

const vnx::TypeCode* SearchInterface_suggest_domains::get_type_code() const {
	return vnx::search::vnx_native_type_code_SearchInterface_suggest_domains;
}

std::shared_ptr<SearchInterface_suggest_domains> SearchInterface_suggest_domains::create() {
	return std::make_shared<SearchInterface_suggest_domains>();
}

std::shared_ptr<vnx::Value> SearchInterface_suggest_domains::clone() const {
	return std::make_shared<SearchInterface_suggest_domains>(*this);
}

void SearchInterface_suggest_domains::read(vnx::TypeInput& _in, const vnx::TypeCode* _type_code, const uint16_t* _code) {
	vnx::read(_in, *this, _type_code, _code);
}

void SearchInterface_suggest_domains::write(vnx::TypeOutput& _out, const vnx::TypeCode* _type_code, const uint16_t* _code) const {
	vnx::write(_out, *this, _type_code, _code);
}

void SearchInterface_suggest_domains::accept(vnx::Visitor& _visitor) const {
	const vnx::TypeCode* _type_code = vnx::search::vnx_native_type_code_SearchInterface_suggest_domains;
	_visitor.type_begin(*_type_code);
	_visitor.type_field(_type_code->fields[0], 0); vnx::accept(_visitor, prefix);
	_visitor.type_field(_type_code->fields[1], 1); vnx::accept(_visitor, limit);
	_visitor.type_end(*_type_code);
}

void SearchInterface_suggest_domains::write(std::ostream& _out) const {
	_out << "{\"__type\": \"vnx.search.SearchInterface.suggest_domains\"";
	_out << ", \"prefix\": "; vnx::write(_out, prefix);
	_out << ", \"limit\": "; vnx::write(_out, limit);
	_out << "}";
}

void SearchInterface_suggest_domains::read(std::istream& _in) {
	if(auto _json = vnx::read_json(_in)) {
		from_object(_json->to_object());
	}
}

vnx::Object SearchInterface_suggest_domains::to_object() const {
	vnx::Object _object;
	_object["__type"] = "vnx.search.SearchInterface.suggest_domains";
	_object["prefix"] = prefix;
	_object["limit"] = limit;
	return _object;
}

void SearchInterface_suggest_domains::from_object(const vnx::Object& _object) {
	for(const auto& _entry : _object.field) {
		if(_entry.first == "limit") {
			_entry.second.to(limit);
		} else if(_entry.first == "prefix") {
			_entry.second.to(prefix);
		}
	}
}

vnx::Variant SearchInterface_suggest_domains::get_field(const std::string& _name) const {
	if(_name == "prefix") {
		return vnx::Variant(prefix);
	}
	if(_name == "limit") {
		return vnx::Variant(limit);
	}
	return vnx::Variant();
}

void SearchInterface_suggest_domains::set_field(const std::string& _name, const vnx::Variant& _value) {
	if(_name == "prefix") {
		_value.to(prefix);
	} else if(_name == "limit") {
		_value.to(limit);
	} else {
		throw std::logic_error("no such field: '" + _name + "'");
	}
}

/// \private
std::ostream& operator<<(std::ostream& _out, const SearchInterface_suggest_domains& _value) {
	_value.write(_out);
	return _out;
}

/// \private
std::istream& operator>>(std::istream& _in, SearchInterface_suggest_domains& _value) {
	_value.read(_in);
	return _in;
}

const vnx::TypeCode* SearchInterface_suggest_domains::static_get_type_code() {
	const vnx::TypeCode* type_code = vnx::get_type_code(VNX_TYPE_HASH);
	if(!type_code) {
		type_code = vnx::register_type_code(static_create_type_code());
	}
	return type_code;
}

std::shared_ptr<vnx::TypeCode> SearchInterface_suggest_domains::static_create_type_code() {
	std::shared_ptr<vnx::TypeCode> type_code = std::make_shared<vnx::TypeCode>();
	type_code->name = "vnx.search.SearchInterface.suggest_domains";
	type_code->type_hash = vnx::Hash64(0x658934ab6e353dfeull);
	type_code->code_hash = vnx::Hash64(0x55df9f66fc8503ull);
	type_code->is_native = true;
	type_code->is_class = true;
	type_code->is_method = true;
	type_code->create_value = []() -> std::shared_ptr<vnx::Value> { return std::make_shared<SearchInterface_suggest_domains>(); };
	type_code->is_const = true;
	type_code->return_type = ::vnx::search::SearchInterface_suggest_domains_return::static_get_type_code();
	type_code->fields.resize(2);
	{
		vnx::TypeField& field = type_code->fields[0];
		field.is_extended = true;
		field.name = "prefix";
		field.code = {32};
	}
	{
		vnx::TypeField& field = type_code->fields[1];
		field.name = "limit";
		field.code = {7};
	}
	type_code->build();
	return type_code;
}


} // namespace vnx
} // namespace search


namespace vnx {

void read(TypeInput& in, ::vnx::search::SearchInterface_suggest_domains& value, const TypeCode* type_code, const uint16_t* code) {
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
		{
			const vnx::TypeField* const _field = type_code->field_map[1];
			if(_field) {
				vnx::read_value(_buf + _field->offset, value.limit, _field->code.data());
			}
		}
	}
	for(const vnx::TypeField* _field : type_code->ext_fields) {
		switch(_field->native_index) {
			case 0: vnx::read(in, value.prefix, type_code, _field->code.data()); break;
			default: vnx::skip(in, type_code, _field->code.data());
		}
	}
}

void write(TypeOutput& out, const ::vnx::search::SearchInterface_suggest_domains& value, const TypeCode* type_code, const uint16_t* code) {
	if(code && code[0] == CODE_OBJECT) {
		vnx::write(out, value.to_object(), nullptr, code);
		return;
	}
	if(!type_code || (code && code[0] == CODE_ANY)) {
		type_code = vnx::search::vnx_native_type_code_SearchInterface_suggest_domains;
		out.write_type_code(type_code);
		vnx::write_class_header<::vnx::search::SearchInterface_suggest_domains>(out);
	}
	else if(code && code[0] == CODE_STRUCT) {
		type_code = type_code->depends[code[1]];
	}
	char* const _buf = out.write(4);
	vnx::write_value(_buf + 0, value.limit);
	vnx::write(out, value.prefix, type_code, type_code->fields[0].code.data());
}

void read(std::istream& in, ::vnx::search::SearchInterface_suggest_domains& value) {
	value.read(in);
}

void write(std::ostream& out, const ::vnx::search::SearchInterface_suggest_domains& value) {
	value.write(out);
}

void accept(Visitor& visitor, const ::vnx::search::SearchInterface_suggest_domains& value) {
	value.accept(visitor);
}

} // vnx
