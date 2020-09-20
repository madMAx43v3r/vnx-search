
// AUTO GENERATED by vnxcppcodegen

#include <vnx/search/package.hxx>
#include <vnx/search/QueryInterface_query.hxx>
#include <vnx/Value.h>
#include <vnx/search/QueryInterface_query_return.hxx>
#include <vnx/search/query_options_t.hxx>

#include <vnx/vnx.h>


namespace vnx {
namespace search {


const vnx::Hash64 QueryInterface_query::VNX_TYPE_HASH(0xd7ca13b33b457bbaull);
const vnx::Hash64 QueryInterface_query::VNX_CODE_HASH(0xf7dc2369451f533dull);

vnx::Hash64 QueryInterface_query::get_type_hash() const {
	return VNX_TYPE_HASH;
}

const char* QueryInterface_query::get_type_name() const {
	return "vnx.search.QueryInterface.query";
}
const vnx::TypeCode* QueryInterface_query::get_type_code() const {
	return vnx::search::vnx_native_type_code_QueryInterface_query;
}

std::shared_ptr<QueryInterface_query> QueryInterface_query::create() {
	return std::make_shared<QueryInterface_query>();
}

std::shared_ptr<vnx::Value> QueryInterface_query::clone() const {
	return std::make_shared<QueryInterface_query>(*this);
}

void QueryInterface_query::read(vnx::TypeInput& _in, const vnx::TypeCode* _type_code, const uint16_t* _code) {
	vnx::read(_in, *this, _type_code, _code);
}

void QueryInterface_query::write(vnx::TypeOutput& _out, const vnx::TypeCode* _type_code, const uint16_t* _code) const {
	vnx::write(_out, *this, _type_code, _code);
}

void QueryInterface_query::accept(vnx::Visitor& _visitor) const {
	const vnx::TypeCode* _type_code = vnx::search::vnx_native_type_code_QueryInterface_query;
	_visitor.type_begin(*_type_code);
	_visitor.type_field(_type_code->fields[0], 0); vnx::accept(_visitor, words);
	_visitor.type_field(_type_code->fields[1], 1); vnx::accept(_visitor, options);
	_visitor.type_end(*_type_code);
}

void QueryInterface_query::write(std::ostream& _out) const {
	_out << "{\"__type\": \"vnx.search.QueryInterface.query\"";
	_out << ", \"words\": "; vnx::write(_out, words);
	_out << ", \"options\": "; vnx::write(_out, options);
	_out << "}";
}

void QueryInterface_query::read(std::istream& _in) {
	std::map<std::string, std::string> _object;
	vnx::read_object(_in, _object);
	for(const auto& _entry : _object) {
		if(_entry.first == "options") {
			vnx::from_string(_entry.second, options);
		} else if(_entry.first == "words") {
			vnx::from_string(_entry.second, words);
		}
	}
}

vnx::Object QueryInterface_query::to_object() const {
	vnx::Object _object;
	_object["__type"] = "vnx.search.QueryInterface.query";
	_object["words"] = words;
	_object["options"] = options;
	return _object;
}

void QueryInterface_query::from_object(const vnx::Object& _object) {
	for(const auto& _entry : _object.field) {
		if(_entry.first == "options") {
			_entry.second.to(options);
		} else if(_entry.first == "words") {
			_entry.second.to(words);
		}
	}
}

vnx::Variant QueryInterface_query::get_field(const std::string& _name) const {
	if(_name == "words") {
		return vnx::Variant(words);
	}
	if(_name == "options") {
		return vnx::Variant(options);
	}
	return vnx::Variant();
}

void QueryInterface_query::set_field(const std::string& _name, const vnx::Variant& _value) {
	if(_name == "words") {
		_value.to(words);
	} else if(_name == "options") {
		_value.to(options);
	} else {
		throw std::logic_error("no such field: '" + _name + "'");
	}
}

/// \private
std::ostream& operator<<(std::ostream& _out, const QueryInterface_query& _value) {
	_value.write(_out);
	return _out;
}

/// \private
std::istream& operator>>(std::istream& _in, QueryInterface_query& _value) {
	_value.read(_in);
	return _in;
}

const vnx::TypeCode* QueryInterface_query::static_get_type_code() {
	const vnx::TypeCode* type_code = vnx::get_type_code(VNX_TYPE_HASH);
	if(!type_code) {
		type_code = vnx::register_type_code(static_create_type_code());
	}
	return type_code;
}

std::shared_ptr<vnx::TypeCode> QueryInterface_query::static_create_type_code() {
	std::shared_ptr<vnx::TypeCode> type_code = std::make_shared<vnx::TypeCode>();
	type_code->name = "vnx.search.QueryInterface.query";
	type_code->type_hash = vnx::Hash64(0xd7ca13b33b457bbaull);
	type_code->code_hash = vnx::Hash64(0xf7dc2369451f533dull);
	type_code->is_native = true;
	type_code->is_class = true;
	type_code->is_method = true;
	type_code->create_value = []() -> std::shared_ptr<vnx::Value> { return std::make_shared<QueryInterface_query>(); };
	type_code->depends.resize(1);
	type_code->depends[0] = ::vnx::search::query_options_t::static_get_type_code();
	type_code->return_type = ::vnx::search::QueryInterface_query_return::static_get_type_code();
	type_code->fields.resize(2);
	{
		vnx::TypeField& field = type_code->fields[0];
		field.is_extended = true;
		field.name = "words";
		field.code = {12, 32};
	}
	{
		vnx::TypeField& field = type_code->fields[1];
		field.is_extended = true;
		field.name = "options";
		field.code = {19, 0};
	}
	type_code->build();
	return type_code;
}


} // namespace vnx
} // namespace search


namespace vnx {

void read(TypeInput& in, ::vnx::search::QueryInterface_query& value, const TypeCode* type_code, const uint16_t* code) {
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
	if(type_code->is_matched) {
	}
	for(const vnx::TypeField* _field : type_code->ext_fields) {
		switch(_field->native_index) {
			case 0: vnx::read(in, value.words, type_code, _field->code.data()); break;
			case 1: vnx::read(in, value.options, type_code, _field->code.data()); break;
			default: vnx::skip(in, type_code, _field->code.data());
		}
	}
}

void write(TypeOutput& out, const ::vnx::search::QueryInterface_query& value, const TypeCode* type_code, const uint16_t* code) {
	if(!type_code || (code && code[0] == CODE_ANY)) {
		type_code = vnx::search::vnx_native_type_code_QueryInterface_query;
		out.write_type_code(type_code);
		vnx::write_class_header<::vnx::search::QueryInterface_query>(out);
	}
	if(code && code[0] == CODE_STRUCT) {
		type_code = type_code->depends[code[1]];
	}
	vnx::write(out, value.words, type_code, type_code->fields[0].code.data());
	vnx::write(out, value.options, type_code, type_code->fields[1].code.data());
}

void read(std::istream& in, ::vnx::search::QueryInterface_query& value) {
	value.read(in);
}

void write(std::ostream& out, const ::vnx::search::QueryInterface_query& value) {
	value.write(out);
}

void accept(Visitor& visitor, const ::vnx::search::QueryInterface_query& value) {
	value.accept(visitor);
}

} // vnx
