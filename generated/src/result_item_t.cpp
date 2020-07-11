
// AUTO GENERATED by vnxcppcodegen

#include <vnx/search/package.hxx>
#include <vnx/search/result_item_t.hxx>
#include <vnx/Value.h>

#include <vnx/vnx.h>


namespace vnx {
namespace search {


const vnx::Hash64 result_item_t::VNX_TYPE_HASH(0x25b1d557d37a1bdfull);
const vnx::Hash64 result_item_t::VNX_CODE_HASH(0x602dbdbdcfdc7f59ull);

vnx::Hash64 result_item_t::get_type_hash() const {
	return VNX_TYPE_HASH;
}

const char* result_item_t::get_type_name() const {
	return "vnx.search.result_item_t";
}
const vnx::TypeCode* result_item_t::get_type_code() const {
	return vnx::search::vnx_native_type_code_result_item_t;
}

std::shared_ptr<result_item_t> result_item_t::create() {
	return std::make_shared<result_item_t>();
}

std::shared_ptr<vnx::Value> result_item_t::clone() const {
	return std::make_shared<result_item_t>(*this);
}

void result_item_t::read(vnx::TypeInput& _in, const vnx::TypeCode* _type_code, const uint16_t* _code) {
	vnx::read(_in, *this, _type_code, _code);
}

void result_item_t::write(vnx::TypeOutput& _out, const vnx::TypeCode* _type_code, const uint16_t* _code) const {
	vnx::write(_out, *this, _type_code, _code);
}

void result_item_t::accept(vnx::Visitor& _visitor) const {
	const vnx::TypeCode* _type_code = vnx::search::vnx_native_type_code_result_item_t;
	_visitor.type_begin(*_type_code);
	_visitor.type_field(_type_code->fields[0], 0); vnx::accept(_visitor, page_id);
	_visitor.type_field(_type_code->fields[1], 1); vnx::accept(_visitor, score);
	_visitor.type_field(_type_code->fields[2], 2); vnx::accept(_visitor, last_modified);
	_visitor.type_field(_type_code->fields[3], 3); vnx::accept(_visitor, url);
	_visitor.type_field(_type_code->fields[4], 4); vnx::accept(_visitor, title);
	_visitor.type_end(*_type_code);
}

void result_item_t::write(std::ostream& _out) const {
	_out << "{\"__type\": \"vnx.search.result_item_t\"";
	_out << ", \"page_id\": "; vnx::write(_out, page_id);
	_out << ", \"score\": "; vnx::write(_out, score);
	_out << ", \"last_modified\": "; vnx::write(_out, last_modified);
	_out << ", \"url\": "; vnx::write(_out, url);
	_out << ", \"title\": "; vnx::write(_out, title);
	_out << "}";
}

void result_item_t::read(std::istream& _in) {
	std::map<std::string, std::string> _object;
	vnx::read_object(_in, _object);
	for(const auto& _entry : _object) {
		if(_entry.first == "last_modified") {
			vnx::from_string(_entry.second, last_modified);
		} else if(_entry.first == "page_id") {
			vnx::from_string(_entry.second, page_id);
		} else if(_entry.first == "score") {
			vnx::from_string(_entry.second, score);
		} else if(_entry.first == "title") {
			vnx::from_string(_entry.second, title);
		} else if(_entry.first == "url") {
			vnx::from_string(_entry.second, url);
		}
	}
}

vnx::Object result_item_t::to_object() const {
	vnx::Object _object;
	_object["__type"] = "vnx.search.result_item_t";
	_object["page_id"] = page_id;
	_object["score"] = score;
	_object["last_modified"] = last_modified;
	_object["url"] = url;
	_object["title"] = title;
	return _object;
}

void result_item_t::from_object(const vnx::Object& _object) {
	for(const auto& _entry : _object.field) {
		if(_entry.first == "last_modified") {
			_entry.second.to(last_modified);
		} else if(_entry.first == "page_id") {
			_entry.second.to(page_id);
		} else if(_entry.first == "score") {
			_entry.second.to(score);
		} else if(_entry.first == "title") {
			_entry.second.to(title);
		} else if(_entry.first == "url") {
			_entry.second.to(url);
		}
	}
}

/// \private
std::ostream& operator<<(std::ostream& _out, const result_item_t& _value) {
	_value.write(_out);
	return _out;
}

/// \private
std::istream& operator>>(std::istream& _in, result_item_t& _value) {
	_value.read(_in);
	return _in;
}

const vnx::TypeCode* result_item_t::static_get_type_code() {
	const vnx::TypeCode* type_code = vnx::get_type_code(VNX_TYPE_HASH);
	if(!type_code) {
		type_code = vnx::register_type_code(static_create_type_code());
	}
	return type_code;
}

std::shared_ptr<vnx::TypeCode> result_item_t::static_create_type_code() {
	std::shared_ptr<vnx::TypeCode> type_code = std::make_shared<vnx::TypeCode>();
	type_code->name = "vnx.search.result_item_t";
	type_code->type_hash = vnx::Hash64(0x25b1d557d37a1bdfull);
	type_code->code_hash = vnx::Hash64(0x602dbdbdcfdc7f59ull);
	type_code->is_native = true;
	type_code->is_class = true;
	type_code->create_value = []() -> std::shared_ptr<vnx::Value> { return std::make_shared<result_item_t>(); };
	type_code->fields.resize(5);
	{
		vnx::TypeField& field = type_code->fields[0];
		field.name = "page_id";
		field.code = {3};
	}
	{
		vnx::TypeField& field = type_code->fields[1];
		field.name = "score";
		field.code = {8};
	}
	{
		vnx::TypeField& field = type_code->fields[2];
		field.name = "last_modified";
		field.code = {8};
	}
	{
		vnx::TypeField& field = type_code->fields[3];
		field.is_extended = true;
		field.name = "url";
		field.code = {12, 5};
	}
	{
		vnx::TypeField& field = type_code->fields[4];
		field.is_extended = true;
		field.name = "title";
		field.code = {12, 5};
	}
	type_code->build();
	return type_code;
}


} // namespace vnx
} // namespace search


