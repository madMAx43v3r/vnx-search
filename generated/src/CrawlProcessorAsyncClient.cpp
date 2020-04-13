
// AUTO GENERATED by vnxcppcodegen

#include <vnx/search/package.hxx>
#include <vnx/search/CrawlProcessorAsyncClient.hxx>
#include <vnx/Input.h>
#include <vnx/Output.h>


namespace vnx {
namespace search {

CrawlProcessorAsyncClient::CrawlProcessorAsyncClient(const std::string& service_name)
	:	AsyncClient::AsyncClient(vnx::Hash64(service_name))
{
}

CrawlProcessorAsyncClient::CrawlProcessorAsyncClient(vnx::Hash64 service_addr)
	:	AsyncClient::AsyncClient(service_addr)
{
}

uint64_t CrawlProcessorAsyncClient::get_stats(const ::int32_t& limit, const std::function<void(::std::shared_ptr<const ::vnx::search::CrawlStats>)>& _callback) {
	std::shared_ptr<vnx::Binary> _argument_data = vnx::Binary::create();
	vnx::BinaryOutputStream _stream_out(_argument_data.get());
	vnx::TypeOutput _out(&_stream_out);
	const vnx::TypeCode* _type_code = vnx::search::vnx_native_type_code_CrawlProcessor_get_stats;
	{
		char* const _buf = _out.write(4);
		vnx::write_value(_buf + 0, limit);
	}
	_out.flush();
	_argument_data->type_code = _type_code;
	const uint64_t _request_id = vnx_request(_argument_data);
	vnx_queue_get_stats[_request_id] = _callback;
	vnx_num_pending++;
	return _request_id;
}

uint64_t CrawlProcessorAsyncClient::handle(const ::std::shared_ptr<const ::vnx::keyvalue::KeyValuePair>& sample, const std::function<void()>& _callback) {
	std::shared_ptr<vnx::Binary> _argument_data = vnx::Binary::create();
	vnx::BinaryOutputStream _stream_out(_argument_data.get());
	vnx::TypeOutput _out(&_stream_out);
	const vnx::TypeCode* _type_code = vnx::search::vnx_native_type_code_CrawlProcessor_handle_vnx_keyvalue_KeyValuePair;
	{
		vnx::write(_out, sample, _type_code, _type_code->fields[0].code.data());
	}
	_out.flush();
	_argument_data->type_code = _type_code;
	const uint64_t _request_id = vnx_request(_argument_data);
	vnx_queue_handle_vnx_keyvalue_KeyValuePair[_request_id] = _callback;
	vnx_num_pending++;
	return _request_id;
}

std::vector<uint64_t>CrawlProcessorAsyncClient::vnx_get_pending_ids() const {
	std::vector<uint64_t> _list;
	for(const auto& entry : vnx_queue_get_stats) {
		_list.push_back(entry.first);
	}
	for(const auto& entry : vnx_queue_handle_vnx_keyvalue_KeyValuePair) {
		_list.push_back(entry.first);
	}
	return _list;
}

void CrawlProcessorAsyncClient::vnx_purge_request(uint64_t _request_id) {
	vnx_num_pending -= vnx_queue_get_stats.erase(_request_id);
	vnx_num_pending -= vnx_queue_handle_vnx_keyvalue_KeyValuePair.erase(_request_id);
}

void CrawlProcessorAsyncClient::vnx_callback_switch(uint64_t _request_id, std::shared_ptr<const vnx::Binary> _data) {
	vnx::BinaryInputStream _stream_in(_data.get());
	vnx::TypeInput _in(&_stream_in);
	const vnx::TypeCode* _return_type = _data->type_code;
	
	if(_return_type->type_hash == vnx::Hash64(0xb8530c2e4e418c47ull)) {
		::std::shared_ptr<const ::vnx::search::CrawlStats> _ret_0;
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
		auto _iter = vnx_queue_get_stats.find(_request_id);
		if(_iter != vnx_queue_get_stats.end()) {
			const auto _callback = std::move(_iter->second);
			vnx_queue_get_stats.erase(_iter);
			vnx_num_pending--;
			if(_callback) {
				_callback(_ret_0);
			}
		}
	}
	else if(_return_type->type_hash == vnx::Hash64(0x5468ec5275268163ull)) {
		auto _iter = vnx_queue_handle_vnx_keyvalue_KeyValuePair.find(_request_id);
		if(_iter != vnx_queue_handle_vnx_keyvalue_KeyValuePair.end()) {
			const auto _callback = std::move(_iter->second);
			vnx_queue_handle_vnx_keyvalue_KeyValuePair.erase(_iter);
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
