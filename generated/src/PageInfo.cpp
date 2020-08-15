
// AUTO GENERATED by vnxcppcodegen

#include <vnx/search/package.hxx>
#include <vnx/search/PageInfo.hxx>
#include <vnx/Value.h>

#include <vnx/vnx.h>


namespace vnx {
namespace search {


const vnx::Hash64 PageInfo::VNX_TYPE_HASH(0x547cfd9b2bb19c80ull);
const vnx::Hash64 PageInfo::VNX_CODE_HASH(0xb1245a17e40cf5a6ull);

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
	_visitor.type_field(_type_code->fields[0], 0); vnx::accept(_visitor, id);
	_visitor.type_field(_type_code->fields[1], 1); vnx::accept(_visitor, is_deleted);
	_visitor.type_field(_type_code->fields[2], 2); vnx::accept(_visitor, index_version);
	_visitor.type_field(_type_code->fields[3], 3); vnx::accept(_visitor, link_version);
	_visitor.type_field(_type_code->fields[4], 4); vnx::accept(_visitor, word_version);
	_visitor.type_field(_type_code->fields[5], 5); vnx::accept(_visitor, array_version);
	_visitor.type_field(_type_code->fields[6], 6); vnx::accept(_visitor, first_seen);
	_visitor.type_field(_type_code->fields[7], 7); vnx::accept(_visitor, last_modified);
	_visitor.type_field(_type_code->fields[8], 8); vnx::accept(_visitor, scheme);
	_visitor.type_field(_type_code->fields[9], 9); vnx::accept(_visitor, domain);
	_visitor.type_field(_type_code->fields[10], 10); vnx::accept(_visitor, title);
	_visitor.type_field(_type_code->fields[11], 11); vnx::accept(_visitor, words);
	_visitor.type_field(_type_code->fields[12], 12); vnx::accept(_visitor, links);
	_visitor.type_field(_type_code->fields[13], 13); vnx::accept(_visitor, reverse_links);
	_visitor.type_field(_type_code->fields[14], 14); vnx::accept(_visitor, reverse_domains);
	_visitor.type_end(*_type_code);
}

void PageInfo::write(std::ostream& _out) const {
	_out << "{\"__type\": \"vnx.search.PageInfo\"";
	_out << ", \"id\": "; vnx::write(_out, id);
	_out << ", \"is_deleted\": "; vnx::write(_out, is_deleted);
	_out << ", \"index_version\": "; vnx::write(_out, index_version);
	_out << ", \"link_version\": "; vnx::write(_out, link_version);
	_out << ", \"word_version\": "; vnx::write(_out, word_version);
	_out << ", \"array_version\": "; vnx::write(_out, array_version);
	_out << ", \"first_seen\": "; vnx::write(_out, first_seen);
	_out << ", \"last_modified\": "; vnx::write(_out, last_modified);
	_out << ", \"scheme\": "; vnx::write(_out, scheme);
	_out << ", \"domain\": "; vnx::write(_out, domain);
	_out << ", \"title\": "; vnx::write(_out, title);
	_out << ", \"words\": "; vnx::write(_out, words);
	_out << ", \"links\": "; vnx::write(_out, links);
	_out << ", \"reverse_links\": "; vnx::write(_out, reverse_links);
	_out << ", \"reverse_domains\": "; vnx::write(_out, reverse_domains);
	_out << "}";
}

void PageInfo::read(std::istream& _in) {
	std::map<std::string, std::string> _object;
	vnx::read_object(_in, _object);
	for(const auto& _entry : _object) {
		if(_entry.first == "array_version") {
			vnx::from_string(_entry.second, array_version);
		} else if(_entry.first == "domain") {
			vnx::from_string(_entry.second, domain);
		} else if(_entry.first == "first_seen") {
			vnx::from_string(_entry.second, first_seen);
		} else if(_entry.first == "id") {
			vnx::from_string(_entry.second, id);
		} else if(_entry.first == "index_version") {
			vnx::from_string(_entry.second, index_version);
		} else if(_entry.first == "is_deleted") {
			vnx::from_string(_entry.second, is_deleted);
		} else if(_entry.first == "last_modified") {
			vnx::from_string(_entry.second, last_modified);
		} else if(_entry.first == "link_version") {
			vnx::from_string(_entry.second, link_version);
		} else if(_entry.first == "links") {
			vnx::from_string(_entry.second, links);
		} else if(_entry.first == "reverse_domains") {
			vnx::from_string(_entry.second, reverse_domains);
		} else if(_entry.first == "reverse_links") {
			vnx::from_string(_entry.second, reverse_links);
		} else if(_entry.first == "scheme") {
			vnx::from_string(_entry.second, scheme);
		} else if(_entry.first == "title") {
			vnx::from_string(_entry.second, title);
		} else if(_entry.first == "word_version") {
			vnx::from_string(_entry.second, word_version);
		} else if(_entry.first == "words") {
			vnx::from_string(_entry.second, words);
		}
	}
}

vnx::Object PageInfo::to_object() const {
	vnx::Object _object;
	_object["__type"] = "vnx.search.PageInfo";
	_object["id"] = id;
	_object["is_deleted"] = is_deleted;
	_object["index_version"] = index_version;
	_object["link_version"] = link_version;
	_object["word_version"] = word_version;
	_object["array_version"] = array_version;
	_object["first_seen"] = first_seen;
	_object["last_modified"] = last_modified;
	_object["scheme"] = scheme;
	_object["domain"] = domain;
	_object["title"] = title;
	_object["words"] = words;
	_object["links"] = links;
	_object["reverse_links"] = reverse_links;
	_object["reverse_domains"] = reverse_domains;
	return _object;
}

void PageInfo::from_object(const vnx::Object& _object) {
	for(const auto& _entry : _object.field) {
		if(_entry.first == "array_version") {
			_entry.second.to(array_version);
		} else if(_entry.first == "domain") {
			_entry.second.to(domain);
		} else if(_entry.first == "first_seen") {
			_entry.second.to(first_seen);
		} else if(_entry.first == "id") {
			_entry.second.to(id);
		} else if(_entry.first == "index_version") {
			_entry.second.to(index_version);
		} else if(_entry.first == "is_deleted") {
			_entry.second.to(is_deleted);
		} else if(_entry.first == "last_modified") {
			_entry.second.to(last_modified);
		} else if(_entry.first == "link_version") {
			_entry.second.to(link_version);
		} else if(_entry.first == "links") {
			_entry.second.to(links);
		} else if(_entry.first == "reverse_domains") {
			_entry.second.to(reverse_domains);
		} else if(_entry.first == "reverse_links") {
			_entry.second.to(reverse_links);
		} else if(_entry.first == "scheme") {
			_entry.second.to(scheme);
		} else if(_entry.first == "title") {
			_entry.second.to(title);
		} else if(_entry.first == "word_version") {
			_entry.second.to(word_version);
		} else if(_entry.first == "words") {
			_entry.second.to(words);
		}
	}
}

vnx::Variant PageInfo::get_field(const std::string& _name) const {
	if(_name == "id") {
		return vnx::Variant(id);
	}
	if(_name == "is_deleted") {
		return vnx::Variant(is_deleted);
	}
	if(_name == "index_version") {
		return vnx::Variant(index_version);
	}
	if(_name == "link_version") {
		return vnx::Variant(link_version);
	}
	if(_name == "word_version") {
		return vnx::Variant(word_version);
	}
	if(_name == "array_version") {
		return vnx::Variant(array_version);
	}
	if(_name == "first_seen") {
		return vnx::Variant(first_seen);
	}
	if(_name == "last_modified") {
		return vnx::Variant(last_modified);
	}
	if(_name == "scheme") {
		return vnx::Variant(scheme);
	}
	if(_name == "domain") {
		return vnx::Variant(domain);
	}
	if(_name == "title") {
		return vnx::Variant(title);
	}
	if(_name == "words") {
		return vnx::Variant(words);
	}
	if(_name == "links") {
		return vnx::Variant(links);
	}
	if(_name == "reverse_links") {
		return vnx::Variant(reverse_links);
	}
	if(_name == "reverse_domains") {
		return vnx::Variant(reverse_domains);
	}
	return vnx::Variant();
}

void PageInfo::set_field(const std::string& _name, const vnx::Variant& _value) {
	if(_name == "id") {
		_value.to(id);
	} else if(_name == "is_deleted") {
		_value.to(is_deleted);
	} else if(_name == "index_version") {
		_value.to(index_version);
	} else if(_name == "link_version") {
		_value.to(link_version);
	} else if(_name == "word_version") {
		_value.to(word_version);
	} else if(_name == "array_version") {
		_value.to(array_version);
	} else if(_name == "first_seen") {
		_value.to(first_seen);
	} else if(_name == "last_modified") {
		_value.to(last_modified);
	} else if(_name == "scheme") {
		_value.to(scheme);
	} else if(_name == "domain") {
		_value.to(domain);
	} else if(_name == "title") {
		_value.to(title);
	} else if(_name == "words") {
		_value.to(words);
	} else if(_name == "links") {
		_value.to(links);
	} else if(_name == "reverse_links") {
		_value.to(reverse_links);
	} else if(_name == "reverse_domains") {
		_value.to(reverse_domains);
	} else {
		throw std::logic_error("no such field: '" + _name + "'");
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
	const vnx::TypeCode* type_code = vnx::get_type_code(VNX_TYPE_HASH);
	if(!type_code) {
		type_code = vnx::register_type_code(static_create_type_code());
	}
	return type_code;
}

std::shared_ptr<vnx::TypeCode> PageInfo::static_create_type_code() {
	std::shared_ptr<vnx::TypeCode> type_code = std::make_shared<vnx::TypeCode>();
	type_code->name = "vnx.search.PageInfo";
	type_code->type_hash = vnx::Hash64(0x547cfd9b2bb19c80ull);
	type_code->code_hash = vnx::Hash64(0xb1245a17e40cf5a6ull);
	type_code->is_native = true;
	type_code->is_class = true;
	type_code->create_value = []() -> std::shared_ptr<vnx::Value> { return std::make_shared<PageInfo>(); };
	type_code->fields.resize(15);
	{
		vnx::TypeField& field = type_code->fields[0];
		field.name = "id";
		field.code = {3};
	}
	{
		vnx::TypeField& field = type_code->fields[1];
		field.name = "is_deleted";
		field.code = {31};
	}
	{
		vnx::TypeField& field = type_code->fields[2];
		field.name = "index_version";
		field.code = {4};
	}
	{
		vnx::TypeField& field = type_code->fields[3];
		field.name = "link_version";
		field.code = {4};
	}
	{
		vnx::TypeField& field = type_code->fields[4];
		field.name = "word_version";
		field.code = {4};
	}
	{
		vnx::TypeField& field = type_code->fields[5];
		field.name = "array_version";
		field.code = {4};
	}
	{
		vnx::TypeField& field = type_code->fields[6];
		field.name = "first_seen";
		field.code = {8};
	}
	{
		vnx::TypeField& field = type_code->fields[7];
		field.name = "last_modified";
		field.code = {8};
	}
	{
		vnx::TypeField& field = type_code->fields[8];
		field.is_extended = true;
		field.name = "scheme";
		field.code = {32};
	}
	{
		vnx::TypeField& field = type_code->fields[9];
		field.is_extended = true;
		field.name = "domain";
		field.code = {32};
	}
	{
		vnx::TypeField& field = type_code->fields[10];
		field.is_extended = true;
		field.name = "title";
		field.code = {32};
	}
	{
		vnx::TypeField& field = type_code->fields[11];
		field.is_extended = true;
		field.name = "words";
		field.code = {12, 3};
	}
	{
		vnx::TypeField& field = type_code->fields[12];
		field.is_extended = true;
		field.name = "links";
		field.code = {12, 32};
	}
	{
		vnx::TypeField& field = type_code->fields[13];
		field.is_extended = true;
		field.name = "reverse_links";
		field.code = {12, 32};
	}
	{
		vnx::TypeField& field = type_code->fields[14];
		field.is_extended = true;
		field.name = "reverse_domains";
		field.code = {13, 3, 32, 7};
	}
	type_code->build();
	return type_code;
}


} // namespace vnx
} // namespace search


