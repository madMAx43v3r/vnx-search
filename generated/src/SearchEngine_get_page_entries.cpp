
// AUTO GENERATED by vnxcppcodegen

#include <vnx/search/package.hxx>
#include <vnx/search/SearchEngine_get_page_entries.hxx>
#include <vnx/Value.h>
#include <vnx/search/SearchEngine_get_page_entries_return.hxx>

#include <vnx/vnx.h>


namespace vnx {
namespace search {


const vnx::Hash64 SearchEngine_get_page_entries::VNX_TYPE_HASH(0x7570dd1b83810e89ull);
const vnx::Hash64 SearchEngine_get_page_entries::VNX_CODE_HASH(0x9e316051d0f28b6bull);

vnx::Hash64 SearchEngine_get_page_entries::get_type_hash() const {
	return VNX_TYPE_HASH;
}

std::string SearchEngine_get_page_entries::get_type_name() const {
	return "vnx.search.SearchEngine.get_page_entries";
}

const vnx::TypeCode* SearchEngine_get_page_entries::get_type_code() const {
	return vnx::search::vnx_native_type_code_SearchEngine_get_page_entries;
}

std::shared_ptr<SearchEngine_get_page_entries> SearchEngine_get_page_entries::create() {
	return std::make_shared<SearchEngine_get_page_entries>();
}

std::shared_ptr<vnx::Value> SearchEngine_get_page_entries::clone() const {
	return std::make_shared<SearchEngine_get_page_entries>(*this);
}

void SearchEngine_get_page_entries::read(vnx::TypeInput& _in, const vnx::TypeCode* _type_code, const uint16_t* _code) {
	vnx::read(_in, *this, _type_code, _code);
}

void SearchEngine_get_page_entries::write(vnx::TypeOutput& _out, const vnx::TypeCode* _type_code, const uint16_t* _code) const {
	vnx::write(_out, *this, _type_code, _code);
}

void SearchEngine_get_page_entries::accept(vnx::Visitor& _visitor) const {
	const vnx::TypeCode* _type_code = vnx::search::vnx_native_type_code_SearchEngine_get_page_entries;
	_visitor.type_begin(*_type_code);
	_visitor.type_field(_type_code->fields[0], 0); vnx::accept(_visitor, page_ids);
	_visitor.type_end(*_type_code);
}

void SearchEngine_get_page_entries::write(std::ostream& _out) const {
	_out << "{\"__type\": \"vnx.search.SearchEngine.get_page_entries\"";
	_out << ", \"page_ids\": "; vnx::write(_out, page_ids);
	_out << "}";
}

void SearchEngine_get_page_entries::read(std::istream& _in) {
	if(auto _json = vnx::read_json(_in)) {
		from_object(_json->to_object());
	}
}

vnx::Object SearchEngine_get_page_entries::to_object() const {
	vnx::Object _object;
	_object["__type"] = "vnx.search.SearchEngine.get_page_entries";
	_object["page_ids"] = page_ids;
	return _object;
}

void SearchEngine_get_page_entries::from_object(const vnx::Object& _object) {
	for(const auto& _entry : _object.field) {
		if(_entry.first == "page_ids") {
			_entry.second.to(page_ids);
		}
	}
}

vnx::Variant SearchEngine_get_page_entries::get_field(const std::string& _name) const {
	if(_name == "page_ids") {
		return vnx::Variant(page_ids);
	}
	return vnx::Variant();
}

void SearchEngine_get_page_entries::set_field(const std::string& _name, const vnx::Variant& _value) {
	if(_name == "page_ids") {
		_value.to(page_ids);
	} else {
		throw std::logic_error("no such field: '" + _name + "'");
	}
}

/// \private
std::ostream& operator<<(std::ostream& _out, const SearchEngine_get_page_entries& _value) {
	_value.write(_out);
	return _out;
}

/// \private
std::istream& operator>>(std::istream& _in, SearchEngine_get_page_entries& _value) {
	_value.read(_in);
	return _in;
}

const vnx::TypeCode* SearchEngine_get_page_entries::static_get_type_code() {
	const vnx::TypeCode* type_code = vnx::get_type_code(VNX_TYPE_HASH);
	if(!type_code) {
		type_code = vnx::register_type_code(static_create_type_code());
	}
	return type_code;
}

std::shared_ptr<vnx::TypeCode> SearchEngine_get_page_entries::static_create_type_code() {
	std::shared_ptr<vnx::TypeCode> type_code = std::make_shared<vnx::TypeCode>();
	type_code->name = "vnx.search.SearchEngine.get_page_entries";
	type_code->type_hash = vnx::Hash64(0x7570dd1b83810e89ull);
	type_code->code_hash = vnx::Hash64(0x9e316051d0f28b6bull);
	type_code->is_native = true;
	type_code->is_class = true;
	type_code->is_method = true;
	type_code->create_value = []() -> std::shared_ptr<vnx::Value> { return std::make_shared<SearchEngine_get_page_entries>(); };
	type_code->is_const = true;
	type_code->is_async = true;
	type_code->return_type = ::vnx::search::SearchEngine_get_page_entries_return::static_get_type_code();
	type_code->fields.resize(1);
	{
		vnx::TypeField& field = type_code->fields[0];
		field.is_extended = true;
		field.name = "page_ids";
		field.code = {12, 3};
	}
	type_code->build();
	return type_code;
}


} // namespace vnx
} // namespace search


namespace vnx {

void read(TypeInput& in, ::vnx::search::SearchEngine_get_page_entries& value, const TypeCode* type_code, const uint16_t* code) {
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
	if(type_code->is_matched) {
	}
	for(const vnx::TypeField* _field : type_code->ext_fields) {
		switch(_field->native_index) {
			case 0: vnx::read(in, value.page_ids, type_code, _field->code.data()); break;
			default: vnx::skip(in, type_code, _field->code.data());
		}
	}
}

void write(TypeOutput& out, const ::vnx::search::SearchEngine_get_page_entries& value, const TypeCode* type_code, const uint16_t* code) {
	if(code && code[0] == CODE_OBJECT) {
		vnx::write(out, value.to_object(), nullptr, code);
		return;
	}
	if(!type_code || (code && code[0] == CODE_ANY)) {
		type_code = vnx::search::vnx_native_type_code_SearchEngine_get_page_entries;
		out.write_type_code(type_code);
		vnx::write_class_header<::vnx::search::SearchEngine_get_page_entries>(out);
	}
	else if(code && code[0] == CODE_STRUCT) {
		type_code = type_code->depends[code[1]];
	}
	vnx::write(out, value.page_ids, type_code, type_code->fields[0].code.data());
}

void read(std::istream& in, ::vnx::search::SearchEngine_get_page_entries& value) {
	value.read(in);
}

void write(std::ostream& out, const ::vnx::search::SearchEngine_get_page_entries& value) {
	value.write(out);
}

void accept(Visitor& visitor, const ::vnx::search::SearchEngine_get_page_entries& value) {
	value.accept(visitor);
}

} // vnx
