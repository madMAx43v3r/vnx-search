/*
 * CrawlFrontend.cpp
 *
 *  Created on: Mar 30, 2020
 *      Author: mad
 */

#include <vnx/search/CrawlFrontend.h>
#include <vnx/search/Util.h>
#include <vnx/search/HttpResponse.hxx>
#include <vnx/search/CrawlProcessorClient.hxx>

#include <curl/curl.h>
#include <url.h>

#include <sstream>
#include <locale>
#include <iomanip>
#include <time.h>
#include <signal.h>
#include <unistd.h>


namespace vnx {
namespace search {

static
bool iequals(const std::string& a, const std::string& b)
{
    const size_t sz = a.size();
    if(b.size() != sz) {
        return false;
    }
    for(size_t i = 0; i < sz; ++i) {
        if(std::tolower(a[i]) != std::tolower(b[i])) {
            return false;
        }
    }
    return true;
}

static
int64_t parse_http_date(const std::string& date)
{
	std::tm tm = {};
	std::istringstream ss(date);
	ss.imbue(std::locale("en_US.utf-8"));
	ss >> std::get_time(&tm, "%a, %d %b %Y %H:%M:%S GMT");
	if(ss.fail()) {
		return -1;
	}
	return ::timegm(&tm);
}


CrawlFrontend::CrawlFrontend(const std::string& _vnx_name)
	:	CrawlFrontendBase(_vnx_name)
{
	unique_service = Hash64::rand();
}

void CrawlFrontend::init()
{
	vnx::open_pipe(vnx_name, this, UNLIMITED);			// unlimited since clients control number of pending
	vnx::open_pipe(unique_service, this, UNLIMITED);	// unlimited since clients control number of pending
}

void CrawlFrontend::main()
{
	subscribe(output_http, 1000);		// need to block here due to potential processing bottleneck
	
	work_threads.resize(num_threads);
	for(int i = 0; i < num_threads; ++i) {
		work_threads[i] = std::thread(&CrawlFrontend::fetch_loop, this);
	}
	
	set_timer_millis(stats_interval_ms, std::bind(&CrawlFrontend::print_stats, this));
	
	Super::main();
	
	work_condition.notify_all();
	for(auto& thread : work_threads) {
		if(thread.joinable()) {
			thread.join();
		}
	}
}

void CrawlFrontend::fetch_async(	const std::string& url,
									const std::function<void(const std::shared_ptr<const UrlIndex>&)>& _callback,
									const vnx::request_id_t& _request_id) const
{
	auto request = std::make_shared<request_t>();
	request->url = url;
	{
		std::set<std::string> mime_types;
		for(const auto& entry : parser_map) {
			mime_types.insert(entry.second.content_types.begin(), entry.second.content_types.end());
		}
		for(const auto& type : mime_types) {
			request->accept_content.push_back(type);
		}
	}
	request->request_id = _request_id;
	request->callback = _callback;
	{
		std::lock_guard lock(mutex);
		work_queue.push(request);
	}
	work_condition.notify_one();
}

void CrawlFrontend::register_parser(const vnx::Hash64& address,
									const std::vector<std::string>& mime_types,
									const int32_t& num_threads)
{
	auto& entry = parser_map[address];
	if(!entry.client) {
		entry.address = address;
		entry.client = std::make_shared<ContentParserAsyncClient>(address);
		entry.client->vnx_set_error_callback(std::bind(&CrawlFrontend::parse_error, this, address, std::placeholders::_1, std::placeholders::_2));
		add_async_client(entry.client);
		for(auto type : mime_types) {
			entry.content_types.insert(type);
			log(INFO).out << "Got a '" << type << "' parser with " << num_threads << " threads";
		}
	}
}

void CrawlFrontend::handle(std::shared_ptr<const HttpResponse> value)
{
	log(INFO).out << "Fetched '" << value->url << "': " << value->payload.size() << " bytes in " << value->fetch_duration_us/1000
			<< " ms (" << float(value->payload.size() / (value->fetch_duration_us * 1e-6f) / 1024.) << " KB/s)";
	
	uint64_t parse_id = 0;
	std::multimap<size_t, parser_t*> parser_list;
	
	for(auto& entry : parser_map) {
		auto& parser = entry.second;
		if(parser.content_types.count(value->content_type)) {
			parser_list.emplace(parser.client->vnx_get_num_pending(), &parser);
		}
	}
	if(!parser_list.empty()) {
		auto parser = parser_list.begin()->second;
		parse_id = parser->client->parse(value, std::bind(&CrawlFrontend::parse_callback, this, std::placeholders::_1));
	} else {
		log(WARN).out << "Cannot parse content type: '" << value->content_type << "'";
	}
	parse_counter++;
	num_bytes_fetched += value->payload.size();
}

void CrawlFrontend::parse_callback(std::shared_ptr<const TextResponse> value)
{
	log(INFO).out << "Parsed '" << value->url << "': " << value->text.size() << " bytes, "
			<< value->links.size() << " links, " << value->images.size() << " images";
	
	publish(value, output_text, BLOCKING);
	num_bytes_parsed += value->text.size();
}

void CrawlFrontend::parse_error(Hash64 address, uint64_t request_id, const std::exception& ex)
{
	// check error and remove parser if connection error
	auto vnx_except = dynamic_cast<const vnx::exception*>(&ex);
	if(vnx_except) {
		if(std::dynamic_pointer_cast<const NoSuchService>(vnx_except->value())) {
			parser_map.erase(address);
		}
	}
	parse_failed_counter++;
}

void CrawlFrontend::print_stats()
{
	const uint64_t fetch_count = fetch_counter;
	
	log(INFO).out << (60000 * (fetch_count - last_fetch_count)) / stats_interval_ms << " pages/min, "
			<< (1000 * (num_bytes_fetched - last_num_bytes_fetched) / 1024) / stats_interval_ms << " KB/s http, "
			<< (1000 * (num_bytes_parsed - last_num_bytes_parsed) / 1024) / stats_interval_ms << " KB/s text, "
			<< redirect_counter << " redirect, "
			<< connection_fail_counter << " network, "
			<< server_fail_counter << " server, "
			<< invalid_url_counter << " url, "
			<< invalid_content_type_counter << " content, "
			<< invalid_response_size_counter << " size, "
			<< empty_response_counter << " empty, "
			<< parse_failed_counter << " parse, "
			<< general_fail_counter << " other";
	
	last_fetch_count = fetch_count;
	last_num_bytes_fetched = num_bytes_fetched;
	last_num_bytes_parsed = num_bytes_parsed;
}

struct fetch_t {
	std::shared_ptr<CrawlFrontend::request_t> request;
	std::shared_ptr<HttpResponse> out;
	const CrawlFrontend* frontend = 0;
	CURL* client = 0;
	bool is_begin = true;
};

size_t CrawlFrontend::header_callback(char* buffer, size_t size, size_t len, void* userdata)
{
	const fetch_t* data = (const fetch_t*)userdata;
	const std::string line(buffer, len);
	
	if(iequals(line.substr(0, 4), "Date")) {
		const auto pos = line.find(':');
		if(pos != std::string::npos) {
			data->out->date = parse_http_date(line.substr(pos + 1));
		}
	}
	if(iequals(line.substr(0, 13), "Last-Modified")) {
		const auto pos = line.find(':');
		if(pos != std::string::npos) {
			data->out->last_modified = parse_http_date(line.substr(pos + 1));
		}
	}
	return len;
}

size_t CrawlFrontend::write_callback(char* buf, size_t size, size_t len, void* userdata)
{
	fetch_t* data = (fetch_t*)userdata;
	
	if(data->is_begin)
	{
		char* content_type_ = 0;
		std::string content_type;
		curl_easy_getinfo(data->client, CURLINFO_CONTENT_TYPE, &content_type_);
		if(content_type_) {
			content_type = std::string(content_type_);
			{
				auto pos = content_type.find("charset=");
				if(pos != std::string::npos) {
					pos += 8;
					auto end = content_type.find(';', pos);
					if(end != std::string::npos) {
						data->out->content_charset = content_type.substr(pos, end - pos);
					} else {
						data->out->content_charset = content_type.substr(pos);
					}
				}
			}
			{
				auto pos = content_type.find(';');
				if(pos != std::string::npos) {
					content_type = content_type.substr(0, pos);
				}
			}
		} else {
			if(is_robots_txt(Url::Url(data->request->url))) {
				content_type = "text/plain";		// assume it's text
			} else {
				data->frontend->invalid_content_type_counter++;
				return 0;
			}
		}
		
		bool valid_type = false;
		for(const auto& mime_type : data->request->accept_content) {
			if(content_type == mime_type) {
				valid_type = true;
			}
		}
		if(!valid_type) {
			if(is_robots_txt(Url::Url(data->request->url))) {
				content_type = "text/plain";		// assume it's really text
			} else {
				data->frontend->invalid_content_type_counter++;
				return 0;
			}
		}
		data->out->content_type = content_type;
		data->is_begin = false;
	}
	
	const size_t offset = data->out->payload.size();
	if(offset + len > data->frontend->max_response_size) {
		data->out->payload.clear();
		data->frontend->invalid_response_size_counter++;
		return 0;
	}
	data->out->payload.resize(offset + len);
	::memcpy(data->out->payload.data(offset), buf, len);
	return len;
}

void CrawlFrontend::fetch_loop() const noexcept
{
	Publisher publisher;
	
	while(vnx_do_run())
	{
		std::shared_ptr<request_t> request;
		{
			std::unique_lock<std::mutex> lock(mutex);
			while(vnx_do_run() && work_queue.empty()) {
				work_condition.wait(lock);
			}
			if(vnx_do_run()) {
				request = work_queue.front();
				work_queue.pop();
			} else {
				break;
			}
		}
		
		auto out = HttpResponse::create();
		out->url = request->url;
		out->payload.reserve(1048576);
		
		auto index = UrlIndex::create();
		index->last_fetched = std::time(0);
		index->is_fail = true;
		
		::signal(SIGPIPE, SIG_IGN);
		
		CURL* client = curl_easy_init();
		if(!client) {
			break;
		}
		
		fetch_t fetch_data;
		fetch_data.request = request;
		fetch_data.out = out;
		fetch_data.frontend = this;
		fetch_data.client = client;
		
		curl_easy_setopt(client, CURLOPT_URL, request->url.c_str());
		curl_easy_setopt(client, CURLOPT_PROTOCOLS, CURLPROTO_HTTP | CURLPROTO_HTTPS | CURLPROTO_FTP | CURLPROTO_FTPS | CURLPROTO_SFTP);
		curl_easy_setopt(client, CURLOPT_ACCEPT_ENCODING, "");
		curl_easy_setopt(client, CURLOPT_FOLLOWLOCATION, 1);
		curl_easy_setopt(client, CURLOPT_NOSIGNAL, 0);		// prevent libcurl from messing with signal handlers
		curl_easy_setopt(client, CURLOPT_NOPROGRESS, 1);
		curl_easy_setopt(client, CURLOPT_MAXFILESIZE, max_content_length);
		curl_easy_setopt(client, CURLOPT_TIMEOUT_MS, response_timeout_ms);
		curl_easy_setopt(client, CURLOPT_USERAGENT, user_agent.c_str());
		curl_easy_setopt(client, CURLOPT_SSL_VERIFYHOST, 0);
		curl_easy_setopt(client, CURLOPT_SSL_VERIFYPEER, 0);
		
		curl_easy_setopt(client, CURLOPT_HEADERDATA, &fetch_data);
		curl_easy_setopt(client, CURLOPT_HEADERFUNCTION, &header_callback);
		
		curl_easy_setopt(client, CURLOPT_WRITEDATA, &fetch_data);
		curl_easy_setopt(client, CURLOPT_WRITEFUNCTION, &write_callback);
		
		const auto fetch_start = vnx::get_wall_time_micros();
		
		const CURLcode res = curl_easy_perform(client);
		
		const auto fetch_time = vnx::get_wall_time_micros() - fetch_start;
		
		long status = 0;
		curl_easy_getinfo(client, CURLINFO_RESPONSE_CODE, &status);
		if(status == 0) {
			status = -1;
		}
		
		{
			char* final_url = 0;
			curl_easy_getinfo(client, CURLINFO_EFFECTIVE_URL, &final_url);
			if(final_url) {
				const std::string tmp(final_url);
				if(tmp != request->url) {
					out->url = tmp;
					index->redirect = tmp;
					redirect_counter++;
				}
			}
		}
		
		if(out->content_type.empty() && out->payload.size() == 0) {
			out->content_type = "text/plain";
		}
		
		switch(res) {
			case CURLE_OK:
				if(status == 200)
				{
					out->status = status;
					out->fetch_duration_us = fetch_time;
					if(!out->date) {
						out->date = index->last_fetched;
					}
					if(!out->last_modified) {
						out->last_modified = out->date;
					}
					publisher.publish(out, output_http, Message::BLOCKING);
					
					index->is_fail = false;
					fetch_counter++;
				}
				else {
					server_fail_counter++;
				}
				break;
			case CURLE_UNSUPPORTED_PROTOCOL:
			case CURLE_URL_MALFORMAT:
				invalid_url_counter++;
				break;
			case CURLE_COULDNT_RESOLVE_HOST:
			case CURLE_COULDNT_CONNECT:
			case CURLE_SSL_CONNECT_ERROR:
			case CURLE_SSL_CIPHER:
			case CURLE_OPERATION_TIMEDOUT:
			case CURLE_SEND_ERROR:
			case CURLE_RECV_ERROR:
			{
				const auto diff = 1000000 - fetch_time;
				if(diff > 0) {
					::usleep(diff);		// limit errors
				}
				connection_fail_counter++;
				break;
			}
			case CURLE_GOT_NOTHING:
				empty_response_counter++;
				break;
			case CURLE_FILESIZE_EXCEEDED:
				invalid_response_size_counter++;
				break;
			case CURLE_WRITE_ERROR:
				break;
			default:
				general_fail_counter++;
		}
		
		index->fetch_duration_us = fetch_time;
		index->http_status = status;
		index->num_bytes = out->payload.size();
		index->content_type = out->content_type;
		index->last_modified = out->last_modified;
		index->curl_status = res;
		
		curl_easy_cleanup(client);
		
		request->callback(index);
	}
}


} // search
} // vnx
