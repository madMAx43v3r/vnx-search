/*
 * CrawlFrontend.cpp
 *
 *  Created on: Mar 30, 2020
 *      Author: mad
 */

#include <vnx/search/CrawlFrontend.h>
#include <vnx/search/HttpResponse.hxx>
#include <vnx/search/CrawlProcessorClient.hxx>

#define CPPHTTPLIB_ZLIB_SUPPORT
#define CPPHTTPLIB_OPENSSL_SUPPORT
#include <httplib.h>

#include <url.h>

#include <sstream>
#include <locale>
#include <iomanip>
#include <time.h>


namespace vnx {
namespace search {

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
	vnx::open_pipe(vnx_name, this, UNLIMITED);
	vnx::open_pipe(unique_service, this, UNLIMITED);
}

void CrawlFrontend::main()
{
	subscribe(output_http);
	
	work_threads.resize(num_threads);
	for(int i = 0; i < num_threads; ++i) {
		work_threads[i] = std::thread(&CrawlFrontend::fetch_loop, this);
	}
	
	set_timer_millis(stats_interval_ms, std::bind(&CrawlFrontend::print_stats, this));
	
	Super::main();
	
	condition.notify_all();
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
	Url::Url parsed(url);
	parsed.defrag();
	
	const std::string protocol = parsed.scheme();
	const std::string host = parsed.host();
	const std::string path = parsed.fullpath();
	const int port = parsed.port();
	
	log(DEBUG).out << "Fetching '" << protocol << "://" << host << path << "' (port " << port << ")";
	
	auto request = std::make_shared<request_t>();
	request->url = url;
	request->protocol = protocol;
	request->host = host;
	request->path = path;
	request->port = port;
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
		std::lock_guard<std::mutex> lock(mutex);
		work_queue.push(request);
		condition.notify_one();
	}
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
	log(INFO).out << "Fetched '" << value->url << "': " << value->payload.size() << " bytes in " << value->fetch_time_us/1000
			<< " ms (" << float(value->payload.size() / (value->fetch_time_us * 1e-6f) / 1024.) << " KB/s)";
	
	bool parse_ok = false;
	uint64_t parse_id = 0;
	
	// TODO: proper load balancing
	for(auto iter = parser_map.begin(); iter != parser_map.end();)
	{
		const auto& parser = iter->second;
		if(parser.content_types.count(value->content_type)) {
			try {
				parse_id = parser.client->parse(value,
						std::bind(&CrawlFrontend::parse_callback, this, std::placeholders::_1));
				parse_ok = true;
				break;
			}
			catch(const std::exception& ex) {
				log(WARN).out << "parse(): " << ex.what();
			}
			rem_async_client(parser.client);
			iter = parser_map.erase(iter);
		} else {
			iter++;
		}
	}
	if(!parse_ok) {
		log(WARN).out << "Cannot parse content type: '" << value->content_type << "'";
	}
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
			<< general_fail_counter << " fetch error, "
			<< invalid_content_type_counter << " content type, "
			<< invalid_protocol_counter << " protocol, "
			<< invalid_response_size_counter << " response size, "
			<< parse_failed_counter << " parse fail";
	
	last_fetch_count = fetch_count;
	last_num_bytes_fetched = num_bytes_fetched;
	last_num_bytes_parsed = num_bytes_parsed;
}

void CrawlFrontend::fetch_loop()
{
	Publisher publisher;
	
	while(vnx_do_run())
	{
		std::shared_ptr<request_t> request;
		{
			std::unique_lock<std::mutex> lock(mutex);
			while(vnx_do_run() && work_queue.empty()) {
				condition.wait(lock);
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
		
		httplib::Headers headers = {
				{"User-Agent", user_agent},
				{"Accept-Encoding", "gzip, deflate"}
		};
		{
			int i = 0;
			std::string list;
			for(const auto& mime_type : request->accept_content) {
				list += (i++ > 0 ? ", " : "") + mime_type;
			}
			headers.emplace("Accept", list);
		}
		
		auto response_handler = [&, request, out](const httplib::Response& response) -> bool
		{
			out->payload.clear();
			
			if(response.status == 200)
			{
				if(!response.has_header("Content-Type")) {
					return false;
				}
				
				bool valid_type = false;
				auto content_type = response.get_header_value("Content-Type");
				{
					auto pos = content_type.find("charset=");
					if(pos != std::string::npos) {
						pos += 8;
						auto end = content_type.find_first_of(';', pos);
						if(end != std::string::npos) {
							out->content_charset = content_type.substr(pos, end - pos);
						} else {
							out->content_charset = content_type.substr(pos);
						}
					}
				}
				{
					auto pos = content_type.find_first_of(';');
					if(pos != std::string::npos) {
						content_type = content_type.substr(0, pos);
					}
				}
				out->content_type = content_type;
				
				for(const auto& mime_type : request->accept_content) {
					if(content_type == mime_type) {
						valid_type = true;
					}
				}
				if(!valid_type) {
					invalid_content_type_counter++;
					return false;
				}
			}
			if(response.content_length > max_content_length) {
				invalid_response_size_counter++;
				return false;
			}
			return true;
		};
		
		auto content_receiver = [&, out](const char* buf, size_t len) -> bool
		{
			const size_t offset = out->payload.size();
			if(offset + len > max_response_size) {
				invalid_response_size_counter++;
				return false;
			}
			out->payload.resize(offset + len);
			::memcpy(out->payload.data(offset), buf, len);
			return true;
		};
		
		auto index = UrlIndex::create();
		index->last_fetched = std::time(0);
		index->is_fail = true;
		
		std::shared_ptr<httplib::Client> client;
		
		if(request->protocol == "http") {
			if(!client) {
				client = std::make_shared<httplib::Client>(request->host, request->port > 0 ? request->port : 80);
			}
		}
		else if(request->protocol == "https") {
			if(!client) {
				auto ssl_client = std::make_shared<httplib::SSLClient>(request->host, request->port > 0 ? request->port : 443);
				ssl_client->enable_server_certificate_verification(false);
				client = ssl_client;
			}
		}
		else {
			invalid_protocol_counter++;
			auto ex = InternalError::create();
			ex->what = "unsupported protocol: '" + request->protocol + "'";
			vnx_async_return(request->request_id, ex);
			continue;
		}
		
		client->set_follow_location(true);
		client->set_timeout_sec(response_timeout_ms / 1000);
		
		try {
			const auto fetch_start = vnx::get_wall_time_micros();
			
			auto response = client->Get(request->path.c_str(), headers, response_handler, content_receiver);
			
			const auto fetch_time = vnx::get_wall_time_micros() - fetch_start;
			
			if(response) {
				index->http_status = response->status;
				index->fetch_time_us = fetch_time;
			}
			
			if(response && response->status == 200)
			{
				out->status = response->status;
				out->fetch_time_us = fetch_time;
				
				if(response->has_header("Date")) {
					out->date = parse_http_date(response->get_header_value("Date"));
				} else {
					out->date = index->last_fetched;
				}
				if(response->has_header("Last-Modified")) {
					out->last_modified = parse_http_date(response->get_header_value("Last-Modified"));
				} else {
					out->last_modified = out->date;
				}
				publish(out, output_http, Message::BLOCKING);
				
				index->is_fail = false;
				fetch_counter++;
			}
			else {
				general_fail_counter++;
			}
		}
		catch(...) {
			general_fail_counter++;
		}
		
		index->content_type = out->content_type;
		index->last_modified = out->last_modified;
		
		request->callback(index);
	}
}


} // search
} // vnx
