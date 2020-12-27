/*
 * CrawlProcessor.cpp
 *
 *  Created on: Apr 4, 2020
 *      Author: mad
 */

#include <vnx/search/CrawlProcessor.h>
#include <vnx/search/Util.h>

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
	input_url_index_sync = vnx_name + ".url_index.sync_" + std::to_string(vnx::rand64());
	input_page_index_sync = vnx_name + ".page_index.sync_" + std::to_string(vnx::rand64());
	
	protocols.push_back("http");
	protocols.push_back("https");
	
	matcher = std::make_shared<googlebot::RobotsMatcher>();
}

void CrawlProcessor::init()
{
	vnx::open_pipe(vnx_name, this, 100);
}

void CrawlProcessor::main()
{
	subscribe(input_url_index, 100);
	subscribe(input_url_index_sync, 100, 100);
	subscribe(input_page_index_sync, 100, 10);
	
	protocols = unique(protocols);
	domain_blacklist = unique(domain_blacklist);
	path_blacklist = unique(path_blacklist);
	regex_blacklist = unique(regex_blacklist);
	
	for(const auto& entry : regex_blacklist) {
		regex_blacklist_.emplace_back(entry);
	}
	
	url_index_async = std::make_shared<keyvalue::ServerAsyncClient>(url_index_server);
	page_index_async = std::make_shared<keyvalue::ServerAsyncClient>(page_index_server);
	page_content_async = std::make_shared<keyvalue::ServerAsyncClient>(page_content_server);
	crawl_frontend_async = std::make_shared<CrawlFrontendAsyncClient>(crawl_frontend_server);
	
	url_index_async->vnx_set_error_callback(std::bind(&CrawlProcessor::url_index_error, this, std::placeholders::_1, std::placeholders::_2));
	page_index_async->vnx_set_error_callback(std::bind(&CrawlProcessor::page_index_error, this, std::placeholders::_1, std::placeholders::_2));
	page_content_async->vnx_set_error_callback(std::bind(&CrawlProcessor::page_content_error, this, std::placeholders::_1, std::placeholders::_2));
	
	add_async_client(url_index_async);
	add_async_client(page_index_async);
	add_async_client(page_content_async);
	add_async_client(crawl_frontend_async);
	
	set_timer_millis(1000, std::bind(&CrawlProcessor::print_stats, this));
	set_timer_millis(10 * 1000, std::bind(&CrawlProcessor::publish_stats, this));
	set_timer_millis(check_interval_ms, std::bind(&CrawlProcessor::check_queues, this));
	set_timer_millis((reload_interval * 1000) / 10, std::bind(&CrawlProcessor::check_root_urls, this));
	
	{
		std::vector<std::string> tmp;
		for(const auto& url : root_urls) {
			try {
				if(filter_url(Url::Url(url))) {
					tmp.push_back(url);
				}
			} catch(...) {
				// ignore
			}
		}
		root_urls = tmp;
	}
	check_root_urls();
	
	if(inititial_sync) {
		check_all_urls();
	} else {
		set_timeout_millis(sync_interval * 1000, std::bind(&CrawlProcessor::check_all_urls, this));
	}
	if(do_reprocess) {
		page_index_async->sync_all(input_page_index_sync);
	}
	
	work_threads = std::make_shared<ThreadPool>(num_threads, 100);
	
	Super::main();
	
	work_threads->close();
}

void CrawlProcessor::process_new(std::shared_ptr<process_job_t> job)
{
	const auto& url = job->response->url;
	const Url::Url parsed(url);
	if(url.size() > max_url_length || !filter_url(parsed)) {
		return;
	}
	auto response = job->response;
	
	auto content = PageContent::create();
	content->text = response->text;
	
	auto index = PageIndex::create();
	index->title = response->title;
	index->last_modified = response->last_modified;
	
	auto* domain = find_domain(parsed.host());
	if(domain) {
		job->robots = domain->robots_txt;
	}
	job->index = index;
	job->content = response->text;
	job->base_url = response->base_url;
	job->links = response->links;
	job->images = response->images;
	
	if(is_robots_txt(parsed)) {
		auto entry = keyvalue::Entry::create();
		entry->key = Variant(job->url_key);
		entry->value = content;
		robots_txt_callback(job->url_key, robots_txt_state_e::ROBOTS_TXT_MISSING, entry);
		page_content_async->store_value(Variant(job->url_key), content);
	} else {
		page_content_async->store_value(Variant(job->url_key), content,
			[this, job]() {
				work_threads->add_task(std::bind(&CrawlProcessor::process_task, this, job));
			});
	}
}

