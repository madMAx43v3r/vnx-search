
// AUTO GENERATED by vnxcppcodegen

#include <vnx/search/package.hxx>
#include <vnx/search/PageIndex.hxx>
#include <vnx/Value.h>
#include <vnx/search/image_link_t.hxx>
#include <vnx/search/page_link_t.hxx>

#include <vnx/vnx.h>


namespace vnx {
namespace search {


const vnx::Hash64 PageIndex::VNX_TYPE_HASH(0x4c9c9cf43a382f0ull);
const vnx::Hash64 PageIndex::VNX_CODE_HASH(0xcfc6aa33297ad488ull);

vnx::Hash64 PageIndex::get_type_hash() const {
	return VNX_TYPE_HASH;
}

std::string PageIndex::get_type_name() const {
	return "vnx.search.PageIndex";
}

const vnx::TypeCode* PageIndex::get_type_code() const {
	return vnx::search::vnx_native_type_code_PageIndex;
}

std::shared_ptr<PageIndex> PageIndex::create() {
	return std::make_shared<PageIndex>();
}

std::shared_ptr<vnx::Value> PageIndex::clone() const {
	return std::make_shared<PageIndex>(*this);
}

void PageIndex::read(vnx::TypeInput& _in, const vnx::TypeCode* _type_code, const uint16_t* _code) {
	vnx::read(_in, *this, _type_code, _code);
}

void PageIndex::write(vnx::TypeOutput& _out, const vnx::TypeCode* _type_code, const uint16_t* _code) const {
	vnx::write(_out, *this, _type_code, _code);
}

void PageIndex::accept(vnx::Visitor& _visitor) const {
	const vnx::TypeCode* _type_code = vnx::search::vnx_native_type_code_PageIndex;
	_visitor.type_begin(*_type_code);
	_visitor.type_field(_type_code->fields[0], 0); vnx::accept(_visitor, version);
	_visitor.type_field(_type_code->fields[1], 1); vnx::accept(_visitor, word_count);
	_visitor.type_field(_type_code->fields[2], 2); vnx::accept(_visitor, last_modified);
	_visitor.type_field(_type_code->fields[3], 3); vnx::accept(_visitor, title);
	_visitor.type_field(_type_code->fields[4], 4); vnx::accept(_visitor, links);
	_visitor.type_field(_type_code->fields[5], 5); vnx::accept(_visitor, images);
	_visitor.type_field(_type_code->fields[6], 6); vnx::accept(_visitor, words);
	_visitor.type_end(*_type_code);
}

void PageIndex::write(std::ostream& _out) const {
	_out << "{\"__type\": \"vnx.search.PageIndex\"";
	_out << ", \"version\": "; vnx::write(_out, version);
	_out << ", \"word_count\": "; vnx::write(_out, word_count);
	_out << ", \"last_modified\": "; vnx::write(_out, last_modified);
	_out << ", \"title\": "; vnx::write(_out, title);
	_out << ", \"links\": "; vnx::write(_out, links);
	_out << ", \"images\": "; vnx::write(_out, images);
	_out << ", \"words\": "; vnx::write(_out, words);
	_out << "}";
}

void PageIndex::read(std::istream& _in) {
	if(auto _json = vnx::read_json(_in)) {
		from_object(_json->to_object());
	}
}

vnx::Object PageIndex::to_object() const {
	vnx::Object _object;
	_object["__type"] = "vnx.search.PageIndex";
	_object["version"] = version;
	_object["word_count"] = word_count;
	_object["last_modified"] = last_modified;
	_object["title"] = title;
	_object["links"] = links;
	_object["images"] = images;
	_object["words"] = words;
	return _object;
}

void PageIndex::from_object(const vnx::Object& _object) {
	for(const auto& _entry : _object.field) {
		if(_entry.first == "images") {
			_entry.second.to(images);
		} else if(_entry.first == "last_modified") {
			_entry.second.to(last_modified);
		} else if(_entry.first == "links") {
			_entry.second.to(links);
		} else if(_entry.first == "title") {
			_entry.second.to(title);
		} else if(_entry.first == "version") {
			_entry.second.to(version);
		} else if(_entry.first == "word_count") {
			_entry.second.to(word_count);
		} else if(_entry.first == "words") {
			_entry.second.to(words);
		}
	}
}

vnx::Variant PageIndex::get_field(const std::string& _name) const {
	if(_name == "version") {
		return vnx::Variant(version);
	}
	if(_name == "word_count") {
		return vnx::Variant(word_count);
	}
	if(_name == "last_modified") {
		return vnx::Variant(last_modified);
	}
	if(_name == "title") {
		return vnx::Variant(title);
	}
	if(_name == "links") {
		return vnx::Variant(links);
	}
	if(_name == "images") {
		return vnx::Variant(images);
	}
	if(_name == "words") {
		return vnx::Variant(words);
	}
	return vnx::Variant();
}

void PageIndex::set_field(const std::string& _name, const vnx::Variant& _value) {
	if(_name == "version") {
		_value.to(version);
	} else if(_name == "word_count") {
		_value.to(word_count);
	} else if(_name == "last_modified") {
		_value.to(last_modified);
	} else if(_name == "title") {
		_value.to(title);
	} else if(_name == "links") {
		_value.to(links);
	} else if(_name == "images") {
		_value.to(images);
	} else if(_name == "words") {
		_value.to(words);
	} else {
		throw std::logic_error("no such field: '" + _name + "'");
	}
}

/// \private
std::ostream& operator<<(std::ostream& _out, const PageIndex& _value) {
	_value.write(_out);
	return _out;
}

/// \private
std::istream& operator>>(std::istream& _in, PageIndex& _value) {
	_value.read(_in);
	return _in;
}

const vnx::TypeCode* PageIndex::static_get_type_code() {
	const vnx::TypeCode* type_code = vnx::get_type_code(VNX_TYPE_HASH);
	if(!type_code) {
		type_code = vnx::register_type_code(static_create_type_code());
	}
	return type_code;
}

std::shared_ptr<vnx::TypeCode> PageIndex::static_create_type_code() {
	std::shared_ptr<vnx::TypeCode> type_code = std::make_shared<vnx::TypeCode>();
	type_code->name = "vnx.search.PageIndex";
	type_code->type_hash = vnx::Hash64(0x4c9c9cf43a382f0ull);
	type_code->code_hash = vnx::Hash64(0xcfc6aa33297ad488ull);
	type_code->is_native = true;
	type_code->is_class = true;
	type_code->create_value = []() -> std::shared_ptr<vnx::Value> { return std::make_shared<PageIndex>(); };
	type_code->depends.resize(2);
	type_code->depends[0] = ::vnx::search::page_link_t::static_get_type_code();
	type_code->depends[1] = ::vnx::search::image_link_t::static_get_type_code();
	type_code->fields.resize(7);
	{
		vnx::TypeField& field = type_code->fields[0];
		field.name = "version";
		field.code = {2};
	}
	{
		vnx::TypeField& field = type_code->fields[1];
		field.name = "word_count";
		field.code = {3};
	}
	{
		vnx::TypeField& field = type_code->fields[2];
		field.name = "last_modified";
		field.code = {8};
	}
	{
		vnx::TypeField& field = type_code->fields[3];
		field.is_extended = true;
		field.name = "title";
		field.code = {32};
	}
	{
		vnx::TypeField& field = type_code->fields[4];
		field.is_extended = true;
		field.name = "links";
		field.code = {12, 19, 0};
	}
	{
		vnx::TypeField& field = type_code->fields[5];
		field.is_extended = true;
		field.name = "images";
		field.code = {12, 19, 1};
	}
	{
		vnx::TypeField& field = type_code->fields[6];
		field.is_extended = true;
		field.name = "words";
		field.code = {12, 23, 2, 4, 5, 32, 2};
	}
	type_code->build();
	return type_code;
}


} // namespace vnx
} // namespace search


