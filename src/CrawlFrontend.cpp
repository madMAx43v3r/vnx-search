/*
 * CrawlFrontend.cpp
 *
 *  Created on: Mar 30, 2020
 *      Author: mad
 */

#include <vnx/search/CrawlFrontend.h>
#include <vnx/search/Util.h>
#include <vnx/search/HttpResponse.hxx>
#include <vnx/search/LoadResult.hxx>
#include <vnx/search/CrawlFrontendClient.hxx>

#include <curl/curl.h>

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
}

void CrawlFrontend::init()
{
	vnx::open_pipe(vnx_name, this, UNLIMITED);			// unlimited since clients control number of pending
}

void CrawlFrontend::main()
{
	work_threads = std::make_shared<ThreadPool>(-1);
	for(int i = 0; i < num_threads; ++i) {
		work_threads->add_task(std::bind(&CrawlFrontend::fetch_loop, this));
	}
	
	set_timer_millis(stats_interval_ms, std::bind(&CrawlFrontend::print_stats, this));
	
	Super::main();
	
	work_condition.notify_all();
	work_threads->close();
}

void CrawlFrontend::load_async(	const std::string& url,
								const vnx::request_id_t& req_id) const
{
	auto request = std::make_shared<request_t>();
	request->url = url;
	request->is_fetch = false;
	request->parsed_url = Url::Url(url);
	request->req_id = req_id;
	{
		std::lock_guard lock(work_mutex);
		work_queue.push(request);
	}
	work_condition.notify_one();
}

void CrawlFrontend::fetch_async(const std::string& url,
								const vnx::request_id_t& req_id) const
{
	auto request = std::make_shared<request_t>();
	request->url = url;
	request->is_fetch = true;
	request->parsed_url = Url::Url(url);
	for(const auto& entry : parser_map) {
		const auto& parser = entry.second;
		request->accept_content.insert(parser.content_types.begin(), parser.content_types.end());
	}
	request->req_id = req_id;
	{
		std::lock_guard lock(work_mutex);
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
		add_async_client(entry.client);
		
		for(const auto& type : mime_types) {
			entry.content_types.insert(type);
			log(INFO).out << "Got a '" << type << "' parser with " << num_threads << " threads";
		}
	}
}

void CrawlFrontend::fetch_callback(	std::shared_ptr<const HttpResponse> response,
									std::shared_ptr<request_t> request)
{
	num_bytes_fetched += response->payload.size();
	log(INFO) << "Fetched '" << response->url << "': " << response->payload.size() << " bytes in " << response->fetch_duration_us/1000
			<< " ms (" << float(response->payload.size() / (response->fetch_duration_us * 1e-6) / 1024.) << " KB/s)";
	
	if(!request->is_fetch) {
		auto result = LoadResult::create();
		result->UrlInfo::operator=(request->info);
		result->response = response;
		load_async_return(request->req_id, result);
		return;
	}
	
	auto result = FetchResult::create();
	result->UrlInfo::operator=(request->info);
	if(request->info.is_fail) {
		fetch_async_return(request->req_id, result);
		return;
	}
	request->result = result;
	
	auto content_type = response->content_type;
	if(content_type.empty() && response->payload.size() == 0) {
		content_type = "text/plain";
	}
	
	std::multimap<size_t, parser_t*> parser_list;
	for(auto& entry : parser_map) {
		auto& parser = entry.second;
		if(parser.content_types.count(content_type)) {
			parser_list.emplace(parser.client->vnx_get_num_pending(), &parser);
		}
	}
	
	if(!parser_list.empty()) {
		auto parser = parser_list.begin()->second;
		request->parse_id = parser->client->parse(response,
					std::bind(&CrawlFrontend::parse_callback, this, std::placeholders::_1, request),
					std::bind(&CrawlFrontend::parse_error, this, parser->address, request, std::placeholders::_1));
	} else {
		fetch_async_return(request->req_id, result);
		log(WARN) << "Cannot parse content type: '" << content_type << "'";
	}
	
	publish(response, output_http);
}

void CrawlFrontend::parse_callback(	std::shared_ptr<const TextResponse> response,
									std::shared_ptr<request_t> request)
{
	num_bytes_parsed += response->text.size();
	log(INFO) << "Parsed '" << response->url << "': " << response->text.size() << " bytes, "
			<< response->links.size() << " links, " << response->images.size() << " images";
	
	request->result->response = response;
	fetch_async_return(request->req_id, request->result);
	
	publish(response, output_text);
}

