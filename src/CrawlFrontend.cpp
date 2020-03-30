/*
 * CrawlFrontend.cpp
 *
 *  Created on: Mar 30, 2020
 *      Author: mad
 */

#include <vnx/search/CrawlFrontend.h>
#include <vnx/search/HttpResponse.hxx>

#define CPPHTTPLIB_ZLIB_SUPPORT
#define CPPHTTPLIB_OPENSSL_SUPPORT
#include <httplib.h>

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
	{
		auto request = std::make_shared<request_t>();
		request->url = "https://example.com/";
		request->protocol = "https";
		request->host = "example.com";
		request->path = "/";
		request->port = 443;
		request->accept_content = {"text/plain", "text/html"};
		request->callback = [](){};
		{
			std::lock_guard<std::mutex> lock(mutex);
			work_queue.push(request);
			condition.notify_one();
		}
	}
	
	Super::main();
	
	condition.notify_all();
	for(auto& thread : work_threads) {
		if(thread.joinable()) {
			thread.join();
		}
	}
}

void CrawlFrontend::fetch_async(const std::string& url,
								const std::function<void()>& _callback,
								const vnx::request_id_t& _request_id)
{
	// TODO
}

void CrawlFrontend::register_parser(const vnx::Hash64& address,
									const std::string& mime_type,
									const int32_t& num_threads)
{
	auto& client = parser_map[mime_type][address];
	if(!client) {
		client = std::make_shared<ContentParserAsyncClient>(address);
		log(INFO).out << "Got a '" << mime_type << "' parser with " << num_threads << " threads";
	}
}

void CrawlFrontend::handle(std::shared_ptr<const HttpResponse> value)
{
	log(INFO).out << "Fetched '" << value->url << "': " << value->payload.size() << " bytes";
	
	log(INFO).out << *value;
	
	num_bytes_fetched += value->payload.size();
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
		out->payload.reserve(max_response_size);
		
		httplib::Headers headers = {
				{"Connection", "close"},
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
		
		auto response_handler = [&](const httplib::Response& response) -> bool
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
			for(const auto& mime_type : request->accept_content) {
				if(content_type == mime_type) {
					valid_type = true;
				}
			}
			if(!valid_type) {
				invalid_content_type_counter++;
				return false;
			}
			if(response.content_length > max_response_size) {
				invalid_reponse_size_counter++;
				return false;
			}
			return true;
		};
		
		auto content_receiver = [&](const char* buf, size_t len) -> bool
		{
			if(out->payload.capacity() - out->payload.size() < len) {
				return false;
			}
			::memcpy(out->payload.data(out->payload.size()), buf, len);
			out->payload.resize(out->payload.size() + len);
			return true;
		};
		
		httplib::Client* client = 0;
		const auto fetch_start = vnx::get_wall_time_micros();
		
		try {
			if(request->protocol == "http") {
				client = new httplib::Client(request->host, request->port);
			}
			else if(request->protocol == "https") {
				auto ssl_client = new httplib::SSLClient(request->host, request->port);
				ssl_client->enable_server_certificate_verification(false);
				client = ssl_client;
			}
			else {
				invalid_protocol_counter++;
				throw std::logic_error("unsupported protocol");
			}
			client->set_follow_location(true);
			client->set_timeout_sec(response_timeout_ms / 1000);
			
			auto response = client->Get(request->path.c_str(), headers, response_handler, content_receiver);
			
			if(response) {
				if(response->has_header("Date")) {
					out->date = parse_http_date(response->get_header_value("Date"));
				}
				if(response->has_header("Last-Modified")) {
					out->last_modified = parse_http_date(response->get_header_value("Last-Modified"));
				}
				if(response->has_header("Content-Type")) {
					out->content_type = response->get_header_value("Content-Type");
				}
				out->status = response->status;
				out->fetch_time = int32_t(vnx::get_wall_time_micros() - fetch_start);
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
		delete client;
		
		request->callback();
	}
}


} // search
} // vnx
