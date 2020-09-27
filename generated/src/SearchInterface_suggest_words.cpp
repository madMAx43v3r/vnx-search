
// AUTO GENERATED by vnxcppcodegen

#include <vnx/search/package.hxx>
#include <vnx/search/SearchInterface_suggest_words.hxx>
#include <vnx/Value.h>
#include <vnx/search/SearchInterface_suggest_words_return.hxx>

#include <vnx/vnx.h>


namespace vnx {
namespace search {


const vnx::Hash64 SearchInterface_suggest_words::VNX_TYPE_HASH(0xe51abc2dddd35827ull);
const vnx::Hash64 SearchInterface_suggest_words::VNX_CODE_HASH(0x573f1a5feca40c19ull);

vnx::Hash64 SearchInterface_suggest_words::get_type_hash() const {
	return VNX_TYPE_HASH;
}

const char* SearchInterface_suggest_words::get_type_name() const {
	return "vnx.search.SearchInterface.suggest_words";
}

const vnx::TypeCode* SearchInterface_suggest_words::get_type_code() const {
	return vnx::search::vnx_native_type_code_SearchInterface_suggest_words;
}

std::shared_ptr<SearchInterface_suggest_words> SearchInterface_suggest_words::create() {
	return std::make_shared<SearchInterface_suggest_words>();
}

std::shared_ptr<vnx::Value> SearchInterface_suggest_words::clone() const {
	return std::make_shared<SearchInterface_suggest_words>(*this);
}

void SearchInterface_suggest_words::read(vnx::TypeInput& _in, const vnx::TypeCode* _type_code, const uint16_t* _code) {
	vnx::read(_in, *this, _type_code, _code);
}

void SearchInterface_suggest_words::write(vnx::TypeOutput& _out, const vnx::TypeCode* _type_code, const uint16_t* _code) const {
	vnx::write(_out, *this, _type_code, _code);
}

void SearchInterface_suggest_words::accept(vnx::Visitor& _visitor) const {
	const vnx::TypeCode* _type_code = vnx::search::vnx_native_type_code_SearchInterface_suggest_words;
	_visitor.type_begin(*_type_code);
	_visitor.type_field(_type_code->fields[0], 0); vnx::accept(_visitor, prefix);
	_visitor.type_field(_type_code->fields[1], 1); vnx::accept(_visitor, limit);
	_visitor.type_end(*_type_code);
}

void SearchInterface_suggest_words::write(std::ostream& _out) const {
	_out << "{\"__type\": \"vnx.search.SearchInterface.suggest_words\"";
	_out << ", \"prefix\": "; vnx::write(_out, prefix);
	_out << ", \"limit\": "; vnx::write(_out, limit);
	_out << "}";
}

void SearchInterface_suggest_words::read(std::istream& _in) {
	std::map<std::string, std::string> _object;
	vnx::read_object(_in, _object);
	for(const auto& _entry : _object) {
		if(_entry.first == "limit") {
			vnx::from_string(_entry.second, limit);
		} else if(_entry.first == "prefix") {
			vnx::from_string(_entry.second, prefix);
		}
	}
}

vnx::Object SearchInterface_suggest_words::to_object() const {
	vnx::Object _object;
	_object["__type"] = "vnx.search.SearchInterface.suggest_words";
	_object["prefix"] = prefix;
	_object["limit"] = limit;
	return _object;
}

void SearchInterface_suggest_words::from_object(const vnx::Object& _object) {
	for(const auto& _entry : _object.field) {
		if(_entry.first == "limit") {
			_entry.second.to(limit);
		} else if(_entry.first == "prefix") {
			_entry.second.to(prefix);
		}
	}
}

vnx::Variant SearchInterface_suggest_words::get_field(const std::string& _name) const {
	if(_name == "prefix") {
		return vnx::Variant(prefix);
	}
	if(_name == "limit") {
		return vnx::Variant(limit);
	}
	return vnx::Variant();
}

void SearchInterface_suggest_words::set_field(const std::string& _name, const vnx::Variant& _value) {
	if(_name == "prefix") {
		_value.to(prefix);
	} else if(_name == "limit") {
		_value.to(limit);
	} else {
		throw std::logic_error("no such field: '" + _name + "'");
	}
}

/// \private
std::ostream& operator<<(std::ostream& _out, const SearchInterface_suggest_words& _value) {
	_value.write(_out);
	return _out;
}

/// \private
std::istream& operator>>(std::istream& _in, SearchInterface_suggest_words& _value) {
	_value.read(_in);
	return _in;
}

const vnx::TypeCode* SearchInterface_suggest_words::static_get_type_code() {
	const vnx::TypeCode* type_code = vnx::get_type_code(VNX_TYPE_HASH);
	if(!type_code) {
		type_code = vnx::register_type_code(static_create_type_code());
	}
	return type_code;
}

std::shared_ptr<vnx::TypeCode> SearchInterface_suggest_words::static_create_type_code() {
	std::shared_ptr<vnx::TypeCode> type_code = std::make_shared<vnx::TypeCode>();
	type_code->name = "vnx.search.SearchInterface.suggest_words";
	type_code->type_hash = vnx::Hash64(0xe51abc2dddd35827ull);
	type_code->code_hash = vnx::Hash64(0x573f1a5feca40c19ull);
	type_code->is_native = true;
	type_code->is_class = true;
	type_code->is_method = true;
	type_code->create_value = []() -> std::shared_ptr<vnx::Value> { return std::make_shared<SearchInterface_suggest_words>(); };
	type_code->return_type = ::vnx::search::SearchInterface_suggest_words_return::static_get_type_code();
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

void read(TypeInput& in, ::vnx::search::SearchInterface_suggest_words& value, const TypeCode* type_code, const uint16_t* code) {
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

void write(TypeOutput& out, const ::vnx::search::SearchInterface_suggest_words& value, const TypeCode* type_code, const uint16_t* code) {
	if(code && code[0] == CODE_OBJECT) {
		vnx::write(out, value.to_object(), nullptr, code);
		return;
	}
	if(!type_code || (code && code[0] == CODE_ANY)) {
		type_code = vnx::search::vnx_native_type_code_SearchInterface_suggest_words;
		out.write_type_code(type_code);
		vnx::write_class_header<::vnx::search::SearchInterface_suggest_words>(out);
	}
	if(code && code[0] == CODE_STRUCT) {
		type_code = type_code->depends[code[1]];
	}
	char* const _buf = out.write(4);
	vnx::write_value(_buf + 0, value.limit);
	vnx::write(out, value.prefix, type_code, type_code->fields[0].code.data());
}

void read(std::istream& in, ::vnx::search::SearchInterface_suggest_words& value) {
	value.read(in);
}

void write(std::ostream& out, const ::vnx::search::SearchInterface_suggest_words& value) {
	value.write(out);
}

void accept(Visitor& visitor, const ::vnx::search::SearchInterface_suggest_words& value) {
	value.accept(visitor);
}

} // vnx