namespace vnx {

void read(TypeInput& in, ::vnx::search::PageIndex& value, const TypeCode* type_code, const uint16_t* code) {
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
			const vnx::TypeField* const _field = type_code->field_map[0];
			if(_field) {
				vnx::read_value(_buf + _field->offset, value.version, _field->code.data());
			}
		}
		{
			const vnx::TypeField* const _field = type_code->field_map[1];
			if(_field) {
				vnx::read_value(_buf + _field->offset, value.word_count, _field->code.data());
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
			case 3: vnx::read(in, value.title, type_code, _field->code.data()); break;
			case 4: vnx::read(in, value.links, type_code, _field->code.data()); break;
			case 5: vnx::read(in, value.images, type_code, _field->code.data()); break;
			case 6: vnx::read(in, value.words, type_code, _field->code.data()); break;
			default: vnx::skip(in, type_code, _field->code.data());
		}
	}
}

void write(TypeOutput& out, const ::vnx::search::PageIndex& value, const TypeCode* type_code, const uint16_t* code) {
	if(code && code[0] == CODE_OBJECT) {
		vnx::write(out, value.to_object(), nullptr, code);
		return;
	}
	if(!type_code || (code && code[0] == CODE_ANY)) {
		type_code = vnx::search::vnx_native_type_code_PageIndex;
		out.write_type_code(type_code);
		vnx::write_class_header<::vnx::search::PageIndex>(out);
	}
	else if(code && code[0] == CODE_STRUCT) {
		type_code = type_code->depends[code[1]];
	}
	char* const _buf = out.write(14);
	vnx::write_value(_buf + 0, value.version);
	vnx::write_value(_buf + 2, value.word_count);
	vnx::write_value(_buf + 6, value.last_modified);
	vnx::write(out, value.title, type_code, type_code->fields[3].code.data());
	vnx::write(out, value.links, type_code, type_code->fields[4].code.data());
	vnx::write(out, value.images, type_code, type_code->fields[5].code.data());
	vnx::write(out, value.words, type_code, type_code->fields[6].code.data());
}

void read(std::istream& in, ::vnx::search::PageIndex& value) {
	value.read(in);
}

void write(std::ostream& out, const ::vnx::search::PageIndex& value) {
	value.write(out);
}

void accept(Visitor& visitor, const ::vnx::search::PageIndex& value) {
	value.accept(visitor);
}

} // vnx