void CrawlProcessor::process_task(std::shared_ptr<process_job_t> job) noexcept
{
	auto index = job->index;
	index->version = index_version;
	
	std::map<std::string, size_t> word_set;
	try {
		const auto words = parse_text(job->content);
		for(const auto& word : words) {
			word_set[word]++;
		}
	} catch(const std::exception& ex) {
		log(WARN) << ex.what();
	}
	
	const Url::Url parent(job->url_key);
	const Url::Url base(job->base_url);
	googlebot::RobotsMatcher matcher;
	
	for(const auto& word : word_set)
	{
		if(word.first.size() <= max_word_length) {
			index->words.emplace_back(word.first, std::min(word.second, size_t(0xFFFF)));
			index->word_count += word.second;
		}
	}
	
	std::map<std::string, page_link_t> links;
	for(const auto& link : job->links) {
		try {
			std::string full_url;
			if(process_link(Url::Url(link.url), base, parent, matcher, job->robots, full_url)) {
				auto& dst = links[full_url];
				dst.url = full_url;
				dst.text += (dst.text.empty() ? "" : " | ") + link.text;
			}
		} catch(...) {
			// ignore bad links
		}
	}
	for(auto& entry : links) {
		auto& link = entry.second;
		link.words = parse_text(link.text);
		index->links.push_back(link);
	}
	
	std::map<std::string, image_link_t> images;
	for(const auto& link : job->images) {
		try {
			std::string full_url;
			if(process_link(Url::Url(link.url), base, parent, matcher, job->robots, full_url)) {
				auto& dst = images[full_url];
				dst.url = full_url;
				dst.width = std::max(dst.width, link.width);
				dst.height = std::max(dst.height, link.height);
				dst.text += (dst.text.empty() ? "" : " | ") + link.text;
			}
		} catch(...) {
			// ignore bad links
		}
	}
	for(auto& entry : images) {
		auto& link = entry.second;
		link.words = parse_text(link.text);
		index->images.push_back(link);
	}
	
	add_task(std::bind(&CrawlProcessor::process_callback, this, job));
}

bool CrawlProcessor::process_link(	const Url::Url& url,
									const Url::Url& base,
									const Url::Url& parent,
									googlebot::RobotsMatcher& matcher,
									std::shared_ptr<const PageContent> robots,
									std::string& result) const
{
	// this function is thread-safe
	const auto parsed = vnx::search::process_link(url, base);
	const auto full_link = parsed.str();
	if(full_link.size() <= max_url_length && filter_url(parsed))
	{
		if(parsed.host() != parent.host() || !robots
			|| matcher.OneAgentAllowedByRobots(robots->text, user_agent, full_link))
		{
			result = full_link;
			return true;
		}
	}
	return false;
}

void CrawlProcessor::process_callback(std::shared_ptr<process_job_t> job)
{
	page_index_async->store_value(Variant(job->url_key), job->index,
		[this, job]() {
			if(job->is_reprocess) {
				reproc_counter++;
			} else {
				check_page(job->depth, job->url_key, job->index);
			}
		});
	log(INFO).out << "Processed '" << job->url_key << "': " << job->index->words.size() << " index words, "
			<< job->index->links.size() << " links, " << job->index->images.size() << " images";
}

void CrawlProcessor::handle(std::shared_ptr<const vnx::keyvalue::SyncUpdate> entry)
{
	const std::string url_key = entry->key.to_string_value();
	{
		auto index = std::dynamic_pointer_cast<const UrlIndex>(entry->value);
		if(index) {
			if(index->depth >= 0) {
				const auto url = index->scheme + ":" + url_key;
				const Url::Url parsed(url);
				if(filter_url(parsed)) {
					if(!index->last_fetched) {
						check_url(parsed, index->depth, entry);
					}
				} else {
					delete_page(url_key);
				}
			}
			return;
		}
	}
	{
		auto index = std::dynamic_pointer_cast<const PageIndex>(entry->value);
		if(index) {
			if(do_reprocess) {
				const Url::Url parsed(url_key);
				if(is_robots_txt(parsed)) {
					page_index_async->delete_value(Variant(url_key));
				} else if(filter_url(parsed)) {
					if(index->version < index_version) {
						page_content_async->get_value(Variant(url_key),
								std::bind(&CrawlProcessor::reproc_page_callback, this, url_key, std::placeholders::_1, index));
					}
				} else {
					delete_page(url_key);
				}
			}
			return;
		}
	}
}

