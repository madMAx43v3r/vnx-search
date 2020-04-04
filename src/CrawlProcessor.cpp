/*
 * CrawlProcessor.cpp
 *
 *  Created on: Apr 4, 2020
 *      Author: mad
 */

#include <vnx/search/CrawlProcessor.h>

#include <url.h>
#include <time.h>


namespace vnx {
namespace search {

CrawlProcessor::CrawlProcessor(const std::string& _vnx_name)
	:	CrawlProcessorBase(_vnx_name)
{
}

void CrawlProcessor::main()
{
	subscribe(input_url_index, max_queue_ms);
	subscribe(input_page_index, max_queue_ms);
	
	url_index = std::make_shared<keyvalue::ServerClient>(url_index_server);
	url_index_async = std::make_shared<keyvalue::ServerAsyncClient>(url_index_server);
	
	url_index_async->vnx_set_error_callback(std::bind(&CrawlProcessor::url_index_error, this, std::placeholders::_1, std::placeholders::_2));
	
	add_async_client(url_index_async);
	
	set_timer_millis(update_interval_ms, std::bind(&CrawlProcessor::check_queue, this));
	
	Super::main();
}

void CrawlProcessor::handle(std::shared_ptr<const vnx::keyvalue::KeyValuePair> pair)
{
	const std::string url = pair->key.to_string_value();
	{
		auto index = std::dynamic_pointer_cast<const UrlIndex>(pair->value);
		if(index) {
			check_url(url, index->depth, index);
			return;
		}
	}
	{
		auto index = std::dynamic_pointer_cast<const PageIndex>(pair->value);
		if(index) {
			auto iter = url_map.find(url);
			if(iter != url_map.end()) {
				check_page(url, iter->second.depth, index);
			} else {
				try {
					url_index_async->get_value(pair->key,
							std::bind(&CrawlProcessor::check_page_callback, this, url, std::placeholders::_1, index));
				}
				catch(const std::exception& ex) {
					log(WARN).out << "UrlIndex: " << ex.what();
				}
			}
			return;
		}
	}
}

void CrawlProcessor::enqueue(const std::string& url, int depth, uint32_t load_time)
{
	const int64_t delta = int64_t(load_time) - int64_t(std::time(0));
	if(delta < 0) {
		queue.emplace(depth, url);
	} else {
		waiting.emplace(load_time, url);
	}
	url_t& map_ = url_map[url];
	map_.depth = depth;
}

void CrawlProcessor::check_queue()
{
	// TODO
}

void CrawlProcessor::check_url(const std::string& url, int depth, std::shared_ptr<const Value> index_)
{
	if(url_map.find(url) != url_map.end()) {
		return;
	}
	auto index = std::dynamic_pointer_cast<const UrlIndex>(index_);
	if(index) {
		const uint32_t load_time = index->last_fetched + uint32_t(depth * reload_interval);
		enqueue(url, depth, load_time);
	} else {
		enqueue(url, depth);
	}
}

void CrawlProcessor::check_page_callback(const std::string& url, std::shared_ptr<const Value> url_index_, std::shared_ptr<const PageIndex> page_index_)
{
	auto index = std::dynamic_pointer_cast<const UrlIndex>(url_index_);
	if(index) {
		check_page(url, index->depth, page_index_);
	}
}

void CrawlProcessor::check_page(const std::string& url, int depth, std::shared_ptr<const PageIndex> index)
{
	const Url::Url parent(url);
	
	for(const auto& link : index->links)
	{
		Url::Url parsed(link);
		parsed.defrag();
		parsed.remove_default_port();
		
		if(parsed.scheme().empty()) {
			parsed.setScheme(parent.scheme());
		}
		
		if(parsed.host().empty()) {
			parsed.setHost(parent.host());
			// check if path is relative
			if(!parsed.path().empty() && parsed.path()[0] != '/') {
				parsed.relative_to(parent);
			}
		}
		parsed.abspath();
		
		const int link_depth = depth + (parsed.host() != parent.host() ? jump_cost : 1);
		const std::string full_link = parsed.str();
		
		try {
			url_index_async->get_value(full_link, std::bind(&CrawlProcessor::check_url, this, full_link, link_depth, std::placeholders::_1));
		}
		catch(const std::exception& ex) {
			log(WARN).out << "UrlIndex: " << ex.what();
		}
	}
	
}

void CrawlProcessor::url_fetched(const std::string& url, int depth, uint64_t req_id, std::shared_ptr<const UrlIndex> index)
{
	auto copy = vnx::clone(index);
	copy->depth = depth;
	
	url_map.erase(url);
	pending_urls.erase(req_id);
	
	try {
		url_index_async->store_value(url, copy);
	}
	catch(const std::exception& ex) {
		log(WARN).out << "UrlIndex: " << ex.what();
	}
}

void CrawlProcessor::url_fetch_error(uint64_t req_id, const std::exception& ex)
{
	auto iter = pending_urls.find(req_id);
	if(iter != pending_urls.end()) {
		url_map.erase(iter->second);
		pending_urls.erase(req_id);
	}
	log(WARN).out << "fetch(): " << ex.what();
}

void CrawlProcessor::url_index_error(uint64_t req_id, const std::exception& ex)
{
	log(WARN).out << "UrlIndex: " << ex.what();
}


} // search
} // vnx
