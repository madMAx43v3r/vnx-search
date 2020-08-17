/*
 * ArchiveServer.cpp
 *
 *  Created on: Aug 17, 2020
 *      Author: mad
 */

#include <vnx/search/ArchiveServer.h>
#include <vnx/search/UrlIndex.hxx>
#include <vnx/search/Util.h>


namespace vnx {
namespace search {

ArchiveServer::ArchiveServer(const std::string& _vnx_name)
	:	ArchiveServerBase(_vnx_name)
{
}

void ArchiveServer::init()
{
	vnx::open_pipe(vnx_name, this, 100);
}

void ArchiveServer::main()
{
	subscribe(input_updates, UNLIMITED, 10);
	
	frontend_async = std::make_shared<CrawlFrontendAsyncClient>(frontend_server);
	http_archive_async = std::make_shared<keyvalue::StorageAsyncClient>(http_archive_server);
	url_index_async = std::make_shared<keyvalue::StorageAsyncClient>(url_index_server);
	
	add_async_client(frontend_async);
	add_async_client(http_archive_async);
	add_async_client(url_index_async);
	
	set_timer_millis(check_interval_ms, std::bind(&ArchiveServer::check_queue, this));
	set_timer_millis(3000, std::bind(&ArchiveServer::print_stats, this));
	
	threads = std::make_shared<ThreadPool>(num_threads, 100);
	
	Super::main();
	
	threads->close();
}

void ArchiveServer::handle(std::shared_ptr<const keyvalue::SyncUpdate> entry)
{
	auto response = std::dynamic_pointer_cast<const HttpResponse>(entry->value);
	if(response) {
		if(g_html_content_types.count(response->content_type)) {
			threads->add_task(std::bind(&ArchiveServer::parse_html, this, response));
		}
	}
}

void ArchiveServer::check_queue()
{
	const auto now = vnx::get_wall_time_micros();
	
	auto iter = domain_map.begin();
	while(iter != domain_map.end()
			&& frontend_async->vnx_get_num_pending() < max_num_pending)
	{
		auto& domain = iter->second;
		if(domain.queue.empty()) {
			iter = domain_map.erase(iter);
			continue;
		}
		if((now - domain.last_fetch_us) / 1000 > int64_t(60 * 1000) / max_per_minute)
		{
			const auto& url = domain.queue.front();
			frontend_async->load(url,
					std::bind(&ArchiveServer::load_callback, this, url, std::placeholders::_1));
			domain.queue.pop();
			domain.last_fetch_us = now;
		}
		iter++;
	}
}

void ArchiveServer::parse_html(std::shared_ptr<const HttpResponse> response)
{
	// TODO
}

void ArchiveServer::check_url(const std::string& url)
{
	url_index_async->get_value(Variant(get_url_key(url)),
			std::bind(&ArchiveServer::check_url_callback_1, this, url, std::placeholders::_1));
}

void ArchiveServer::check_url_callback_1(	const std::string& url,
											std::shared_ptr<const keyvalue::Entry> entry)
{
	auto url_index = std::dynamic_pointer_cast<const UrlIndex>(entry->value);
	const auto url_2 = url_index && !url_index->redirect.empty() ? url_index->redirect : url;
	
	http_archive_async->get_value(Variant(get_url_key(url_2)),
			std::bind(&ArchiveServer::check_url_callback_2, url_index, std::placeholders::_1));
}

void ArchiveServer::check_url_callback_2(	const std::string& url,
											std::shared_ptr<const UrlIndex> url_index,
											std::shared_ptr<const keyvalue::Entry> entry)
{
	// TODO
}

void ArchiveServer::load_callback(const std::string& url, UrlInfo url_info)
{
	const Url::Url parsed(url);
	const auto url_key = get_url_key(parsed);
	auto new_scheme = parsed.scheme();
	
	if(!url_info.redirect.empty())
	{
		const Url::Url parsed_redir(url_info.redirect);
		const auto new_url_key = get_url_key(parsed_redir);
		
		if(new_url_key != url_key)
		{
			UrlInfo info = url_info;
			info.redirect.clear();
			url_update(new_url_key, parsed_redir.scheme(), info);
		} else {
			new_scheme = parsed_redir.scheme();
		}
	}
	url_update(url_key, new_scheme, url_info);
}

void ArchiveServer::url_update(	const std::string& url_key,
								const std::string& new_scheme,
								const UrlInfo& info)
{
	url_index_async->get_value_locked(Variant(url_key), lock_timeout * 1000,
			std::bind(&ArchiveServer::url_update_callback, this, url_key, new_scheme, info, std::placeholders::_1));
}

void ArchiveServer::url_update_callback(	const std::string& url_key,
											const std::string& new_scheme,
											const UrlInfo& info,
											std::shared_ptr<const keyvalue::Entry> entry)
{
	std::shared_ptr<UrlIndex> index;
	auto previous = std::dynamic_pointer_cast<const UrlIndex>(entry->value);
	if(previous) {
		index = vnx::clone(previous);
	} else {
		index = UrlIndex::create();
	}
	index->UrlInfo::operator=(info);
	index->scheme = new_scheme;
	index->depth = -1;
	
	if(previous) {
		index->first_seen = previous->first_seen ? previous->first_seen : info.last_fetched;
		index->fetch_count = previous->fetch_count + 1;
	} else {
		index->first_seen = info.last_fetched;
		index->fetch_count = 1;
	}
	url_index_async->store_value(entry->key, index);
	
	if(info.is_fail) {
		error_counter++;
	} else {
		fetch_counter++;
	}
}

void ArchiveServer::print_stats()
{
	
}


} // search
} // vnx