void CrawlFrontend::parse_error(Hash64 address, std::shared_ptr<request_t> request, const std::exception& ex)
{
	// check error and remove parser if connection error
//	if(std::dynamic_pointer_cast<const NoSuchService>(ex.value())) {
//		parser_map.erase(address);
//	}
	fetch_async_return(request->req_id, request->result);
	parse_failed_counter++;
}

void CrawlFrontend::print_stats()
{
	const uint64_t fetch_count = fetch_counter;
	
	log(INFO) << (60000 * (fetch_count - last_fetch_count)) / stats_interval_ms << " pages/min, "
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
	std::shared_ptr<HttpResponse> http;
	const CrawlFrontend* frontend = nullptr;
	CURL* client = nullptr;
	bool is_begin = true;
};

size_t CrawlFrontend::header_callback(char* buffer, size_t size, size_t len, void* userdata)
{
	const fetch_t* data = (const fetch_t*)userdata;
	const std::string line(buffer, len);
	
	if(iequals(line.substr(0, 4), "Date")) {
		const auto pos = line.find(':');
		if(pos != std::string::npos) {
			data->http->date = parse_http_date(line.substr(pos + 1));
		}
	}
	if(iequals(line.substr(0, 13), "Last-Modified")) {
		const auto pos = line.find(':');
		if(pos != std::string::npos) {
			data->http->last_modified = parse_http_date(line.substr(pos + 1));
		}
	}
	return len;
}

size_t CrawlFrontend::write_callback(char* buf, size_t size, size_t len, void* userdata)
{
	fetch_t* data = (fetch_t*)userdata;
	
	if(data->is_begin)
	{
		char* content_type_ = nullptr;
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
						data->http->content_charset = content_type.substr(pos, end - pos);
					} else {
						data->http->content_charset = content_type.substr(pos);
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
			if(is_robots_txt(data->request->parsed_url)) {
				content_type = "text/plain";		// assume it's text
			} else {
				data->frontend->invalid_content_type_counter++;
				return 0;
			}
		}
		
		const auto& accept_content = data->request->accept_content;
		if(!accept_content.empty()
			&& accept_content.find(content_type) == accept_content.end())
		{
			data->frontend->invalid_content_type_counter++;
			return 0;
		}
		data->http->content_type = content_type;
		data->is_begin = false;
	}
	
	const size_t offset = data->http->payload.size();
	if(offset + len > data->frontend->max_response_size) {
		data->http->payload.clear();
		data->frontend->invalid_response_size_counter++;
		return 0;
	}
	data->http->payload.resize(offset + len);
	::memcpy(data->http->payload.data(offset), buf, len);
	return len;
}

void CrawlFrontend::fetch_loop() noexcept
{
	::signal(SIGPIPE, SIG_IGN);
	
	while(vnx_do_run())
	{
		std::shared_ptr<request_t> request;
		{
			std::unique_lock<std::mutex> lock(work_mutex);
			while(vnx_do_run() && work_queue.empty()) {
				work_condition.wait(lock);
			}
			if(!work_queue.empty()) {
				request = work_queue.front();
				work_queue.pop();
			} else {
				break;
			}
		}
		
		auto& info = request->info;
		info.last_fetched = std::time(0);
		info.is_fail = true;
		
		auto http = HttpResponse::create();
		http->url = request->url;
		
		CURL* client = curl_easy_init();
		if(!client) {
			break;
		}
		
		fetch_t fetch_data;
		fetch_data.request = request;
		fetch_data.http = http;
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
			char* final_url = nullptr;
			curl_easy_getinfo(client, CURLINFO_EFFECTIVE_URL, &final_url);
			if(final_url) {
				try {
					const Url::Url parent(request->url);
					const auto url = process_link(Url::Url(std::string(final_url)), parent).str();
					if(url != request->url) {
						http->url = url;
						info.redirect = url;
						redirect_counter++;
					}
				}
				catch(...) {
					// ignore
				}
			}
		}
		
		http->status = status;
		http->fetch_duration_us = fetch_time;
		if(!http->date) {
			http->date = info.last_fetched;
		}
		if(!http->last_modified) {
			http->last_modified = http->date;
		}
		
		switch(res) {
			case CURLE_OK:
				if(status == 200) {
					info.is_fail = false;
					fetch_counter++;
				} else {
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
		
		info.fetch_duration_us = fetch_time;
		info.http_status = status;
		info.num_bytes = http->payload.size();
		info.content_type = http->content_type;
		info.last_modified = http->last_modified;
		info.curl_status = res;
		
		curl_easy_cleanup(client);
		
		add_task(std::bind(&CrawlFrontend::fetch_callback, this, http, request));
	}
}


} // search
} // vnx
