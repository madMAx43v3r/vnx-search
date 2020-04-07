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
	url_sync_topic = vnx_name + ".url_index.sync";
}

void CrawlProcessor::main()
{
	subscribe(input_url_index, max_queue_ms);
	subscribe(input_page_index, max_queue_ms);
	subscribe(url_sync_topic, 10);
	
	url_index = std::make_shared<keyvalue::ServerClient>(url_index_server);
	url_index_async = std::make_shared<keyvalue::ServerAsyncClient>(url_index_server);
	crawl_frontend_async = std::make_shared<CrawlFrontendAsyncClient>(crawl_frontend_server);
	
	url_index_async->vnx_set_error_callback(std::bind(&CrawlProcessor::url_index_error, this, std::placeholders::_1, std::placeholders::_2));
	crawl_frontend_async->vnx_set_error_callback(std::bind(&CrawlProcessor::url_fetch_error, this, std::placeholders::_1, std::placeholders::_2));
	
	add_async_client(url_index_async);
	add_async_client(crawl_frontend_async);
	
	set_timer_millis(1000, std::bind(&CrawlProcessor::print_stats, this));
	set_timer_millis(update_interval_ms, std::bind(&CrawlProcessor::check_queue, this));
	set_timer_millis(sync_interval * 1000, std::bind(&CrawlProcessor::check_all_urls, this));
	
	for(const auto& url : root_urls)
	{
		Url::Url parsed(url);
		parsed.defrag();
		parsed.remove_default_port();
		if(parsed.scheme().empty()) {
			parsed.setScheme("https");
		}
		parsed.abspath();
		enqueue(parsed.str(), 0);
	}
	
	url_index->sync_all(url_sync_topic);
	
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

bool CrawlProcessor::enqueue(const std::string& url, int depth, int64_t load_time)
{
	if(url_map.find(url) != url_map.end()) {
		return false;
	}
	const auto delta = load_time - std::time(0);
	if(delta <= 0) {
		queue.emplace(depth, url);
	} else if(delta < 2 * sync_interval) {
		waiting.emplace(load_time, url);
	} else {
		return false;
	}
	const Url::Url parsed(url);
	url_t& entry = url_map[url];
	entry.domain = parsed.host();
	entry.depth = depth;
	entry.is_reload = load_time > 0;
	return true;
}

void CrawlProcessor::check_queue()
{
	const int64_t now_posix = std::time(0);
	const int64_t now_wall = vnx::get_wall_time_micros();
	
	while(!waiting.empty())
	{
		const auto entry = waiting.begin();
		if(entry->first <= now_posix) {
			queue.emplace(url_map[entry->second].depth, entry->second);
			waiting.erase(entry);
		} else {
			break;
		}
	}
	
	for(auto iter = queue.begin(); iter != queue.end();)
	{
		if(pending_urls.size() >= max_num_pending) {
			break;
		}
		
		url_t& url = url_map[iter->second];
		domain_t& domain = domain_map[url.domain];
		
		if(domain.num_pending < max_per_domain
				&& now_wall - domain.last_fetch_us > int64_t(60 * 1000 * 1000) / max_per_minute)
		{
			try {
				url.request_id = crawl_frontend_async->fetch(iter->second,
						std::bind(&CrawlProcessor::url_fetch_callback, this, iter->second, std::placeholders::_1));
				
				pending_urls.emplace(url.request_id, iter->second);
				
				domain.num_pending++;
				domain.last_fetch_us = now_wall;
				
				average_depth = url.depth * 0.01 + average_depth * 0.99;
			}
			catch(const std::exception& ex) {
				break;
			}
			iter = queue.erase(iter);
		}
		else {
			iter++;
		}
	}
}

void CrawlProcessor::check_all_urls()
{
	url_index->sync_all(url_sync_topic);
}

void CrawlProcessor::check_url(const std::string& url, int depth, std::shared_ptr<const Value> index_)
{
	auto index = std::dynamic_pointer_cast<const UrlIndex>(index_);
	if(index) {
		if(index->last_fetched > 0) {
			const int64_t load_time = index->last_fetched + (depth + 1) * reload_interval;
			enqueue(url, depth, load_time);
		} else {
			enqueue(url, depth);
		}
	} else {
		try {
			auto index = UrlIndex::create();
			index->depth = depth;
			url_index_async->store_value(url, index);
		}
		catch(const std::exception& ex) {
			log(WARN).out << "UrlIndex: " << ex.what();
		}
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

CrawlProcessor::url_t CrawlProcessor::url_fetch_done(const std::string& url)
{
	const auto entry = url_map[url];
	domain_map[entry.domain].num_pending--;
	pending_urls.erase(entry.request_id);
	url_map.erase(url);
	return entry;
}

void CrawlProcessor::url_fetch_callback(const std::string& url, std::shared_ptr<const UrlIndex> index)
{
	const url_t entry = url_fetch_done(url);
	
	if(index) {
		auto copy = vnx::clone(index);
		copy->depth = entry.depth;
		try {
			url_index_async->store_value(url, copy);
		}
		catch(const std::exception& ex) {
			log(WARN).out << "UrlIndex: " << ex.what();
		}
		fetch_counter++;
		reload_counter += entry.is_reload ? 1 : 0;
	}
}

void CrawlProcessor::url_fetch_error(uint64_t request_id, const std::exception& ex)
{
	auto iter = pending_urls.find(request_id);
	if(iter != pending_urls.end())
	{
		const auto url = iter->second;
		const auto entry = url_fetch_done(url);
		
		log(WARN).out << "fetch('" << url << "'): " << ex.what();
		
		auto vnx_except = dynamic_cast<const vnx::exception*>(&ex);
		if(vnx_except) {
			if(std::dynamic_pointer_cast<const NoSuchService>(vnx_except->value())) {
				enqueue(url, entry.depth);
			} else {
				try {
					auto index = UrlIndex::create();
					index->depth = entry.depth;
					index->last_fetched = std::time(0);
					index->is_fail = true;
					url_index_async->store_value(url, index);
				}
				catch(const std::exception& ex) {
					log(WARN).out << "UrlIndex: " << ex.what();
				}
			}
		}
	}
}

void CrawlProcessor::url_index_error(uint64_t request_id, const std::exception& ex)
{
	log(WARN).out << "UrlIndex: " << ex.what();
}

void CrawlProcessor::print_stats()
{
	log(INFO).out << queue.size() << " queued, " << waiting.size() << " waiting, "
			<< pending_urls.size() << " pending, " << fetch_counter << " fetched, "
			<< reload_counter << " reload, " << average_depth << " avg. depth";
}


} // search
} // vnx
