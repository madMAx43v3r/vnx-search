
// AUTO GENERATED by vnxcppcodegen

#include <vnx/search/package.hxx>
#include <vnx/search/CrawlFrontendAsyncClient.hxx>
#include <vnx/Input.h>
#include <vnx/Output.h>


namespace vnx {
namespace search {

CrawlFrontendAsyncClient::CrawlFrontendAsyncClient(const std::string& service_name)
	:	AsyncClient::AsyncClient(vnx::Hash64(service_name))
{
}

CrawlFrontendAsyncClient::CrawlFrontendAsyncClient(vnx::Hash64 service_addr)
	:	AsyncClient::AsyncClient(service_addr)
{
}

uint64_t CrawlFrontendAsyncClient::fetch(const ::std::string& url, const ::std::string& profile, const std::function<void(::std::shared_ptr<const ::vnx::search::UrlIndex>)>& _callback) {
	std::shared_ptr<vnx::Binary> _argument_data = vnx::Binary::create();
	vnx::BinaryOutputStream _stream_out(_argument_data.get());
	vnx::TypeOutput _out(&_stream_out);
	const vnx::TypeCode* _type_code = vnx::search::vnx_native_type_code_CrawlFrontend_fetch;
	{
		vnx::write(_out, url, _type_code, _type_code->fields[0].code.data());
		vnx::write(_out, profile, _type_code, _type_code->fields[1].code.data());
	}
	_out.flush();
	_argument_data->type_code = _type_code;
	const uint64_t _request_id = vnx_request(_argument_data);
	vnx_queue_fetch[_request_id] = _callback;
	vnx_num_pending++;
	return _request_id;
}

uint64_t CrawlFrontendAsyncClient::handle(const ::std::shared_ptr<const ::vnx::search::HttpResponse>& sample, const std::function<void()>& _callback) {
	std::shared_ptr<vnx::Binary> _argument_data = vnx::Binary::create();
	vnx::BinaryOutputStream _stream_out(_argument_data.get());
	vnx::TypeOutput _out(&_stream_out);
	const vnx::TypeCode* _type_code = vnx::search::vnx_native_type_code_CrawlFrontend_handle_vnx_search_HttpResponse;
	{
		vnx::write(_out, sample, _type_code, _type_code->fields[0].code.data());
	}
	_out.flush();
	_argument_data->type_code = _type_code;
	const uint64_t _request_id = vnx_request(_argument_data);
	vnx_queue_handle_vnx_search_HttpResponse[_request_id] = _callback;
	vnx_num_pending++;
	return _request_id;
}

uint64_t CrawlFrontendAsyncClient::register_parser(const ::vnx::Hash64& address, const ::std::vector<::std::string>& mime_types, const ::int32_t& num_threads, const std::function<void()>& _callback) {
	std::shared_ptr<vnx::Binary> _argument_data = vnx::Binary::create();
	vnx::BinaryOutputStream _stream_out(_argument_data.get());
	vnx::TypeOutput _out(&_stream_out);
	const vnx::TypeCode* _type_code = vnx::search::vnx_native_type_code_CrawlFrontend_register_parser;
	{
		char* const _buf = _out.write(4);
		vnx::write_value(_buf + 0, num_threads);
		vnx::write(_out, address, _type_code, _type_code->fields[0].code.data());
		vnx::write(_out, mime_types, _type_code, _type_code->fields[1].code.data());
	}
	_out.flush();
	_argument_data->type_code = _type_code;
	const uint64_t _request_id = vnx_request(_argument_data);
	vnx_queue_register_parser[_request_id] = _callback;
	vnx_num_pending++;
	return _request_id;
}

std::vector<uint64_t>CrawlFrontendAsyncClient::vnx_get_pending_ids() const {
	std::vector<uint64_t> _list;
	for(const auto& entry : vnx_queue_fetch) {
		_list.push_back(entry.first);
	}
	for(const auto& entry : vnx_queue_handle_vnx_search_HttpResponse) {
		_list.push_back(entry.first);
	}
	for(const auto& entry : vnx_queue_register_parser) {
		_list.push_back(entry.first);
	}
	return _list;
}

void CrawlFrontendAsyncClient::vnx_purge_request(uint64_t _request_id) {
	vnx_num_pending -= vnx_queue_fetch.erase(_request_id);
	vnx_num_pending -= vnx_queue_handle_vnx_search_HttpResponse.erase(_request_id);
	vnx_num_pending -= vnx_queue_register_parser.erase(_request_id);
}

void CrawlFrontendAsyncClient::vnx_callback_switch(uint64_t _request_id, std::shared_ptr<const vnx::Binary> _data) {
	vnx::BinaryInputStream _stream_in(_data.get());
	vnx::TypeInput _in(&_stream_in);
	const vnx::TypeCode* _return_type = _data->type_code;
	
	if(_return_type->type_hash == vnx::Hash64(0xce1d8d53217a282eull)) {
		::std::shared_ptr<const ::vnx::search::UrlIndex> _ret_0;
		{
			const char* const _buf = _in.read(_return_type->total_field_size);
			if(_return_type->is_matched) {
			}
			for(const vnx::TypeField* _field : _return_type->ext_fields) {
				switch(_field->native_index) {
					case 0: vnx::read(_in, _ret_0, _return_type, _field->code.data()); break;
					default: vnx::skip(_in, _return_type, _field->code.data());
				}
			}
		}
		auto _iter = vnx_queue_fetch.find(_request_id);
		if(_iter != vnx_queue_fetch.end()) {
			const auto _callback = std::move(_iter->second);
			vnx_queue_fetch.erase(_iter);
			vnx_num_pending--;
			if(_callback) {
				_callback(_ret_0);
			}
		}
	}
	else if(_return_type->type_hash == vnx::Hash64(0x1d67c2d26b1211d3ull)) {
		auto _iter = vnx_queue_handle_vnx_search_HttpResponse.find(_request_id);
		if(_iter != vnx_queue_handle_vnx_search_HttpResponse.end()) {
			const auto _callback = std::move(_iter->second);
			vnx_queue_handle_vnx_search_HttpResponse.erase(_iter);
			vnx_num_pending--;
			if(_callback) {
				_callback();
			}
		}
	}
	else if(_return_type->type_hash == vnx::Hash64(0x8ced9de5e282cceaull)) {
		auto _iter = vnx_queue_register_parser.find(_request_id);
		if(_iter != vnx_queue_register_parser.end()) {
			const auto _callback = std::move(_iter->second);
			vnx_queue_register_parser.erase(_iter);
			vnx_num_pending--;
			if(_callback) {
				_callback();
			}
		}
	}
	else {
		throw std::runtime_error("unknown return value");
	}
}


} // namespace vnx
} // namespace search