namespace vnx {

void read(TypeInput& in, ::vnx::search::result_item_t& value, const TypeCode* type_code, const uint16_t* code) {
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
				vnx::read_value(_buf + _field->offset, value.page_id, _field->code.data());
			}
		}
		{
			const vnx::TypeField* const _field = type_code->field_map[1];
			if(_field) {
				vnx::read_value(_buf + _field->offset, value.score, _field->code.data());
			}
		}
		{
			const vnx::TypeField* const _field = type_code->field_map[2];
			if(_field) {
				vnx::read_value(_buf + _field->offset, value.last_modified, _field->code.data());
			}
		}
	}
	for(const vnx::TypeField* _field : type_code->ext_fields) {
		switch(_field->native_index) {
			case 3: vnx::read(in, value.url, type_code, _field->code.data()); break;
			case 4: vnx::read(in, value.title, type_code, _field->code.data()); break;
			default: vnx::skip(in, type_code, _field->code.data());
		}
	}
}

void write(TypeOutput& out, const ::vnx::search::result_item_t& value, const TypeCode* type_code, const uint16_t* code) {
	if(!type_code || (code && code[0] == CODE_ANY)) {
		type_code = vnx::search::vnx_native_type_code_result_item_t;
		out.write_type_code(type_code);
		vnx::write_class_header<::vnx::search::result_item_t>(out);
	}
	if(code && code[0] == CODE_STRUCT) {
		type_code = type_code->depends[code[1]];
	}
	char* const _buf = out.write(20);
	vnx::write_value(_buf + 0, value.page_id);
	vnx::write_value(_buf + 4, value.score);
	vnx::write_value(_buf + 12, value.last_modified);
	vnx::write(out, value.url, type_code, type_code->fields[3].code.data());
	vnx::write(out, value.title, type_code, type_code->fields[4].code.data());
}

void read(std::istream& in, ::vnx::search::result_item_t& value) {
	value.read(in);
}

void write(std::ostream& out, const ::vnx::search::result_item_t& value) {
	value.write(out);
}

void accept(Visitor& visitor, const ::vnx::search::result_item_t& value) {
	value.accept(visitor);
}

} // vnx
