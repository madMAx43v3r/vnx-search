/*
 * CrawlProcessor.cpp
 *
 *  Created on: Apr 4, 2020
 *      Author: mad
 */

#include <vnx/search/CrawlProcessor.h>
#include <vnx/search/CrawlStats.hxx>

#include <url.h>
#include <time.h>
#include <math.h>


namespace vnx {
namespace search {

CrawlProcessor::CrawlProcessor(const std::string& _vnx_name)
	:	CrawlProcessorBase(_vnx_name)
{
	url_sync_topic = vnx_name + ".url_index.sync";
}

void CrawlProcessor::main()
{
	subscribe(input_url_index, UNLIMITED);		// needs to be unlimited to avoid dead-lock
	subscribe(input_page_index, UNLIMITED);		// needs to be unlimited to avoid dead-lock
	subscribe(url_sync_topic, 100);				// sync runs in a separate thread so we can block here
	
	url_index = std::make_shared<keyvalue::ServerClient>(url_index_server);
	url_index_async = std::make_shared<keyvalue::ServerAsyncClient>(url_index_server);
	crawl_frontend_async = std::make_shared<CrawlFrontendAsyncClient>(crawl_frontend_server);
	
	url_index_async->vnx_set_error_callback(std::bind(&CrawlProcessor::url_index_error, this, std::placeholders::_1, std::placeholders::_2));
	crawl_frontend_async->vnx_set_error_callback(std::bind(&CrawlProcessor::url_fetch_error, this, std::placeholders::_1, std::placeholders::_2));
	
	add_async_client(url_index_async);
	add_async_client(crawl_frontend_async);
	
	set_timer_millis(1000, std::bind(&CrawlProcessor::print_stats, this));
	set_timer_millis(10 * 1000, std::bind(&CrawlProcessor::publish_stats, this));
	set_timer_millis(update_interval_ms, std::bind(&CrawlProcessor::check_queue, this));
	set_timer_millis(sync_interval * 1000, std::bind(&CrawlProcessor::check_all_urls, this));
	
	for(const auto& url : root_urls)
	{
		Url::Url parsed(url);
		parsed.defrag();
		if(parsed.scheme().empty()) {
			parsed.setScheme("https");
		}
		parsed.remove_default_port();
		parsed.strip();
		parsed.abspath();
		const auto link = parsed.str();
		try {
			url_index_async->get_value(link, std::bind(&CrawlProcessor::check_url, this, link, 0, std::placeholders::_1));
		}
		catch(const std::exception& ex) {
			log(WARN).out << "UrlIndex: " << ex.what();
		}
	}
	
	for(const auto& domain : domain_blacklist) {
		domain_map[domain].is_blacklisted = true;
	}
	
	check_all_urls();
	
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
			try {
				url_index_async->get_value(pair->key,
						std::bind(&CrawlProcessor::check_page_callback, this, url, std::placeholders::_1, index));
			}
			catch(const std::exception& ex) {
				log(WARN).out << "UrlIndex: " << ex.what();
			}
			return;
		}
	}
}

bool CrawlProcessor::enqueue(const std::string& url, int depth, int64_t load_time)
{
	if(depth > max_depth) {
		return false;
	}
	if(url.size() > max_url_length) {
		return false;
	}
	{
		auto url_iter = url_map.find(url);
		if(url_iter != url_map.end()) {
			url_t& entry = url_iter->second;
			if(depth < entry.depth) {
				const Url::Url parsed(url);
				domain_t& domain = domain_map[parsed.host()];
				for(auto it = domain.queue.lower_bound(entry.depth); it != domain.queue.upper_bound(entry.depth); ++it) {
					if(it->second == url) {
						domain.queue.erase(it);
						break;
					}
				}
				for(auto it = waiting.begin(); it != waiting.end(); ++it) {
					if(it->second == url) {
						waiting.erase(it);
						break;
					}
				}
			} else {
				return false;
			}
		}
	}
	
	const Url::Url parsed(url);
	const auto host = parsed.host();
	if(host.empty()) {
		return false;
	}
	
	domain_t& domain = domain_map[host];
	if(domain.is_blacklisted) {
		return false;
	}
	
	const auto delta = load_time - std::time(0);
	if(delta <= 0) {
		domain.queue.emplace(depth, url);
	} else if(delta < 2 * sync_interval) {
		waiting.emplace(load_time, url);
	} else {
		return false;
	}
	
	url_t& entry = url_map[url];
	entry.domain = host;
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
			const url_t& url = url_map[entry->second];
			domain_map[url.domain].queue.emplace(url.depth, entry->second);
			waiting.erase(entry);
		} else {
			break;
		}
	}
	
	std::multimap<std::pair<int, int64_t>, domain_t*> queue;
	
	for(auto& entry : domain_map) {
		if(!entry.second.queue.empty()) {
			queue.emplace(std::make_pair(entry.second.queue.begin()->first,
										entry.second.last_fetch_us), &entry.second);
		}
	}
	active_domains = queue.size();
	
	for(const auto& entry : queue)
	{
		if(pending_urls.size() >= max_num_pending) {
			break;
		}
		domain_t& domain = *entry.second;
		
		if(now_wall - domain.last_fetch_us > int64_t(60 * 1000 * 1000) / max_per_minute)
		{
			try {
				const auto iter = domain.queue.begin();
				const auto url_str = iter->second;
				url_t& url = url_map[url_str];
				
				url.request_id = crawl_frontend_async->fetch(url_str,
						std::bind(&CrawlProcessor::url_fetch_callback, this, url_str, std::placeholders::_1));
				
				pending_urls.emplace(url.request_id, url_str);
				
				domain.queue.erase(iter);
				domain.num_pending++;
				domain.last_fetch_us = now_wall;
				
				average_depth = url.depth * 0.01 + average_depth * 0.99;
			}
			catch(const std::exception& ex) {
				break;
			}
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
		depth = std::min(depth, index->depth);
		if(index->last_fetched > 0) {
			const int64_t load_time = index->last_fetched + int64_t(pow(depth + 1, reload_power) * reload_interval);
			enqueue(url, depth, load_time);
		} else {
			enqueue(url, depth);
		}
		if(depth < index->depth) {
			try {
				auto copy = vnx::clone(index);
				copy->depth = depth;
				url_index_async->store_value(url, copy);
			}
			catch(const std::exception& ex) {
				log(WARN).out << "UrlIndex: " << ex.what();
			}
		}
	} else {
		try {
			auto index = UrlIndex::create();
			index->depth = depth;
			index->first_seen = std::time(0);
			url_index_async->store_value(url, index);
		}
		catch(const std::exception& ex) {
			log(WARN).out << "UrlIndex: " << ex.what();
		}
		enqueue(url, depth);
	}
}

