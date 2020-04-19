
// AUTO GENERATED by vnxcppcodegen

#include <vnx/search/package.hxx>
#include <vnx/search/PageInfo.hxx>
#include <vnx/Input.h>
#include <vnx/Output.h>
#include <vnx/Visitor.h>
#include <vnx/Object.h>
#include <vnx/Struct.h>


namespace vnx {
namespace search {


const vnx::Hash64 PageInfo::VNX_TYPE_HASH(0x547cfd9b2bb19c80ull);
const vnx::Hash64 PageInfo::VNX_CODE_HASH(0xbd7cc77ddc38d3f5ull);

vnx::Hash64 PageInfo::get_type_hash() const {
	return VNX_TYPE_HASH;
}

const char* PageInfo::get_type_name() const {
	return "vnx.search.PageInfo";
}
const vnx::TypeCode* PageInfo::get_type_code() const {
	return vnx::search::vnx_native_type_code_PageInfo;
}

std::shared_ptr<PageInfo> PageInfo::create() {
	return std::make_shared<PageInfo>();
}

std::shared_ptr<vnx::Value> PageInfo::clone() const {
	return std::make_shared<PageInfo>(*this);
}

void PageInfo::read(vnx::TypeInput& _in, const vnx::TypeCode* _type_code, const uint16_t* _code) {
	vnx::read(_in, *this, _type_code, _code);
}

void PageInfo::write(vnx::TypeOutput& _out, const vnx::TypeCode* _type_code, const uint16_t* _code) const {
	vnx::write(_out, *this, _type_code, _code);
}

void PageInfo::accept(vnx::Visitor& _visitor) const {
	const vnx::TypeCode* _type_code = vnx::search::vnx_native_type_code_PageInfo;
	_visitor.type_begin(*_type_code);
	_visitor.type_field(_type_code->fields[0], 0); vnx::accept(_visitor, version);
	_visitor.type_field(_type_code->fields[1], 1); vnx::accept(_visitor, url_key);
	_visitor.type_field(_type_code->fields[2], 2); vnx::accept(_visitor, depth);
	_visitor.type_end(*_type_code);
}

void PageInfo::write(std::ostream& _out) const {
	_out << "{";
	_out << "\"version\": "; vnx::write(_out, version);
	_out << ", \"url_key\": "; vnx::write(_out, url_key);
	_out << ", \"depth\": "; vnx::write(_out, depth);
	_out << "}";
}

void PageInfo::read(std::istream& _in) {
	std::map<std::string, std::string> _object;
	vnx::read_object(_in, _object);
	for(const auto& _entry : _object) {
		if(_entry.first == "depth") {
			vnx::from_string(_entry.second, depth);
		} else if(_entry.first == "url_key") {
			vnx::from_string(_entry.second, url_key);
		} else if(_entry.first == "version") {
			vnx::from_string(_entry.second, version);
		}
	}
}

vnx::Object PageInfo::to_object() const {
	vnx::Object _object;
	_object["version"] = version;
	_object["url_key"] = url_key;
	_object["depth"] = depth;
	return _object;
}

void PageInfo::from_object(const vnx::Object& _object) {
	for(const auto& _entry : _object.field) {
		if(_entry.first == "depth") {
			_entry.second.to(depth);
		} else if(_entry.first == "url_key") {
			_entry.second.to(url_key);
		} else if(_entry.first == "version") {
			_entry.second.to(version);
		}
	}
}

/// \private
std::ostream& operator<<(std::ostream& _out, const PageInfo& _value) {
	_value.write(_out);
	return _out;
}

/// \private
std::istream& operator>>(std::istream& _in, PageInfo& _value) {
	_value.read(_in);
	return _in;
}

const vnx::TypeCode* PageInfo::static_get_type_code() {
	const vnx::TypeCode* type_code = vnx::get_type_code(vnx::Hash64(0x547cfd9b2bb19c80ull));
	if(!type_code) {
		type_code = vnx::register_type_code(static_create_type_code());
	}
	return type_code;
}

std::shared_ptr<vnx::TypeCode> PageInfo::static_create_type_code() {
	std::shared_ptr<vnx::TypeCode> type_code = std::make_shared<vnx::TypeCode>(true);
	type_code->name = "vnx.search.PageInfo";
	type_code->type_hash = vnx::Hash64(0x547cfd9b2bb19c80ull);
	type_code->code_hash = vnx::Hash64(0xbd7cc77ddc38d3f5ull);
	type_code->is_class = true;
	type_code->create_value = []() -> std::shared_ptr<vnx::Value> { return std::make_shared<PageInfo>(); };
	type_code->methods.resize(0);
	type_code->fields.resize(3);
	{
		vnx::TypeField& field = type_code->fields[0];
		field.name = "version";
		field.code = {4};
	}
	{
		vnx::TypeField& field = type_code->fields[1];
		field.is_extended = true;
		field.name = "url_key";
		field.code = {12, 5};
	}
	{
		vnx::TypeField& field = type_code->fields[2];
		field.name = "depth";
		field.value = vnx::to_string(-1);
		field.code = {7};
	}
	type_code->build();
	return type_code;
}


} // namespace vnx
} // namespace search


namespace vnx {

void read(TypeInput& in, ::vnx::search::PageInfo& value, const TypeCode* type_code, const uint16_t* code) {
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
				vnx::read_value(_buf + _field->offset, value.version, _field->code.data());
			}
		}
		{
			const vnx::TypeField* const _field = type_code->field_map[2];
			if(_field) {
				vnx::read_value(_buf + _field->offset, value.depth, _field->code.data());
			}
		}
	}
	for(const vnx::TypeField* _field : type_code->ext_fields) {
		switch(_field->native_index) {
			case 1: vnx::read(in, value.url_key, type_code, _field->code.data()); break;
			default: vnx::skip(in, type_code, _field->code.data());
		}
	}
}

void write(TypeOutput& out, const ::vnx::search::PageInfo& value, const TypeCode* type_code, const uint16_t* code) {
	if(!type_code || (code && code[0] == CODE_ANY)) {
		type_code = vnx::search::vnx_native_type_code_PageInfo;
		out.write_type_code(type_code);
		vnx::write_class_header<::vnx::search::PageInfo>(out);
	}
	if(code && code[0] == CODE_STRUCT) {
		type_code = type_code->depends[code[1]];
	}
	char* const _buf = out.write(12);
	vnx::write_value(_buf + 0, value.version);
	vnx::write_value(_buf + 8, value.depth);
	vnx::write(out, value.url_key, type_code, type_code->fields[1].code.data());
}

void read(std::istream& in, ::vnx::search::PageInfo& value) {
	value.read(in);
}

void write(std::ostream& out, const ::vnx::search::PageInfo& value) {
	value.write(out);
}

void accept(Visitor& visitor, const ::vnx::search::PageInfo& value) {
	value.accept(visitor);
}

} // vnx
