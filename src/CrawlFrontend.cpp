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
}

void CrawlFrontend::init()
{
	unique_service = Hash64::rand();
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
	
	// test
	auto dummy = [](const std::shared_ptr<const UrlIndex>&){};
	fetch_async("https://example.com/", dummy, vnx::request_id_t());
	::usleep(2000 * 1000);
	fetch_async("https://example.com/", dummy, vnx::request_id_t());
	
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
	const Url::Url parsed(url);
	
	const std::string protocol = parsed.scheme();
	const std::string host = parsed.host();
	const std::string path = parsed.fullpath();
	const int port = parsed.port();
	
	log(DEBUG).out << "Fetching '" << protocol << "://" << host << path << "' (port " << port << ")";
	
	std::shared_ptr<httplib::Client> client;
	
	if(protocol == "http") {
		auto& list = http_clients[host];
		for(const auto& client_ : list) {
			if(client_.use_count() == 1) {
				client = client_;
				break;
			}
		}
		if(!client) {
			client = std::make_shared<httplib::Client>(host, port > 0 ? port : 80);
			list.push_back(client);
		}
	}
	else if(protocol == "https") {
		auto& list = https_clients[host];
		for(const auto& client_ : list) {
			if(client_.use_count() == 1) {
				client = client_;
				break;
			}
		}
		if(!client) {
			auto ssl_client = std::make_shared<httplib::SSLClient>(host, port > 0 ? port : 443);
			ssl_client->enable_server_certificate_verification(false);
			list.push_back(ssl_client);
			client = ssl_client;
		}
	}
	else {
		invalid_protocol_counter++;
		throw std::logic_error("unsupported protocol: " + protocol);
	}
	
	client->set_follow_location(true);
	client->set_timeout_sec(response_timeout_ms / 1000);
	
	auto request = std::make_shared<request_t>();
	request->url = url;
	request->path = path;
	request->client = client;
	request->accept_content = {"text/plain", "text/html"};		// TODO
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
	for(auto type : mime_types) {
		auto& client = parser_map[type][address];
		if(!client) {
			client = std::make_shared<ContentParserAsyncClient>(address);
			log(INFO).out << "Got a '" << type << "' parser with " << num_threads << " threads";
		}
	}
}

void CrawlFrontend::handle(std::shared_ptr<const HttpResponse> value)
{
	log(INFO).out << "Fetched '" << value->url << "': " << value->payload.size() << " bytes in " << value->fetch_time/1000
			<< " ms (" << float(value->payload.size() / (value->fetch_time * 1e-6f) / 1024.) << " KB/s)";
	
	auto iter = parser_map.find(value->content_type);
	if(iter != parser_map.end() && iter->second.size() > 0)
	{
		// TODO: proper load balancing
		iter->second.begin()->second->parse(value, std::bind(&CrawlFrontend::parse_callback, this, std::placeholders::_1));
	} else {
		log(WARN).out << "Cannot parse content type: " << value->content_type;
	}
	num_bytes_fetched += value->payload.size();
}

void CrawlFrontend::parse_callback(std::shared_ptr<const TextResponse> value)
{
	log(INFO).out << "Parsed '" << value->url << "': " << value->text.size() << " bytes";
	
	publish(value, output_text, BLOCKING);
	num_bytes_parsed += value->text.size();
}

void CrawlFrontend::print_stats()
{
	const uint64_t fetch_count = fetch_counter;
	const uint64_t total_error_count = general_fail_counter + invalid_content_type_counter
			+ invalid_protocol_counter + invalid_reponse_size_counter;
	
	log(INFO).out << 60000 * (fetch_count - last_fetch_count) / stats_interval_ms / 1000 << " pages/min, "
			<< 60000 * (total_error_count - last_error_count) / stats_interval_ms / 1000 << " errors/min";
	
	last_fetch_count = fetch_count;
	last_error_count = total_error_count;
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
				{"Connection", "keep-alive"},
				{"User-Agent", "Mozilla/5.0"},
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
			bool valid_type = false;
			if(!response.has_header("Content-Type")) {
				return false;
			}
			auto content_type = response.get_header_value("Content-Type");
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
			if(response.content_length > max_content_length) {
				invalid_reponse_size_counter++;
				return false;
			}
			return true;
		};
		
		auto content_receiver = [&, out](const char* buf, size_t len) -> bool
		{
			const size_t offset = out->payload.size();
			if(offset + len > max_response_size) {
				invalid_reponse_size_counter++;
				return false;
			}
			out->payload.resize(offset + len);
			::memcpy(out->payload.data(offset), buf, len);
			return true;
		};
		
		auto index = UrlIndex::create();
		index->last_fetched = std::time(0);
		
		try {
			const auto fetch_start = vnx::get_wall_time_micros();
			
			auto response = request->client->Get(request->path.c_str(), headers, response_handler, content_receiver);
			
			const auto fetch_time = int32_t(vnx::get_wall_time_micros() - fetch_start);
			
			if(response) {
				index->http_status = response->status;
				index->is_fail = response->status != 200;
				index->fetch_time = fetch_time;
			}
			
			if(response && response->status == 200)
			{
				out->status = response->status;
				out->fetch_time = fetch_time;
				
				if(response->has_header("Date")) {
					out->date = parse_http_date(response->get_header_value("Date"));
				}
				if(response->has_header("Last-Modified")) {
					out->last_modified = parse_http_date(response->get_header_value("Last-Modified"));
				}
				
				publish(out, output_http, Message::BLOCKING);
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
		request = 0;
	}
}


} // search
} // vnx
