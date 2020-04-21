/*
 * CrawlProcessor.cpp
 *
 *  Created on: Apr 4, 2020
 *      Author: mad
 */

#include <vnx/search/CrawlProcessor.h>
#include <vnx/search/Util.h>
#include <vnx/search/CrawlStats.hxx>

#include <unicode/unistr.h>
#include <unicode/brkiter.h>
using namespace icu;

#include <url.h>
#include <time.h>
#include <math.h>
#include <robots.h>
#include <algorithm>


namespace vnx {
namespace search {

CrawlProcessor::CrawlProcessor(const std::string& _vnx_name)
	:	CrawlProcessorBase(_vnx_name)
{
	input_url_index_sync = vnx_name + ".url_index.sync";
	
	protocols.push_back("http");
	protocols.push_back("https");
	
	matcher = std::make_shared<googlebot::RobotsMatcher>();
}

void CrawlProcessor::main()
{
	subscribe(input_text, 1000);				// need to block here since we are a bottleneck
	subscribe(input_url_index, 1000);			// publisher runs in a separate thread so we can block here
	subscribe(input_page_index, 1000);			// publisher runs in a separate thread so we can block here
	subscribe(input_url_index_sync, 100);				// sync runs in a separate thread so we can block here
	
	protocols = get_unique(protocols);
	
	url_index_async = std::make_shared<keyvalue::ServerAsyncClient>(url_index_server);
	page_index_async = std::make_shared<keyvalue::ServerAsyncClient>(page_index_server);
	page_content_async = std::make_shared<keyvalue::ServerAsyncClient>(page_content_server);
	crawl_frontend_async = std::make_shared<CrawlFrontendAsyncClient>(crawl_frontend_server);
	
	url_index_async->vnx_set_error_callback(std::bind(&CrawlProcessor::url_index_error, this, std::placeholders::_1, std::placeholders::_2));
	page_index_async->vnx_set_error_callback(std::bind(&CrawlProcessor::page_index_error, this, std::placeholders::_1, std::placeholders::_2));
	page_content_async->vnx_set_error_callback(std::bind(&CrawlProcessor::page_content_error, this, std::placeholders::_1, std::placeholders::_2));
	crawl_frontend_async->vnx_set_error_callback(std::bind(&CrawlProcessor::url_fetch_error, this, std::placeholders::_1, std::placeholders::_2));
	
	add_async_client(url_index_async);
	add_async_client(page_index_async);
	add_async_client(page_content_async);
	add_async_client(crawl_frontend_async);
	
	set_timer_millis(1000, std::bind(&CrawlProcessor::print_stats, this));
	set_timer_millis(10 * 1000, std::bind(&CrawlProcessor::publish_stats, this));
	set_timer_millis(check_interval_ms, std::bind(&CrawlProcessor::check_queue, this));
	set_timer_millis(update_interval_ms, std::bind(&CrawlProcessor::update_queue, this));
	set_timer_millis(sync_interval * 1000, std::bind(&CrawlProcessor::check_all_urls, this));
	
	for(const auto& url : root_urls)
	{
		const auto parsed = process_url(Url::Url(url));
		url_index_async->get_value(get_url_key(parsed),
				std::bind(&CrawlProcessor::check_url, this, parsed.str(), 0, std::placeholders::_1));
	}
	
	check_all_urls();
	
	Super::main();
}

void CrawlProcessor::handle(std::shared_ptr<const TextResponse> value)
{
	if(value->profile != profile) {
		return;
	}
	
	std::set<std::string> word_set;
	{
		const UnicodeString text = UnicodeString::fromUTF8(value->text);
		
		UErrorCode status = U_ZERO_ERROR;
		BreakIterator* bi = BreakIterator::createWordInstance(Locale::getUS(), status);
		bi->setText(text);
		
		auto pos = bi->first();
		auto begin = pos;
		while(pos != BreakIterator::DONE) {
			begin = pos;
			pos = bi->next();
			if(pos != BreakIterator::DONE) {
				if(bi->getRuleStatus() != UBRK_WORD_NONE) {
					UnicodeString word;
					text.extractBetween(begin, pos, word);
					word.toLower();
					std::string tmp;
					word.toUTF8String(tmp);
					word_set.insert(tmp);
				}
			}
		}
		delete bi;
    }
	
	const Url::Url parent(value->url);
	const auto parent_key = get_url_key(parent);
	
	auto index = PageIndex::create();
	index->title = value->title;
	index->last_modified = value->last_modified;
	
	for(const auto& word : word_set)
	{
		if(word.size() <= max_word_length) {
			index->words.push_back(word);
		}
	}
	
	for(const auto& link : value->links)
	{
		try {
			const auto parsed = process_link(Url::Url(link), parent);
			if(filter_url(parsed)) {
				const auto full_link = parsed.str();
				if(full_link.size() <= max_url_length) {
					index->links.push_back(full_link);
				}
			}
		} catch(...) {
			// ignore bad links
		}
	}
	
	for(const auto& link : value->images)
	{
		try {
			const auto parsed = process_link(Url::Url(link), parent);
			if(filter_url(parsed)) {
				const auto full_link = parsed.str();
				if(full_link.size() <= max_url_length) {
					index->images.push_back(full_link);
				}
			}
		} catch(...) {
			// ignore bad links
		}
	}
	
	index->links = get_unique(index->links);
	index->images = get_unique(index->images);
	
	page_index_async->store_value(parent_key, index);
	
	auto content = PageContent::create();
	content->text = value->text;
	
	page_content_async->store_value(parent_key, content);
	
	log(INFO).out << "Processed '" << parent_key << "': " << index->words.size() << " index words, "
			<< index->links.size() << " links, " << index->images.size() << " images";
}

void CrawlProcessor::handle(std::shared_ptr<const vnx::keyvalue::KeyValuePair> pair)
{
	const std::string url_key = pair->key.to_string_value();
	{
		auto index = std::dynamic_pointer_cast<const UrlIndex>(pair->value);
		if(index) {
			if(index->profile == profile) {
				check_url(index->scheme + ":" + url_key, index->depth, index);
			}
			return;
		}
	}
	{
		auto index = std::dynamic_pointer_cast<const PageIndex>(pair->value);
		if(index) {
			url_index_async->get_value(pair->key,
					std::bind(&CrawlProcessor::check_page_callback, this, url_key, std::placeholders::_1, index));
			return;
		}
	}
}

CrawlProcessor::domain_t& CrawlProcessor::get_domain(const std::string& host)
{
	domain_t& domain = domain_map[host];
	if(domain.host.empty()) {
		domain.host = host;
		for(const auto& entry : domain_blacklist) {
			if(host.size() >= entry.size() && host.substr(host.size() - entry.size()) == entry) {
				if(host.size() == entry.size() || host[host.size() - entry.size() - 1] == '.') {
					domain.is_blacklisted = true;
					blacklisted_domains++;
				}
			}
		}
	}
	return domain;
}

bool CrawlProcessor::filter_url(const Url::Url& parsed)
{
	if(std::find(protocols.begin(), protocols.end(), parsed.scheme()) == protocols.end()) {
		return true;	// just keep unknown protocols
	}
	auto& domain = get_domain(parsed.host());
	if(domain.is_blacklisted) {
		return false;
	}
	if(domain.robots_txt &&
		!matcher->OneAgentAllowedByRobots(domain.robots_txt->text, user_agent, parsed.str()))
	{
		domain.num_disallowed++;
		return false;
	}
	return true;
}

int CrawlProcessor::enqueue(const std::string& url, int depth, int64_t load_time)
{
	if(depth > max_depth) {
		return 0;
	}
	if(url.size() > max_url_length) {
		return 0;
	}
	const Url::Url parsed(url);
	const auto url_key = get_url_key(parsed);
	{
		auto iter = url_map.find(url_key);
		if(iter != url_map.end()) {
			if(depth >= iter->second.depth) {
				return 0;
			}
		}
	}
	if(std::find(protocols.begin(), protocols.end(), parsed.scheme()) == protocols.end()) {
		return 0;
	}
	const auto host = parsed.host();
	if(host.empty()) {
		return 0;
	}
	domain_t& domain = get_domain(host);
	if(domain.is_blacklisted) {
		return 0;
	}
	
	bool is_waiting = false;
	const auto delta = load_time - std::time(0);
	if(delta <= 0) {
		domain.queue.emplace(depth, url);
	} else if(delta < 2 * sync_interval) {
		waiting.emplace(load_time, url);
		is_waiting = true;
	} else {
		return 0;
	}
	
	url_t& entry = url_map[url_key];
	entry.domain = host;
	entry.depth = depth;
	entry.is_reload = load_time > 0;
	return is_waiting ? 2 : 1;
}

void CrawlProcessor::check_queue()
{
	queue_block_count = 0;
	pending_robots_txt = 0;
	const int64_t now_posix = std::time(0);
	const int64_t now_wall = vnx::get_wall_time_micros();
	
	for(const auto& entry : queue)
	{
		if(pending_urls.size() >= max_num_pending) {
			break;
		}
		queue_block_count++;
		
		domain_t& domain = *entry.second;
		const int64_t fetch_delta_ms = (now_wall - domain.last_fetch_us) / 1000;
		
		if(entry.first.first >= 0)
		{
			const std::string link_key = "//" + domain.host + "/robots.txt";
			const std::string link_url = "http:" + link_key;
			
			pending_robots_txt++;
			switch(domain.robots_state) {
				case ROBOTS_TXT_UNKNOWN:
					url_index_async->get_value(link_key,
							std::bind(&CrawlProcessor::check_url, this, link_url, -1, std::placeholders::_1));
					domain.robot_start_time = now_posix;
					domain.robots_state = ROBOTS_TXT_PENDING;
					continue;
				case ROBOTS_TXT_PENDING: {
					const bool is_timeout = now_posix - domain.robot_start_time > robots_txt_timeout;
					page_content_async->get_value(link_key, std::bind(&CrawlProcessor::robots_txt_callback, this, link_key,
									is_timeout ? ROBOTS_TXT_TIMEOUT : ROBOTS_TXT_PENDING, std::placeholders::_1));
					continue;
				}
				case ROBOTS_TXT_MISSING:
				case ROBOTS_TXT_TIMEOUT:
				case ROBOTS_TXT_FOUND:
					break;
				default:
					continue;
			}
			pending_robots_txt--;
		}
		
		if(fetch_delta_ms > int64_t(60 * 1000) / max_per_minute)
		{
			queue_block_count--;
			
			auto iter = domain.queue.begin();
			while(domain.robots_txt && iter != domain.queue.end()) {
				if(matcher->OneAgentAllowedByRobots(domain.robots_txt->text, user_agent, iter->second)) {
					break;
				}
				{
					auto index = UrlIndex::create();
					index->depth = iter->first;
					index->last_fetched = std::time(0);
					index->profile = profile;
					index->http_status = 403;	// fake HTTP Forbidden
					index->is_fail = true;
					url_index_async->get_value(get_url_key(iter->second),
							std::bind(&CrawlProcessor::url_update_callback, this, iter->second, index, std::placeholders::_1));
				}
				iter = domain.queue.erase(iter);
				domain.num_disallowed++;
			}
			if(iter == domain.queue.end()) {
				continue;
			}
			
			const auto url_str = iter->second;
			const auto url_key = get_url_key(url_str);
			const auto url_iter = url_map.find(url_key);
			if(url_iter == url_map.end()) {
				domain.queue.erase(iter);		// already fetched with lower depth
				continue;
			}
			url_t& url = url_iter->second;
			
			url.request_id = crawl_frontend_async->fetch(url_str, profile,
					std::bind(&CrawlProcessor::url_fetch_callback, this, url_str, std::placeholders::_1));
			
			pending_urls.emplace(url.request_id, url_str);
			
			domain.queue.erase(iter);
			domain.num_pending++;
			domain.last_fetch_us = now_wall;
			
			if(url.depth >= 0) {
				average_depth = url.depth * 0.01 + average_depth * 0.99;
			}
		}
	}
}

void CrawlProcessor::update_queue()
{
	const int64_t now_posix = std::time(0);
	while(!waiting.empty())
	{
		const auto entry = waiting.begin();
		if(entry->first <= now_posix) {
			auto url_iter = url_map.find(get_url_key(entry->second));
			if(url_iter != url_map.end()) {
				const url_t& url = url_iter->second;
				get_domain(url.domain).queue.emplace(url.depth, entry->second);
			}
			waiting.erase(entry);
		} else {
			break;
		}
	}
	queue.clear();
	for(auto& entry : domain_map) {
		auto& domain = entry.second;
		if(!domain.queue.empty()) {
			const auto key = std::make_pair(domain.queue.begin()->first, domain.last_fetch_us);
			limited_emplace(queue, key, &domain, max_queue_size);
		}
	}
}

void CrawlProcessor::check_all_urls()
{
	url_index_async->sync_all(input_url_index_sync);
}

void CrawlProcessor::check_url(const std::string& url, int depth, std::shared_ptr<const Value> index_)
{
	const Url::Url parsed(url);
	const auto url_key = get_url_key(parsed);
	const bool is_robots = is_robots_txt(parsed);
	
	auto index = std::dynamic_pointer_cast<const UrlIndex>(index_);
	if(index) {
		if(is_robots) {
			if(index->last_fetched > 0) {
				int64_t load_time = 0;
				if(index->http_status < 0) {
					load_time = index->last_fetched + int64_t(pow(index->fetch_count, 2) * 300);
				} else {
					load_time = index->last_fetched + 2678400;
				}
				const int is_queued = enqueue(url, depth, index->last_fetched + 2678400);
				
				auto& domain = get_domain(parsed.host());
				if(!is_queued) {
					if(index->is_fail) {
						if(domain.robots_state != ROBOTS_TXT_MISSING) {
							missing_robots_txt++;
						}
						domain.robots_state = ROBOTS_TXT_MISSING;
						domain.robots_txt = 0;
					} else {
						if(domain.robots_state != ROBOTS_TXT_PENDING) {
							page_content_async->get_value(url_key,
									std::bind(&CrawlProcessor::robots_txt_callback, this, url_key, ROBOTS_TXT_MISSING, std::placeholders::_1));
						}
					}
				} else if(is_queued == 1) {
					domain.robots_state = ROBOTS_TXT_PENDING;
				}
			} else {
				enqueue(url, depth);
			}
		} else {
			depth = std::min(depth, index->depth);
			if(index->last_fetched > 0) {
				int64_t load_time = 0;
				if(index->http_status < 0) {
					load_time = index->last_fetched + int64_t(pow(index->fetch_count, 2) * error_reload_interval);
				} else {
					load_time = index->last_fetched + int64_t(pow(depth + 1, reload_power) * reload_interval);
				}
				enqueue(url, depth, load_time);
			} else {
				enqueue(url, depth);
			}
			if(depth < index->depth) {
				auto copy = vnx::clone(index);
				copy->scheme = parsed.scheme();
				copy->depth = depth;
				url_index_async->store_value(url_key, copy);
			}
		}
	} else {
		auto index = UrlIndex::create();
		index->scheme = parsed.scheme();
		index->depth = depth;
		index->first_seen = std::time(0);
		index->profile = profile;
		url_index_async->store_value(url_key, index);
		enqueue(url, depth);
	}
}

void CrawlProcessor::check_page_callback(	const std::string& url_key,
											std::shared_ptr<const Value> url_index_,
											std::shared_ptr<const PageIndex> page_index_)
{
	auto index = std::dynamic_pointer_cast<const UrlIndex>(url_index_);
	if(index) {
		if(index->profile == profile) {
			check_page(url_key, index->depth, page_index_);
		}
	}
}

void CrawlProcessor::check_page(const std::string& url_key, int depth, std::shared_ptr<const PageIndex> index)
{
	if(depth < 0) {
		return;		// don't follow links in this case
	}
	const Url::Url parent(url_key);
	
	for(const auto& link : index->links)
	{
		const Url::Url parsed(link);
		if(std::find(protocols.begin(), protocols.end(), parsed.scheme()) == protocols.end()) {
			continue;
		}
		const auto link_depth = depth + (parsed.host() != parent.host() ? jump_cost : 1);
		
		if(link_depth <= max_depth)
		{
			url_index_async->get_value(get_url_key(parsed),
					std::bind(&CrawlProcessor::check_url, this, link, link_depth, std::placeholders::_1));
		}
	}
}

CrawlProcessor::url_t CrawlProcessor::url_fetch_done(const std::string& url_key)
{
	const auto entry = url_map[url_key];
	reload_counter += entry.is_reload ? 1 : 0;
	get_domain(entry.domain).num_pending--;
	pending_urls.erase(entry.request_id);
	url_map.erase(url_key);
	return entry;
}

void CrawlProcessor::url_fetch_callback(const std::string& url, std::shared_ptr<const UrlIndex> index)
{
	const Url::Url parsed(url);
	const auto url_key = get_url_key(parsed);
	const auto entry = url_fetch_done(url_key);
	
	if(index) {
		auto copy = vnx::clone(index);
		copy->scheme = parsed.scheme();
		copy->depth = entry.depth;
		
		url_index_async->get_value(url_key,
				std::bind(&CrawlProcessor::url_update_callback, this, url_key, copy, std::placeholders::_1));
		
		domain_t& domain = get_domain(entry.domain);
		if(index->is_fail) {
			domain.num_errors++;
		} else {
			domain.num_fetched++;
		}
	}
}

void CrawlProcessor::url_update_callback(	const std::string& url_key,
											std::shared_ptr<UrlIndex> fetched,
											std::shared_ptr<const Value> previous_)
{
	auto previous = std::dynamic_pointer_cast<const UrlIndex>(previous_);
	if(previous) {
		fetched->first_seen = previous->first_seen ? previous->first_seen : fetched->last_fetched;
		fetched->fetch_count = previous->fetch_count + 1;
	}
	url_index_async->store_value(url_key, fetched);
	
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
		const Url::Url parsed(url);
		const auto url_key = get_url_key(parsed);
		const auto entry = url_fetch_done(url_key);		// after this iter will be invalid
		
		log(WARN).out << "fetch('" << url << "'): " << ex.what();
		
		auto vnx_except = dynamic_cast<const vnx::exception*>(&ex);
		if(vnx_except) {
			if(std::dynamic_pointer_cast<const NoSuchService>(vnx_except->value())) {
				enqueue(url, entry.depth);
			} else {
				auto index = UrlIndex::create();
				index->scheme = parsed.scheme();
				index->depth = entry.depth;
				index->profile = profile;
				index->last_fetched = std::time(0);
				index->is_fail = true;
				url_index_async->get_value(url_key,
						std::bind(&CrawlProcessor::url_update_callback, this, url_key, index, std::placeholders::_1));
			}
		}
	}
}

void CrawlProcessor::robots_txt_callback(	const std::string& url_key,
											robots_txt_state_e missing_state,
											std::shared_ptr<const Value> value)
{
	const Url::Url parsed(url_key);
	domain_t& domain = get_domain(parsed.host());
	
	auto content = std::dynamic_pointer_cast<const PageContent>(value);
	if(content) {
		if(!domain.robots_txt) {
			found_robots_txt++;
		}
		domain.robots_txt = content;
	}
	if(domain.robots_txt) {
		domain.robots_state = ROBOTS_TXT_FOUND;
	} else {
		if(missing_state == ROBOTS_TXT_TIMEOUT) {
			timed_out_robots_txt++;
		}
		domain.robots_state = missing_state;
	}
}

void CrawlProcessor::url_index_error(uint64_t request_id, const std::exception& ex)
{
	log(WARN).out << "UrlIndex: " << ex.what();
}

void CrawlProcessor::page_index_error(uint64_t request_id, const std::exception& ex)
{
	log(WARN).out << "PageIndex: " << ex.what();
}

void CrawlProcessor::page_content_error(uint64_t request_id, const std::exception& ex)
{
	log(WARN).out << "PageContent: " << ex.what();
}

std::shared_ptr<const CrawlStats> CrawlProcessor::get_stats(const int32_t& limit) const
{
	auto stats = CrawlStats::create();
	stats->num_fetched = fetch_counter;
	stats->num_errors = error_counter;
	stats->num_reload = reload_counter;
	stats->num_domains = domain_map.size();
	stats->num_queued = url_map.size();
	stats->num_waiting = waiting.size();
	{
		std::vector<crawl_domain_stats_t> domains;
		domains.reserve(domain_map.size());
		for(const auto& entry : domain_map) {
			crawl_domain_stats_t dstats;
			dstats.host = entry.first;
			dstats.num_fetched = entry.second.num_fetched;
			dstats.num_errors = entry.second.num_errors;
			dstats.num_disallowed = entry.second.num_disallowed;
			dstats.num_queued = entry.second.queue.size();
			dstats.has_robots_txt = entry.second.robots_state == ROBOTS_TXT_FOUND;
			domains.push_back(dstats);
		}
		{
			std::multimap<int64_t, crawl_domain_stats_t, std::greater<int64_t>> sorted;
			for(const auto& entry : domains) {
				limited_emplace(sorted, entry.num_fetched, entry, size_t(limit));
			}
			for(const auto& entry : sorted) {
				stats->most_fetched.push_back(entry.second);
				if(stats->most_fetched.size() >= size_t(limit)) {
					break;
				}
			}
		}
		{
			std::multimap<int64_t, crawl_domain_stats_t, std::greater<int64_t>> sorted;
			for(const auto& entry : domains) {
				limited_emplace(sorted, entry.num_queued, entry, size_t(limit));
			}
			for(const auto& entry : sorted) {
				stats->most_queued.push_back(entry.second);
				if(stats->most_queued.size() >= size_t(limit)) {
					break;
				}
			}
		}
	}
	return stats;
}

void CrawlProcessor::publish_stats()
{
	publish(get_stats(100), output_crawl_stats);
}

void CrawlProcessor::print_stats()
{
	log(INFO).out << url_map.size() << " queued, " << waiting.size() << " waiting, "
			<< pending_urls.size() << " pending, " << queue_block_count << " blocking, "
			<< fetch_counter << " fetched, " << error_counter << " failed, "
			<< reload_counter << " reload, " << domain_map.size()
			<< " (-" << blacklisted_domains << ") domains, " << average_depth << " depth";
	log(INFO).out << "Robots: " << pending_robots_txt << " pending, "
			<< missing_robots_txt << " missing, " << timed_out_robots_txt << " timeout, "
			<< found_robots_txt << " found";
}


} // search
} // vnx