void CrawlProcessor::check_page_callback(	const std::string& url,
											std::shared_ptr<const Value> url_index_,
											std::shared_ptr<const PageIndex> page_index_)
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
		const Url::Url parsed(link);
		
		const int link_depth = depth + (parsed.host() != parent.host() ? jump_cost : 1);
		
		if(link_depth <= max_depth && link.size() <= max_url_length) {
			try {
				url_index_async->get_value(link, std::bind(&CrawlProcessor::check_url, this, link,
															link_depth, std::placeholders::_1));
			}
			catch(const std::exception& ex) {
				log(WARN).out << "UrlIndex: " << ex.what();
			}
		}
	}
	
}

CrawlProcessor::url_t CrawlProcessor::url_fetch_done(const std::string& url)
{
	const auto entry = url_map[url];
	reload_counter += entry.is_reload ? 1 : 0;
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
			url_index_async->get_value(url, std::bind(&CrawlProcessor::url_update_callback, this, url,
														copy, std::placeholders::_1));
		}
		catch(const std::exception& ex) {
			log(WARN).out << "UrlIndex: " << ex.what();
		}
		
		domain_t& domain = domain_map[entry.domain];
		if(index->is_fail) {
			domain.num_errors++;
		} else {
			domain.num_fetched++;
		}
	}
}

void CrawlProcessor::url_update_callback(	const std::string& url,
											std::shared_ptr<UrlIndex> fetched,
											std::shared_ptr<const Value> previous_)
{
	auto previous = std::dynamic_pointer_cast<const UrlIndex>(previous_);
	if(previous) {
		if(previous->depth >= 0) {
			fetched->depth = std::min(previous->depth, fetched->depth);
		}
		fetched->first_seen = previous->first_seen ? previous->first_seen : fetched->last_fetched;
		fetched->fetch_count = previous->fetch_count + 1;
	}
	try {
		url_index_async->store_value(url, fetched);
	}
	catch(const std::exception& ex) {
		log(WARN).out << "UrlIndex: " << ex.what();
	}
	if(fetched->is_fail) {
		error_counter++;
	} else {
		fetch_counter++;
	}
}

void CrawlProcessor::url_fetch_error(uint64_t request_id, const std::exception& ex)
{
	auto iter = pending_urls.find(request_id);
	if(iter != pending_urls.end())
	{
		const auto url = iter->second;
		const auto entry = url_fetch_done(url);		// after this iter will be invalid
		
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
					url_index_async->get_value(url, std::bind(&CrawlProcessor::url_update_callback, this, url,
																index, std::placeholders::_1));
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

void CrawlProcessor::publish_stats()
{
	auto stats = CrawlStats::create();
	stats->num_fetched = fetch_counter;
	stats->num_errors = error_counter;
	stats->num_reload = reload_counter;
	stats->num_active_domains = active_domains;
	stats->num_queued = url_map.size();
	stats->num_waiting = waiting.size();
	{
		std::vector<crawl_domain_stats_t> domains;
		for(const auto& entry : domain_map) {
			crawl_domain_stats_t dstats;
			dstats.host = entry.first;
			dstats.num_fetched = entry.second.num_fetched;
			dstats.num_errors = entry.second.num_errors;
			dstats.num_queued = entry.second.queue.size();
			domains.push_back(dstats);
		}
		{
			std::multimap<int64_t, crawl_domain_stats_t, std::greater<int64_t>> sorted;
			for(const auto& entry : domains) {
				sorted.emplace(entry.num_fetched, entry);
			}
			for(const auto& entry : sorted) {
				stats->most_fetched.push_back(entry.second);
				if(stats->most_fetched.size() >= 100) {
					break;
				}
			}
		}
		{
			std::multimap<int64_t, crawl_domain_stats_t, std::greater<int64_t>> sorted;
			for(const auto& entry : domains) {
				sorted.emplace(entry.num_queued, entry);
			}
			for(const auto& entry : sorted) {
				stats->most_queued.push_back(entry.second);
				if(stats->most_queued.size() >= 100) {
					break;
				}
			}
		}
	}
	publish(stats, output_crawl_stats);
}

void CrawlProcessor::print_stats()
{
	log(INFO).out << url_map.size() << " queued, " << waiting.size() << " waiting, "
			<< pending_urls.size() << " pending, " << fetch_counter << " fetched, "
			<< error_counter << " failed, " << reload_counter << " reload, "
			<< active_domains << " domains, " << average_depth << " avg. depth";
}


} // search
} // vnx