namespace vnx {

void read(TypeInput& in, ::vnx::search::PageInfo& value, const TypeCode* type_code, const uint16_t* code) {
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
				vnx::read_value(_buf + _field->offset, value.id, _field->code.data());
			}
		}
		{
			const vnx::TypeField* const _field = type_code->field_map[1];
			if(_field) {
				vnx::read_value(_buf + _field->offset, value.is_deleted, _field->code.data());
			}
		}
		{
			const vnx::TypeField* const _field = type_code->field_map[2];
			if(_field) {
				vnx::read_value(_buf + _field->offset, value.index_version, _field->code.data());
			}
		}
		{
			const vnx::TypeField* const _field = type_code->field_map[3];
			if(_field) {
				vnx::read_value(_buf + _field->offset, value.link_version, _field->code.data());
			}
		}
		{
			const vnx::TypeField* const _field = type_code->field_map[4];
			if(_field) {
				vnx::read_value(_buf + _field->offset, value.word_version, _field->code.data());
			}
		}
		{
			const vnx::TypeField* const _field = type_code->field_map[5];
			if(_field) {
				vnx::read_value(_buf + _field->offset, value.array_version, _field->code.data());
			}
		}
		{
			const vnx::TypeField* const _field = type_code->field_map[6];
			if(_field) {
				vnx::read_value(_buf + _field->offset, value.first_seen, _field->code.data());
			}
		}
		{
			const vnx::TypeField* const _field = type_code->field_map[7];
			if(_field) {
				vnx::read_value(_buf + _field->offset, value.last_modified, _field->code.data());
			}
		}
	}
	for(const vnx::TypeField* _field : type_code->ext_fields) {
		switch(_field->native_index) {
			case 8: vnx::read(in, value.scheme, type_code, _field->code.data()); break;
			case 9: vnx::read(in, value.domain, type_code, _field->code.data()); break;
			case 10: vnx::read(in, value.title, type_code, _field->code.data()); break;
			case 11: vnx::read(in, value.words, type_code, _field->code.data()); break;
			case 12: vnx::read(in, value.links, type_code, _field->code.data()); break;
			case 13: vnx::read(in, value.reverse_links, type_code, _field->code.data()); break;
			case 14: vnx::read(in, value.reverse_domains, type_code, _field->code.data()); break;
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
	char* const _buf = out.write(53);
	vnx::write_value(_buf + 0, value.id);
	vnx::write_value(_buf + 4, value.is_deleted);
	vnx::write_value(_buf + 5, value.index_version);
	vnx::write_value(_buf + 13, value.link_version);
	vnx::write_value(_buf + 21, value.word_version);
	vnx::write_value(_buf + 29, value.array_version);
	vnx::write_value(_buf + 37, value.first_seen);
	vnx::write_value(_buf + 45, value.last_modified);
	vnx::write(out, value.scheme, type_code, type_code->fields[8].code.data());
	vnx::write(out, value.domain, type_code, type_code->fields[9].code.data());
	vnx::write(out, value.title, type_code, type_code->fields[10].code.data());
	vnx::write(out, value.words, type_code, type_code->fields[11].code.data());
	vnx::write(out, value.links, type_code, type_code->fields[12].code.data());
	vnx::write(out, value.reverse_links, type_code, type_code->fields[13].code.data());
	vnx::write(out, value.reverse_domains, type_code, type_code->fields[14].code.data());
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