void CrawlProcessor::delete_page(const std::string& url_key)
{
	url_index_async->delete_value(Variant(url_key));
	page_index_async->delete_value(Variant(url_key));
	page_content_async->delete_value(Variant(url_key));
	delete_counter++;
	log(INFO).out << "Deleted '" << url_key << "'";
}

CrawlProcessor::domain_t& CrawlProcessor::get_domain(const std::string& host)
{
	auto& domain = domain_map[host];
	if(domain.host.empty()) {
		domain.host = host;
	}
	return domain;
}

CrawlProcessor::domain_t* CrawlProcessor::find_domain(const std::string& host)
{
	const auto iter = domain_map.find(host);
	if(iter != domain_map.end()) {
		return &iter->second;
	}
	return 0;
}

bool CrawlProcessor::filter_url(const Url::Url& parsed) const
{
	if(!parsed.scheme().empty()) {
		if(std::find(protocols.begin(), protocols.end(), parsed.scheme()) == protocols.end()) {
			return true;	// just keep unknown protocols
		}
	}
	const auto& host = parsed.host();
	if(host.empty()) {
		return true;		// just keep unknown hosts
	}
	for(const auto& entry : domain_blacklist) {
		if(host.size() >= entry.size() && host.substr(host.size() - entry.size()) == entry) {
			if(host.size() == entry.size() || host[host.size() - entry.size() - 1] == '.') {
				return false;
			}
		}
	}
	const auto url_key = get_url_key(parsed);
	for(const auto& entry : path_blacklist) {
		if(url_key.compare(0, entry.size(), entry) == 0) {
			return false;
		}
	}
	for(const auto& entry : regex_blacklist_) {
		if(std::regex_search(url_key, entry)) {
			return false;
		}
	}
	return true;
}

int CrawlProcessor::enqueue(const std::string& url, const int depth, int64_t load_time)
{
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
	
	const auto& host = parsed.host();
	if(host.empty()) {
		return 0;
	}
	
	const auto delta = load_time - std::time(0);
	if(delta <= 0) {
		auto& domain = get_domain(host);
		domain.queue.emplace(depth, url);
	} else {
		return 0;
	}
	
	url_t& entry = url_map[url_key];
	entry.domain = host;
	entry.depth = depth;
	entry.is_reload = load_time > 0;
	return 1;
}

void CrawlProcessor::check_queues()
{
	check_fetch_queue();
}

