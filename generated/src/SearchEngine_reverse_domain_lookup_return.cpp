
// AUTO GENERATED by vnxcppcodegen

#include <vnx/search/package.hxx>
#include <vnx/search/SearchEngine_reverse_domain_lookup_return.hxx>
#include <vnx/Value.h>

#include <vnx/vnx.h>


namespace vnx {
namespace search {


const vnx::Hash64 SearchEngine_reverse_domain_lookup_return::VNX_TYPE_HASH(0x8f0160842cd83465ull);
const vnx::Hash64 SearchEngine_reverse_domain_lookup_return::VNX_CODE_HASH(0x246b319aef73dcaull);

vnx::Hash64 SearchEngine_reverse_domain_lookup_return::get_type_hash() const {
	return VNX_TYPE_HASH;
}

const char* SearchEngine_reverse_domain_lookup_return::get_type_name() const {
	return "vnx.search.SearchEngine.reverse_domain_lookup.return";
}
const vnx::TypeCode* SearchEngine_reverse_domain_lookup_return::get_type_code() const {
	return vnx::search::vnx_native_type_code_SearchEngine_reverse_domain_lookup_return;
}

std::shared_ptr<SearchEngine_reverse_domain_lookup_return> SearchEngine_reverse_domain_lookup_return::create() {
	return std::make_shared<SearchEngine_reverse_domain_lookup_return>();
}

std::shared_ptr<vnx::Value> SearchEngine_reverse_domain_lookup_return::clone() const {
	return std::make_shared<SearchEngine_reverse_domain_lookup_return>(*this);
}

void SearchEngine_reverse_domain_lookup_return::read(vnx::TypeInput& _in, const vnx::TypeCode* _type_code, const uint16_t* _code) {
	vnx::read(_in, *this, _type_code, _code);
}

void SearchEngine_reverse_domain_lookup_return::write(vnx::TypeOutput& _out, const vnx::TypeCode* _type_code, const uint16_t* _code) const {
	vnx::write(_out, *this, _type_code, _code);
}

void SearchEngine_reverse_domain_lookup_return::accept(vnx::Visitor& _visitor) const {
	const vnx::TypeCode* _type_code = vnx::search::vnx_native_type_code_SearchEngine_reverse_domain_lookup_return;
	_visitor.type_begin(*_type_code);
	_visitor.type_field(_type_code->fields[0], 0); vnx::accept(_visitor, _ret_0);
	_visitor.type_end(*_type_code);
}

void SearchEngine_reverse_domain_lookup_return::write(std::ostream& _out) const {
	_out << "{\"__type\": \"vnx.search.SearchEngine.reverse_domain_lookup.return\"";
	_out << ", \"_ret_0\": "; vnx::write(_out, _ret_0);
	_out << "}";
}

void SearchEngine_reverse_domain_lookup_return::read(std::istream& _in) {
	std::map<std::string, std::string> _object;
	vnx::read_object(_in, _object);
	for(const auto& _entry : _object) {
		if(_entry.first == "_ret_0") {
			vnx::from_string(_entry.second, _ret_0);
		}
	}
}

vnx::Object SearchEngine_reverse_domain_lookup_return::to_object() const {
	vnx::Object _object;
	_object["__type"] = "vnx.search.SearchEngine.reverse_domain_lookup.return";
	_object["_ret_0"] = _ret_0;
	return _object;
}

void SearchEngine_reverse_domain_lookup_return::from_object(const vnx::Object& _object) {
	for(const auto& _entry : _object.field) {
		if(_entry.first == "_ret_0") {
			_entry.second.to(_ret_0);
		}
	}
}

/// \private
std::ostream& operator<<(std::ostream& _out, const SearchEngine_reverse_domain_lookup_return& _value) {
	_value.write(_out);
	return _out;
}

/// \private
std::istream& operator>>(std::istream& _in, SearchEngine_reverse_domain_lookup_return& _value) {
	_value.read(_in);
	return _in;
}

const vnx::TypeCode* SearchEngine_reverse_domain_lookup_return::static_get_type_code() {
	const vnx::TypeCode* type_code = vnx::get_type_code(VNX_TYPE_HASH);
	if(!type_code) {
		type_code = vnx::register_type_code(static_create_type_code());
	}
	return type_code;
}

std::shared_ptr<vnx::TypeCode> SearchEngine_reverse_domain_lookup_return::static_create_type_code() {
	std::shared_ptr<vnx::TypeCode> type_code = std::make_shared<vnx::TypeCode>();
	type_code->name = "vnx.search.SearchEngine.reverse_domain_lookup.return";
	type_code->type_hash = vnx::Hash64(0x8f0160842cd83465ull);
	type_code->code_hash = vnx::Hash64(0x246b319aef73dcaull);
	type_code->is_native = true;
	type_code->is_class = true;
	type_code->is_return = true;
	type_code->create_value = []() -> std::shared_ptr<vnx::Value> { return std::make_shared<SearchEngine_reverse_domain_lookup_return>(); };
	type_code->fields.resize(1);
	{
		vnx::TypeField& field = type_code->fields[0];
		field.is_extended = true;
		field.name = "_ret_0";
		field.code = {12, 23, 2, 4, 6, 12, 5, 3};
	}
	type_code->build();
	return type_code;
}


} // namespace vnx
} // namespace search


namespace vnx {

void read(TypeInput& in, ::vnx::search::SearchEngine_reverse_domain_lookup_return& value, const TypeCode* type_code, const uint16_t* code) {
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
			case 0: vnx::read(in, value._ret_0, type_code, _field->code.data()); break;
			default: vnx::skip(in, type_code, _field->code.data());
		}
	}
}

void write(TypeOutput& out, const ::vnx::search::SearchEngine_reverse_domain_lookup_return& value, const TypeCode* type_code, const uint16_t* code) {
	if(!type_code || (code && code[0] == CODE_ANY)) {
		type_code = vnx::search::vnx_native_type_code_SearchEngine_reverse_domain_lookup_return;
		out.write_type_code(type_code);
		vnx::write_class_header<::vnx::search::SearchEngine_reverse_domain_lookup_return>(out);
	}
	if(code && code[0] == CODE_STRUCT) {
		type_code = type_code->depends[code[1]];
	}
	vnx::write(out, value._ret_0, type_code, type_code->fields[0].code.data());
}

void read(std::istream& in, ::vnx::search::SearchEngine_reverse_domain_lookup_return& value) {
	value.read(in);
}

void write(std::ostream& out, const ::vnx::search::SearchEngine_reverse_domain_lookup_return& value) {
	value.write(out);
}

void accept(Visitor& visitor, const ::vnx::search::SearchEngine_reverse_domain_lookup_return& value) {
	value.accept(visitor);
}

} // vnx