void CrawlProcessor::check_fetch_queue()
{
	queue_block_count = 0;
	pending_robots_txt = 0;
	const int64_t now_posix = std::time(0);
	const int64_t now_wall_us = vnx::get_wall_time_micros();
	std::multimap<std::pair<int, int64_t>, domain_t*> queue;
	
	for(auto iter = domain_map.begin(); iter != domain_map.end();)
	{
		auto& domain = iter->second;
		if(!domain.queue.empty()) {
			const auto key = std::make_pair(domain.queue.begin()->first, domain.last_fetch_us);
			limited_emplace(queue, key, &domain, max_queue_size);
			iter++;
		} else if(domain.num_pending <= 0) {
			iter = domain_map.erase(iter);
		} else {
			iter++;
		}
	}
	active_domains = domain_map.size();
	
	for(const auto& entry : queue)
	{
		if(crawl_frontend_async->vnx_get_num_pending() >= max_num_pending) {
			break;
		}
		queue_block_count++;
		
		domain_t& domain = *entry.second;
		const int64_t fetch_delta_ms = (now_wall_us - domain.last_fetch_us) / 1000;
		
		if(entry.first.first >= 0)
		{
			const std::string link_key = "//" + domain.host + "/robots.txt";
			const std::string link_url = "http:" + link_key;
			
			pending_robots_txt++;
			switch(domain.robots_state) {
				case ROBOTS_TXT_UNKNOWN:
					url_index_async->get_value(Variant(link_key),
							std::bind(&CrawlProcessor::check_url, this, Url::Url(link_url), -1, std::placeholders::_1));
					domain.robot_start_time = now_posix;
					domain.robots_state = ROBOTS_TXT_PENDING;
					continue;
				case ROBOTS_TXT_PENDING: {
					const bool is_timeout = now_posix - domain.robot_start_time > robots_timeout;
					page_content_async->get_value(Variant(link_key), std::bind(&CrawlProcessor::robots_txt_callback, this, link_key,
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
				delete_page(get_url_key(iter->second));
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
			
			crawl_frontend_async->fetch(url_str,
					std::bind(&CrawlProcessor::url_fetch_callback, this, url_str, std::placeholders::_1),
					std::bind(&CrawlProcessor::url_fetch_error, this, url_str, std::placeholders::_1));
			
			domain.queue.erase(iter);
			domain.num_pending++;
			domain.last_fetch_us = now_wall_us;
			
			if(url.depth >= 0) {
				average_depth = url.depth * 0.01 + average_depth * 0.99;
			}
		}
	}
}

void CrawlProcessor::check_all_urls()
{
	url_index_async->sync_all(input_url_index_sync);
}

void CrawlProcessor::check_root_urls()
{
	for(const auto& url : root_urls) {
		const auto parsed = process_url(Url::Url(url));
		url_index_async->get_value(Variant(get_url_key(parsed)),
				std::bind(&CrawlProcessor::check_url, this, parsed, 0, std::placeholders::_1));
	}
}

void CrawlProcessor::check_url(const Url::Url& parsed, const int depth, std::shared_ptr<const keyvalue::Entry> entry)
{
	const auto url = parsed.str();
	const auto url_key = get_url_key(parsed);
	const bool is_robots = is_robots_txt(parsed);
	
	auto index = std::dynamic_pointer_cast<const UrlIndex>(entry->value);
	if(index) {
		if(is_robots) {
			if(index->last_fetched > 0) {
				int64_t load_delay = robots_reload_interval;
				if(index->http_status < 0) {
					load_delay = std::min(int64_t(pow(index->fetch_count, reload_power) * error_reload_interval), load_delay);
				}
				const auto is_queued = enqueue(url, depth, index->last_fetched + load_delay);
				
				auto* domain = find_domain(parsed.host());
				if(domain) {
					if(is_queued != 1) {
						if(index->is_fail) {
							if(domain->robots_state != ROBOTS_TXT_MISSING) {
								missing_robots_txt++;
							}
							domain->robots_state = ROBOTS_TXT_MISSING;
							domain->robots_txt = 0;
						} else {
							page_content_async->get_value(Variant(url_key),
									std::bind(&CrawlProcessor::robots_txt_callback, this, url_key, ROBOTS_TXT_MISSING, std::placeholders::_1));
						}
					} else {
						domain->robots_state = ROBOTS_TXT_PENDING;
					}
				}
			} else {
				enqueue(url, depth);
			}
		} else if(depth >= 0) {
			if(index->last_fetched > 0) {
				int64_t load_delay = pow(depth + 1, reload_power) * reload_interval;
				if(index->http_status < 0) {
					load_delay = std::min(int64_t(pow(index->fetch_count, reload_power) * error_reload_interval), load_delay);
				}
				enqueue(url, depth, index->last_fetched + load_delay);
			} else {
				enqueue(url, depth);
			}
		} else {
			delete_page(url_key);
		}
	} else {
		auto index = UrlIndex::create();
		index->scheme = parsed.scheme();
		index->depth = depth;
		index->first_seen = std::time(0);
		url_index_async->store_value(Variant(url_key), index);
		enqueue(url, depth);
	}
}

void CrawlProcessor::reproc_page_callback(	const std::string& url_key,
											std::shared_ptr<const keyvalue::Entry> entry,
											std::shared_ptr<const PageIndex> index)
{
	auto content = std::dynamic_pointer_cast<const PageContent>(entry->value);
	if(content) {
		reproc_page(url_key, index, content);
	}
}

void CrawlProcessor::check_page(	int depth,
									const std::string& url_key,
									std::shared_ptr<const PageIndex> index)
{
	if(depth < 0) {
		return;		// don't follow links in this case
	}
	const Url::Url parent(url_key);
	
	for(const auto& link : index->links)
	{
		const Url::Url parsed(link.url);
		if(std::find(protocols.begin(), protocols.end(), parsed.scheme()) == protocols.end()) {
			continue;
		}
		const auto link_depth = depth + (parsed.host() != parent.host() ? jump_cost : 1);
		
		if(link_depth <= max_depth)
		{
			const auto url_key = get_url_key(parsed);
			url_index_async->get_value(Variant(url_key),
					std::bind(&CrawlProcessor::check_url, this, parsed, link_depth, std::placeholders::_1));
		}
	}
}

void CrawlProcessor::reproc_page(	const std::string& url_key,
									std::shared_ptr<const PageIndex> index,
									std::shared_ptr<const PageContent> content)
{
	auto new_index = vnx::clone(index);
	new_index->words.clear();
	new_index->links.clear();
	new_index->images.clear();
	
	auto job = std::make_shared<process_job_t>();
	job->is_reprocess = true;
	job->url_key = url_key;
	job->index = new_index;
	job->content = content->text;
	job->links = index->links;
	job->images = index->images;
	
	work_threads->add_task(std::bind(&CrawlProcessor::process_task, this, job));
}

CrawlProcessor::url_t CrawlProcessor::url_fetch_done(const std::string& url_key, bool is_fail)
{
	const auto iter = url_map.find(url_key);
	if(iter == url_map.end()) {
		throw std::logic_error("url_fetch_done(): invalid url_key: '" + url_key + "'");
	}
	const auto entry = iter->second;
	auto* domain = find_domain(entry.domain);
	if(domain) {
		domain->num_pending--;
		if(is_fail) {
			domain->num_errors++;
		} else {
			domain->num_fetched++;
		}
	}
	if(is_fail) {
		error_counter++;
	} else {
		fetch_counter++;
	}
	reload_counter += entry.is_reload ? 1 : 0;
	url_map.erase(iter);
	return entry;
}

void CrawlProcessor::url_fetch_callback(const std::string& url, std::shared_ptr<const FetchResult> result)
{
	const Url::Url parsed(url);
	const auto org_url_key = get_url_key(parsed);
	const auto entry = url_fetch_done(org_url_key, result->is_fail);
	
	if(result->response)
	{
		auto job = std::make_shared<process_job_t>();
		job->depth = entry.depth;
		job->url_key = get_url_key(result->response->url);
		job->result = result;
		job->response = result->response;
		
		url_index_async->get_value(Variant(job->url_key),
				std::bind(&CrawlProcessor::check_result_callback, this, job, std::placeholders::_1));
	}
	url_update(org_url_key, parsed.scheme(), entry.depth, *result);
	
	if(!result->redirect.empty())
	{
		const Url::Url parsed_redir(result->redirect);
		const auto new_url_key = get_url_key(parsed_redir);
		
		if(new_url_key != org_url_key)
		{
			page_index_async->delete_value(Variant(org_url_key));
			page_content_async->delete_value(Variant(org_url_key));
			log(INFO).out << "Deleted obsolete '" << org_url_key << "'";
			
			if(		result->redirect.size() <= max_url_length
				&&	filter_url(parsed_redir))
			{
				UrlInfo info = *result;
				info.redirect.clear();
				url_update(new_url_key, parsed_redir.scheme(), entry.depth, info);
			}
		}
	}
}

void CrawlProcessor::check_result_callback(	std::shared_ptr<process_job_t> job,
											std::shared_ptr<const keyvalue::Entry> entry)
{
	auto previous = std::dynamic_pointer_cast<const UrlIndex>(entry->value);
	if(!previous || job->result->last_fetched > previous->last_fetched + reload_interval / 2)
	{
		process_new(job);
	}
}

void CrawlProcessor::url_update(	const std::string& url_key,
									const std::string& new_scheme,
									const int new_depth,
									const UrlInfo& info)
{
	auto job = std::make_shared<url_update_job_t>();
	job->new_scheme = new_scheme;
	job->new_depth = new_depth;
	job->info = info;
	
	url_index_async->get_value_locked(Variant(url_key), lock_timeout * 1000,
			std::bind(&CrawlProcessor::url_update_callback, this, job, std::placeholders::_1));
}

void CrawlProcessor::url_update_callback(	std::shared_ptr<url_update_job_t> job,
											std::shared_ptr<const keyvalue::Entry> entry)
{
	std::shared_ptr<UrlIndex> index;
	auto previous = std::dynamic_pointer_cast<const UrlIndex>(entry->value);
	if(previous) {
		index = vnx::clone(previous);
	} else {
		index = UrlIndex::create();
	}
	index->UrlInfo::operator=(job->info);
	index->scheme = job->new_scheme;
	index->depth = job->new_depth;
	
	if(previous) {
		index->first_seen = previous->first_seen;
		index->fetch_count = previous->fetch_count + 1;
	} else {
		index->first_seen = index->last_fetched;
		index->fetch_count = 1;
	}
	url_index_async->store_value_delay(entry->key, index, commit_delay * 1000);
}

void CrawlProcessor::url_fetch_error(const std::string& url, const std::exception& ex)
{
	const Url::Url parsed(url);
	const auto url_key = get_url_key(parsed);
	const auto entry = url_fetch_done(url_key, true);
	
	log(WARN).out << "fetch('" << url << "'): " << ex.what();
	
	auto vnx_except = dynamic_cast<const vnx::exception*>(&ex);
	if(vnx_except) {
		if(std::dynamic_pointer_cast<const NoSuchService>(vnx_except->value())) {
			enqueue(url, entry.depth);
		} else {
			UrlInfo info;
			info.last_fetched = std::time(0);
			info.is_fail = true;
			url_update(url_key, parsed.scheme(), entry.depth, info);
		}
	}
}

void CrawlProcessor::robots_txt_callback(	const std::string& url_key,
											robots_txt_state_e missing_state,
											std::shared_ptr<const keyvalue::Entry> entry)
{
	const Url::Url parsed(url_key);
	auto* domain = find_domain(parsed.host());
	if(!domain) {
		return;
	}
	auto content = std::dynamic_pointer_cast<const PageContent>(entry->value);
	if(content) {
		if(!domain->robots_txt) {
			found_robots_txt++;
		}
		domain->robots_txt = content;
	}
	if(domain->robots_txt) {
		domain->robots_state = ROBOTS_TXT_FOUND;
	} else {
		if(missing_state == ROBOTS_TXT_TIMEOUT) {
			timed_out_robots_txt++;
		}
		domain->robots_state = missing_state;
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

Object CrawlProcessor::get_stats(const int32_t& limit) const
{
	Object stats;
	stats["num_fetched"] = fetch_counter;
	stats["num_errors"] = error_counter;
	stats["num_reload"] = reload_counter;
	stats["num_domains"] = domain_map.size();
	stats["num_queued"] = url_map.size();
	{
		std::multimap<int64_t, Object, std::greater<int64_t>> sorted_fetched;
		std::multimap<int64_t, Object, std::greater<int64_t>> sorted_queued;
		for(const auto& entry : domain_map) {
			Object domain;
			domain["host"] = entry.first;
			domain["num_fetched"] = entry.second.num_fetched;
			domain["num_errors"] = entry.second.num_errors;
			domain["num_disallowed"] = entry.second.num_disallowed;
			domain["num_queued"] = entry.second.queue.size();
			domain["has_robots_txt"] = entry.second.robots_state == ROBOTS_TXT_FOUND;
			limited_emplace(sorted_fetched, entry.second.num_fetched, domain, size_t(limit));
			limited_emplace(sorted_queued, entry.second.queue.size(), domain, size_t(limit));
		}
		{
			std::vector<Object> list;
			for(const auto& entry : sorted_fetched) {
				list.push_back(entry.second);
			}
			stats["most_fetched"] = list;
		}
		{
			std::vector<Object> list;
			for(const auto& entry : sorted_queued) {
				list.push_back(entry.second);
			}
			stats["most_queued"] = list;
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
	log(INFO).out << url_map.size() << " queued, "
			<< crawl_frontend_async->vnx_get_num_pending() << " pending, "
			<< queue_block_count << " blocking, "
			<< fetch_counter << " fetched, " << error_counter << " failed, "
			<< delete_counter << " deleted, " << reload_counter << " reload, "
			<< active_domains << " domains, " << average_depth << " depth";
	log(INFO).out << "Robots: " << pending_robots_txt << " pending, "
			<< missing_robots_txt << " missing, " << timed_out_robots_txt << " timeout, "
			<< found_robots_txt << " found";
	if(do_reprocess) {
		log(INFO).out << reproc_counter << " reprocessed, " << delete_counter << " deleted";
	}
}


} // search
} // vnx
