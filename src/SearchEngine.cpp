/*
 * SearchEngine.cpp
 *
 *  Created on: Apr 10, 2020
 *      Author: mad
 */

#include <vnx/search/SearchEngine.h>
#include <vnx/search/Util.h>
#include <vnx/search/UrlIndex.hxx>
#include <vnx/search/PageIndex.hxx>

#include <url.h>
#include <omp.h>
#include <cmath>
#include <chrono>
#include <algorithm>


namespace vnx {
namespace search {

SearchEngine::SearchEngine(const std::string& _vnx_name)
	:	SearchEngineBase(_vnx_name)
{
	input_page_info = vnx_name + ".page_info.updates";
	input_page_content = vnx_name + ".page_content.updates";
	input_url_index_sync = vnx_name + ".url_index.sync_" + std::to_string(vnx::rand64());
	input_page_info_sync = vnx_name + ".page_info.sync_" + std::to_string(vnx::rand64());
	input_page_index_sync = vnx_name + ".page_index.sync_" + std::to_string(vnx::rand64());
	input_word_context_sync = vnx_name + ".word_context.sync_" + std::to_string(vnx::rand64());
	
	protocols.push_back("http");
	protocols.push_back("https");
}

void SearchEngine::init()
{
	vnx::open_pipe(vnx_name, this, 100);
}

void SearchEngine::main()
{
	subscribe(input_page_info, 100);
	subscribe(input_page_content, 100);
	subscribe(input_url_index_sync, 100, 100);
	subscribe(input_page_info_sync, 100, 100);
	subscribe(input_page_index_sync, 100, 100);
	subscribe(input_word_context_sync, 100, 100);
	
	protocols = get_unique(protocols);
	
	module_page_info = new keyvalue::Server("PageInfo");
	module_page_info->collection = "page_info";
	module_page_info->update_topic = input_page_info;
	module_page_info.start();
	
	module_word_context = new keyvalue::Server("WordContext");
	module_word_context->collection = "word_context";
	module_word_context.start();
	
	module_word_array = new keyvalue::Server("WordArray");
	module_word_array->collection = "word_array";
	module_word_array.start();
	
	page_info_async = std::make_shared<keyvalue::ServerAsyncClient>("PageInfo");
	word_context_async = std::make_shared<keyvalue::ServerAsyncClient>("WordContext");
	word_array_async = std::make_shared<keyvalue::ServerAsyncClient>("WordArray");
	url_index_async = std::make_shared<keyvalue::ServerAsyncClient>(url_index_server);
	page_index_async = std::make_shared<keyvalue::ServerAsyncClient>(page_index_server);
	page_content_async = std::make_shared<keyvalue::ServerAsyncClient>(page_content_server);
	
	add_async_client(page_info_async);
	add_async_client(word_context_async);
	add_async_client(word_array_async);
	add_async_client(url_index_async);
	add_async_client(page_index_async);
	add_async_client(page_content_async);
	
	set_timer_millis(queue_interval_ms, std::bind(&SearchEngine::check_queues, this));
	set_timer_millis(stats_interval_ms, std::bind(&SearchEngine::print_stats, this));
	
	page_info_async->sync_all(input_page_info_sync);
	word_context_async->sync_all(input_word_context_sync);
	
	query_threads = std::make_shared<ThreadPool>(num_query_threads);
	update_threads = std::make_shared<ThreadPool>(num_update_threads);
	
	Super::main();
	
	query_threads->close();
	update_threads->close();
}

void SearchEngine::query_async(	const std::vector<std::string>& words,
								const int32_t& limit, const uint32_t& offset,
								const std::vector<search_flags_e>& flags,
								const vnx::request_id_t& req_id) const
{
	auto job = std::make_shared<query_job_t>();
	job->words = get_unique(words);
	job->limit = limit;
	job->offset = offset;
	job->flags = flags;
	job->req_id = req_id;
	
	auto result = SearchResult::create();
	result->is_fail = true;
	job->result = result;
	job->time_begin = vnx::get_wall_time_micros();
	
	word_context_async->get_values(
			std::vector<Variant>(job->words.begin(), job->words.end()),
			std::bind(&SearchEngine::query_callback_0, this, job, std::placeholders::_1),
			[this, job](const std::exception& ex) {
				vnx_async_return(job->req_id, vnx::InternalError::from_what(ex.what()));
			});
	query_counter++;
}

void SearchEngine::query_callback_0(std::shared_ptr<query_job_t> job,
									std::vector<std::shared_ptr<const keyvalue::Entry>> entries) const
{
	for(auto entry : entries) {
		auto context = std::dynamic_pointer_cast<const WordContext>(entry->value);
		if(context) {
			job->context.push_back(context);
			job->result->words.push_back(entry->key.to_string_value());
		}
	}
	job->result->load_time_us = vnx::get_wall_time_micros() - job->time_begin;
	
	if(job->context.empty()) {
		query_async_return(job->req_id, job->result);
	} else {
		query_threads->add_task(std::bind(&SearchEngine::query_task, this, job));
	}
}

void SearchEngine::query_callback_1(std::shared_ptr<query_job_t> job) const
{
	std::vector<uint64_t> versions;
	for(auto& item : job->result->items)
	{
		const auto* page = find_page(item.page_id);
		if(page) {
			item.url = page->scheme.str() + ":";
			versions.push_back(page->info_version);
		} else {
			versions.push_back(0);
		}
	}
	page_info_async->get_keys(versions,
			std::bind(&SearchEngine::query_callback_2, this, job, std::placeholders::_1),
			[this, job](const std::exception& ex) {
				vnx_async_return(job->req_id, vnx::InternalError::from_what(ex.what()));
			});
}

void SearchEngine::query_callback_2(std::shared_ptr<query_job_t> job,
									std::vector<std::pair<uint64_t, Variant>> entries) const
{
	std::vector<Variant> keys;
	for(size_t i = 0; i < entries.size(); ++i) {
		const auto& key = entries[i].second;
		keys.push_back(key);
		job->result->items[i].url += key.to_string_value();
	}
	page_info_async->get_values(keys,
			std::bind(&SearchEngine::query_callback_3, this, job, std::placeholders::_1),
			[this, job](const std::exception& ex) {
				vnx_async_return(job->req_id, vnx::InternalError::from_what(ex.what()));
			});
}

void SearchEngine::query_callback_3(std::shared_ptr<query_job_t> job,
									std::vector<std::shared_ptr<const keyvalue::Entry>> entries) const
{
	for(size_t i = 0; i < entries.size(); ++i) {
		auto info = std::dynamic_pointer_cast<const PageInfo>(entries[i]->value);
		if(info) {
			job->result->items[i].title = info->title;
		}
	}
	query_async_return(job->req_id, job->result);
}

void SearchEngine::get_domain_info_async(	const std::string& host,
											const int32_t& limit,
											const uint32_t& offset,
											const request_id_t& req_id) const
{
	Object result;
	const auto iter = domain_map.find(host);
	if(iter != domain_map.end()) {
		const auto* domain = find_domain(iter->second);
		if(domain) {
			std::vector<std::pair<size_t, const page_t*>> sorted;
			for(const auto page_id : domain->pages) {
				const auto* page = find_page(page_id);
				if(page) {
					sorted.emplace_back(page->reverse_domains.size(), page);
				}
			}
			std::sort(sorted.begin(), sorted.end(), std::greater<std::pair<size_t, const page_t*>>());
			
			result["host"] = host;
			result["num_pages"] = sorted.size();
			
			std::vector<uint64_t> versions;
			std::vector<std::string> pages;
			for(uint32_t i = 0; i < uint32_t(limit) && offset + i < sorted.size(); ++i) {
				const auto* page = sorted[offset + i].second;
				versions.push_back(page->info_version);
				pages.push_back(page->scheme.str() + ":");
			}
			page_info_async->get_keys(versions,
				[this, req_id, result, pages](std::vector<std::pair<uint64_t, Variant>> entries) mutable {
					for(size_t i = 0; i < entries.size(); ++i) {
						pages[i] += entries[i].second.to_string_value();
					}
					result["pages"] = pages;
					get_domain_info_async_return(req_id, result);
				}, [this, req_id](const std::exception& ex) {
					vnx_async_return(req_id, vnx::InternalError::from_what(ex.what()));
				});
			return;
		}
	}
	get_domain_info_async_return(req_id, result);
}

void SearchEngine::get_page_info_async(const std::string& url_key, const request_id_t& req_id) const
{
	page_info_async->get_value(Variant(url_key),
			std::bind(&SearchEngine::get_page_info_callback, this, url_key, std::placeholders::_1, req_id),
			[this, req_id](const std::exception& ex) {
				vnx_async_return(req_id, vnx::InternalError::from_what(ex.what()));
			});
}

void SearchEngine::get_page_info_callback(	const std::string& url_key,
											std::shared_ptr<const keyvalue::Entry> entry,
											const request_id_t& req_id) const
{
	Object result;
	const auto page_info = std::dynamic_pointer_cast<const PageInfo>(entry->value);
	if(!page_info) {
		get_page_info_async_return(req_id, result);
		return;
	}
	const auto* page = find_page(page_info->id);
	if(page) {
		result["url"] = page->scheme.str() + ":" + entry->key.to_string_value();
		result["last_modified"] = page->last_modified;
		{
			const auto* domain = find_domain(page->domain_id);
			if(domain) {
				result["domain"] = domain->host.str();
			}
		}
		std::vector<std::string> reverse_domains;
		for(const auto domain_id : page->reverse_domains) {
			const auto* domain = find_domain(domain_id);
			if(domain) {
				reverse_domains.push_back(domain->host.str());
			}
		}
		result["reverse_domains"] = reverse_domains;
	}
	result["title"] = page_info->title;
	
	std::vector<uint64_t> versions;
	std::vector<std::string> links;
	for(const auto link_id : page_info->links) {
		const auto* child = find_page(link_id);
		if(child) {
			links.push_back(child->scheme.str() + ":");
			versions.push_back(child->info_version);
		}
	}
	page_info_async->get_keys(versions,
		[this, req_id, result, page_info, links](std::vector<std::pair<uint64_t, Variant>> entries) mutable {
			for(size_t i = 0; i < entries.size(); ++i) {
				links[i] += entries[i].second.to_string_value();
			}
			result["links"] = links;
			
			std::vector<uint64_t> versions;
			std::vector<std::string> reverse_links;
			for(const auto link_id : page_info->reverse_links) {
				const auto* parent = find_page(link_id);
				if(parent) {
					reverse_links.push_back(parent->scheme.str() + ":");
					versions.push_back(parent->info_version);
				}
			}
			page_info_async->get_keys(versions,
				[this, req_id, result, reverse_links](std::vector<std::pair<uint64_t, Variant>> entries) mutable {
					for(size_t i = 0; i < entries.size(); ++i) {
						reverse_links[i] += entries[i].second.to_string_value();
					}
					result["reverse_links"] = reverse_links;
					get_page_info_async_return(req_id, result);
				}, [this, req_id](const std::exception& ex) {
					vnx_async_return(req_id, vnx::InternalError::from_what(ex.what()));
				});
		}, [this, req_id](const std::exception& ex) {
			vnx_async_return(req_id, vnx::InternalError::from_what(ex.what()));
		});
}

std::vector<Object> SearchEngine::get_domain_list(const int32_t& limit, const uint32_t& offset) const
{
	std::vector<std::pair<size_t, const domain_t*>> sorted;
	for(const auto& entry : domain_index) {
		const auto& domain = entry.second;
		sorted.emplace_back(domain.pages.size(), &domain);
	}
	std::sort(sorted.begin(), sorted.end(), std::greater<std::pair<size_t, const domain_t*>>());
	
	std::vector<Object> result;
	for(uint32_t i = 0; i < uint32_t(limit) && offset + i < sorted.size(); ++i) {
		const auto* domain = sorted[offset + i].second;
		Object info;
		info["host"] = domain->host.str();
		info["num_pages"] = domain->pages.size();
		result.push_back(info);
	}
	return result;
}

void SearchEngine::reverse_lookup_async(const std::string& url_key,
										const request_id_t& req_id) const
{
	page_info_async->get_value(Variant(url_key),
			std::bind(&SearchEngine::reverse_lookup_callback, this, url_key, std::placeholders::_1, req_id),
			[this, req_id](const std::exception& ex) {
				vnx_async_return(req_id, vnx::InternalError::from_what(ex.what()));
			});
}

void SearchEngine::reverse_lookup_callback(	const std::string& url_key,
											std::shared_ptr<const keyvalue::Entry> entry,
											const request_id_t& req_id) const
{
	struct link_t {
		uint64_t version = 0;
		size_t weight = 0;
		std::string url;
	};
	
	std::vector<link_t> sorted;
	const auto page_info = std::dynamic_pointer_cast<const PageInfo>(entry->value);
	if(page_info) {
		for(const auto link_id : page_info->reverse_links) {
			const auto* parent = find_page(link_id);
			if(parent) {
				link_t link;
				link.version = parent->info_version;
				link.weight = parent->reverse_domains.size();
				link.url = parent->scheme.str() + ":";
				sorted.push_back(link);
			}
		}
	}
	std::sort(sorted.begin(), sorted.end(),
			[] (const link_t& A, const link_t& B) -> bool {
				return A.weight > B.weight;
			});
	
	std::vector<uint64_t> versions;
	for(const auto& entry : sorted) {
		versions.push_back(entry.version);
	}
	page_info_async->get_keys(versions,
		[this, req_id, sorted](std::vector<std::pair<uint64_t, Variant>> entries) {
			std::vector<std::string> result;
			for(size_t i = 0; i < entries.size(); ++i) {
				result.push_back(sorted[i].url + entries[i].second.to_string_value());
			}
			reverse_lookup_async_return(req_id, result);
		}, [this, req_id](const std::exception& ex) {
			vnx_async_return(req_id, vnx::InternalError::from_what(ex.what()));
		});
}

void SearchEngine::reverse_domain_lookup_async(	const std::string& url_key,
												const request_id_t& req_id) const
{
	page_info_async->get_value(Variant(url_key),
			std::bind(&SearchEngine::reverse_domain_lookup_callback, this, url_key, std::placeholders::_1, req_id),
			[this, req_id](const std::exception& ex) {
				vnx_async_return(req_id, vnx::InternalError::from_what(ex.what()));
			});
}

void SearchEngine::reverse_domain_lookup_callback(	const std::string& url_key,
													std::shared_ptr<const keyvalue::Entry> entry,
													const request_id_t& req_id) const
{
	std::map<std::string, uint32_t> tmp;
	const auto page_info = std::dynamic_pointer_cast<const PageInfo>(entry->value);
	if(page_info) {
		for(const auto link_id : page_info->reverse_links) {
			const auto* parent = find_page(link_id);
			if(parent) {
				const auto* domain = find_domain(parent->domain_id);
				if(domain) {
					tmp[domain->host.str()]++;
				}
			}
		}
	}
	std::vector<std::pair<std::string, uint32_t>> sorted(tmp.begin(), tmp.end());
	std::sort(sorted.begin(), sorted.end(),
			[] (const std::pair<std::string, uint32_t>& A, const std::pair<std::string, uint32_t>& B) -> bool {
				return A.second > B.second;
			});
	reverse_domain_lookup_async_return(req_id, sorted);
}

std::vector<std::string> SearchEngine::suggest_words(const std::string& prefix, const int32_t& limit) const
{
	std::vector<std::string> result;
	for(auto it = word_map.lower_bound(prefix); it != word_map.end() && result.size() < size_t(limit); ++it) {
		result.push_back(it->first.str());
	}
	return result;
}

std::vector<std::string> SearchEngine::suggest_domains(const std::string& prefix, const int32_t& limit) const
{
	std::vector<std::string> result;
	for(auto it = domain_map.lower_bound(prefix); it != domain_map.end() && result.size() < size_t(limit); ++it) {
		result.push_back(it->first.str());
	}
	return result;
}

SearchEngine::page_t* SearchEngine::find_page(uint32_t page_id)
{
	const auto iter = page_index.find(page_id);
	if(iter != page_index.end()) {
		return &iter->second;
	}
	return 0;
}

const SearchEngine::page_t* SearchEngine::find_page(uint32_t page_id) const
{
	const auto iter = page_index.find(page_id);
	if(iter != page_index.end()) {
		return &iter->second;
	}
	return 0;
}

const SearchEngine::word_t* SearchEngine::find_word(uint32_t word_id) const
{
	const auto iter = word_index.find(word_id);
	if(iter != word_index.end()) {
		return &iter->second;
	}
	return 0;
}

template<typename T>
SearchEngine::domain_t& SearchEngine::get_domain(const T& host)
{
	const auto iter = domain_map.find(host);
	if(iter != domain_map.end()) {
		return domain_index[iter->second];
	}
	else {
		const auto id = next_domain_id++;
		domain_map[host] = id;
		
		auto& domain = domain_index[id];
		domain.id = id;
		domain.host = host;
		return domain;
	}
}

const SearchEngine::domain_t* SearchEngine::find_domain(uint32_t domain_id) const
{
	const auto iter = domain_index.find(domain_id);
	if(iter != domain_index.end()) {
		return &iter->second;
	}
	return 0;
}

std::shared_ptr<SearchEngine::link_cache_t> SearchEngine::get_link_cache(uint32_t page_id)
{
	if(page_id == 0) {
		return nullptr;
	}
	auto& cache = link_cache[page_id];
	if(!cache) {
		cache = std::make_shared<link_cache_t>();
		cache->page_id = page_id;
		link_queue.emplace(vnx::get_wall_time_micros(), cache);
	}
	return cache;
}

std::shared_ptr<SearchEngine::link_cache_t> SearchEngine::get_link_cache(const std::string& url_key)
{
	auto& cache = link_cache_2[url_key];
	if(!cache) {
		cache = std::make_shared<link_cache_t>();
		cache->url_key = url_key;
		link_queue.emplace(vnx::get_wall_time_micros(), cache);
	}
	return cache;
}

std::shared_ptr<SearchEngine::word_cache_t> SearchEngine::get_word_cache(uint32_t word_id)
{
	auto& cache = word_cache[word_id];
	if(!cache) {
		cache = std::make_shared<word_cache_t>();
		cache->word_id = word_id;
		{
			auto* word = find_word(word_id);
			if(word) {
				cache->word = word->value.str();
			} else {
				throw std::logic_error("invalid word_id: " + std::to_string(word_id));
			}
		}
		const int64_t rand_offset = (::rand() * int64_t(word_commit_interval)) / RAND_MAX;
		word_queue.emplace(vnx::get_wall_time_seconds() + rand_offset, word_id);
	}
	return cache;
}

void SearchEngine::delete_page_async(const std::string& url_key)
{
	page_info_async->get_value_locked(Variant(url_key), lock_timeout * 1000,
			std::bind(&SearchEngine::delete_page_callback, this, url_key, std::placeholders::_1));
}

void SearchEngine::delete_page_callback(const std::string& url_key,
										std::shared_ptr<const keyvalue::Entry> entry)
{
	const auto page_info = std::dynamic_pointer_cast<const PageInfo>(entry->value);
	if(!page_info) {
		page_info_async->unlock(entry->key);
		return;
	}
	if(page_info->link_version)
	{
		const auto p_link_cache = get_link_cache(url_key);
		if(p_link_cache)
		{
			p_link_cache->link_version = 0;
			p_link_cache->is_page_update = true;
			p_link_cache->add_links.clear();
			p_link_cache->rem_links = page_info->links;
			
			for(const auto child_id : page_info->links)
			{
				const auto cache = get_link_cache(child_id);
				if(cache) {
					cache->rem_reverse_links.push_back(page_info->id);
				}
			}
			for(const auto parent_id : page_info->reverse_links)
			{
				const auto cache = get_link_cache(parent_id);
				if(cache) {
					cache->rem_links.push_back(page_info->id);
				}
			}
			p_link_cache->schedule_time_us = vnx::get_wall_time_micros();
		}
	}
	if(page_info->word_version)
	{
		auto& p_page_cache = page_cache[page_info->id];
		if(!p_page_cache) {
			p_page_cache = std::make_shared<page_cache_t>();
			p_page_cache->word_version = 0;
			p_page_cache->url_key = url_key;
			
			for(const auto word_id : page_info->words)
			{
				const auto cache = get_word_cache(word_id);
				if(cache) {
					cache->rem_pages.push_back(page_info->id);
					p_page_cache->words_pending++;
				}
			}
			if(p_page_cache->words_pending == 0) {
				page_word_update_finished(page_info->id);
			}
		}
	}
	if(page_info->id)
	{
		std::unique_lock lock(index_mutex);
		page_index.erase(page_info->id);
	}
	if(!page_info->is_deleted)
	{
		auto copy = vnx::clone(page_info);
		copy->is_deleted = true;
		page_info_async->store_value(entry->key, copy);
		word_array_async->delete_value(entry->key);
	} else {
		page_info_async->unlock(entry->key);
	}
}

void SearchEngine::redirect_callback(	const std::string& org_url_key,
										const std::string& new_url_key,
										std::vector<std::shared_ptr<const keyvalue::Entry>> entries)
{
	std::unique_lock lock(index_mutex);
	
	const auto org_page_info = std::dynamic_pointer_cast<const PageInfo>(entries[0]->value);
	const auto new_page_info = std::dynamic_pointer_cast<const PageInfo>(entries[1]->value);
	
	if(org_page_info && !org_page_info->is_deleted)
	{
		auto* new_page = new_page_info ? find_page(new_page_info->id) : nullptr;
		const auto p_new_cache = get_link_cache(new_url_key);
		
		for(const auto parent_id : org_page_info->reverse_links)
		{
			if(new_page) {
				auto* parent = find_page(parent_id);
				if(parent) {
					unique_push_back(new_page->reverse_domains, parent->domain_id);
				}
				const auto cache = get_link_cache(parent_id);
				if(cache) {
					cache->add_links.push_back(new_page->id);
				}
			}
			p_new_cache->add_reverse_links.push_back(parent_id);
		}
		p_new_cache->schedule_time_us = vnx::get_wall_time_micros();
		
		delete_page_async(org_url_key);
	}
}

void SearchEngine::handle(std::shared_ptr<const keyvalue::SyncUpdate> pair)
{
	auto info = std::dynamic_pointer_cast<const PageInfo>(pair->value);
	if(info) {
		if(info->id && !info->is_deleted)
		{
			std::unique_lock lock(index_mutex);
			
			auto& page = page_index[info->id];
			page.info_version = pair->version;
			
			if(page.id != info->id)
			{
				page.id = info->id;
				page.index_version = info->index_version;
				page.link_version = info->link_version;
				page.word_version = info->word_version;
				page.scheme = info->scheme;
				page.last_modified = info->last_modified;
				
				const auto url_key = pair->key.to_string_value();
				const Url::Url parsed_url_key(url_key);
				
				auto& domain = get_domain(parsed_url_key.host());
				page.domain_id = domain.id;
				domain.pages.push_back(page.id);
				
				// populate reverse_domains
				for(const auto child_id : info->links)
				{
					auto& child = page_index[child_id];
					unique_push_back(child.reverse_domains, page.domain_id);
				}
				next_page_id = std::max(next_page_id, info->id + 1);
			}
		}
		return;
	}
	
	if(pair->collection == "url_index")
	{
		auto url_index = std::dynamic_pointer_cast<const UrlIndex>(pair->value);
		if(!url_index || url_index->fetch_count == 0) {
			return;
		}
		const auto org_url_key = pair->key.to_string_value();
		
		if(!url_index->redirect.empty())
		{
			const auto new_url_key = get_url_key(url_index->redirect);
			if(new_url_key != org_url_key)
			{
				std::vector<Variant> keys;
				keys.push_back(Variant(org_url_key));
				keys.push_back(Variant(new_url_key));
				page_info_async->get_values(keys,
						std::bind(&SearchEngine::redirect_callback, this, org_url_key, new_url_key, std::placeholders::_1));
			}
		}
		return;
	}
	
	auto word_context = std::dynamic_pointer_cast<const WordContext>(pair->value);
	if(word_context)
	{
		std::unique_lock lock(index_mutex);
		
		const stx::sstring key = pair->key.to_string_value();
		word_map[key] = word_context->id;
		
		word_t& word = word_index[word_context->id];
		word.id = word_context->id;
		word.num_pages = word_context->pages.size();
		word.value = key;
		next_word_id = std::max(next_word_id, word_context->id + 1);
		return;
	}
	
	if(pair->collection == "page_index")
	{
		auto job = std::make_shared<page_update_job_t>();
		job->url_key = pair->key.to_string_value();
		job->index_version = pair->version;
		
		page_info_async->get_value(Variant(job->url_key),
				std::bind(&SearchEngine::check_page_callback, this, job, std::placeholders::_1));
		return;
	}
	
	if(pair->collection == "page_content")
	{
		const auto url_key = pair->key.to_string_value();
		if(!is_robots_txt(Url::Url(url_key))) {
			update_word_array(pair);
		}
	}
}

void SearchEngine::handle(std::shared_ptr<const keyvalue::SyncInfo> value)
{
	if(value->code == keyvalue::SyncInfo::END)
	{
		init_sync_count++;
		if(value->collection == "page_info")
		{
			log(INFO).out << "Loaded " << page_index.size() << " page infos.";
		}
		if(init_sync_count == 2)
		{
			subscribe(input_url_index, 100);
			url_index_async->sync_all(input_url_index_sync);
			log(INFO).out << "Starting UrlIndex sync ...";
		}
		if(init_sync_count == 3)
		{
			subscribe(input_page_index, 100);
			page_index_async->sync_all_keys(input_page_index_sync);
			log(INFO).out << "Starting PageIndex sync ...";
		}
		if(init_sync_count == 4)
		{
			is_initialized = true;
			
			log(INFO).out << "Initialized with " << domain_map.size() << " domains, "
					<< page_index.size() << " pages and " << word_map.size() << " words.";
		}
	}
}

void SearchEngine::check_page_callback(	std::shared_ptr<page_update_job_t> job,
										std::shared_ptr<const keyvalue::Entry> entry)
{
	auto info = std::dynamic_pointer_cast<const PageInfo>(entry->value);
	auto* page = info ? find_page(info->id) : nullptr;
	
	job->update_info = !page || job->index_version > page->index_version
					|| !info || info->engine_version < engine_version;
	job->update_links = !page || job->index_version > page->link_version;
	job->update_words = !page || job->index_version > page->word_version;
	
	if(job->update_info || job->update_links || job->update_words)
	{
		job->info = info;
		load_queue.emplace(job);
	}
}

void SearchEngine::update_page_callback_0(	std::shared_ptr<page_update_job_t> job,
											std::shared_ptr<const keyvalue::Entry> entry)
{
	auto url_index = std::dynamic_pointer_cast<const UrlIndex>(entry->value);
	if(!url_index || url_index->depth < 0) {
		delete_page_async(job->url_key);
		return;
	}
	job->url_index = url_index;
	page_index_async->get_value(Variant(job->url_key),
			std::bind(&SearchEngine::update_page_callback_1, this, job, std::placeholders::_1));
	
	if(job->info && job->info->engine_version < 1)
	{
		page_content_async->get_value(Variant(job->url_key),
				std::bind(&SearchEngine::update_word_array, this, std::placeholders::_1));
	}
}

void SearchEngine::update_page_callback_1(	std::shared_ptr<page_update_job_t> job,
											std::shared_ptr<const keyvalue::Entry> entry)
{
	auto index = std::dynamic_pointer_cast<const PageIndex>(entry->value);
	if(!index) {
		delete_page_async(job->url_key);
		return;
	}
	job->index = index;
	job->index_version = entry->version;
	
	std::vector<Variant> link_keys;
	if(job->update_links) {
		for(const auto& link_url : job->index->links) {
			try {
				const Url::Url parsed(link_url);
				if(std::find(protocols.begin(), protocols.end(), parsed.scheme()) == protocols.end()) {
					continue;
				}
				const auto url_key = get_url_key(parsed);
				job->org_links.push_back(url_key);
				link_keys.push_back(Variant(url_key));
			} catch(...) {
				// ignore
			}
		}
	}
	url_index_async->get_values(link_keys,
			std::bind(&SearchEngine::update_page_callback_2, this, job, std::placeholders::_1));
}

void SearchEngine::update_page_callback_2(	std::shared_ptr<page_update_job_t> job,
											std::vector<std::shared_ptr<const keyvalue::Entry>> entries)
{
	for(auto entry : entries) {
		auto url_index = std::dynamic_pointer_cast<const UrlIndex>(entry->value);
		if(url_index) {
			try {
				if(!url_index->redirect.empty()) {
					const auto org_url_key = entry->key.to_string_value();
					const auto new_url_key = get_url_key(url_index->redirect);
					if(new_url_key != org_url_key) {
						job->redirects[org_url_key] = new_url_key;
					}
				}
			} catch(...) {
				// ignore
			}
		}
	}
	std::vector<Variant> link_keys;
	for(const auto& url_key : job->org_links) {
		auto iter = job->redirects.find(url_key);
		if(iter != job->redirects.end()) {
			link_keys.push_back(Variant(iter->second));
		} else {
			link_keys.push_back(Variant(url_key));
		}
	}
	page_info_async->get_values(link_keys,
			std::bind(&SearchEngine::update_page_callback_3, this, job, std::placeholders::_1));
}

void SearchEngine::update_page_callback_3(	std::shared_ptr<page_update_job_t> job,
											std::vector<std::shared_ptr<const keyvalue::Entry>> entries)
{
	for(auto entry : entries) {
		auto info = std::dynamic_pointer_cast<const PageInfo>(entry->value);
		job->links.emplace_back(entry->key.to_string_value(), info ? info->id : 0);
	}
	update_threads->add_task(std::bind(&SearchEngine::word_collect_task, this, job));
}

void SearchEngine::update_page_callback_4(	std::shared_ptr<page_update_job_t> job)
{
	page_info_async->get_value_locked(Variant(job->url_key), lock_timeout * 1000,
			std::bind(&SearchEngine::update_page_callback_5, this, job, std::placeholders::_1));
}

void SearchEngine::update_page_callback_5(	std::shared_ptr<page_update_job_t> job,
											std::shared_ptr<const keyvalue::Entry> entry)
{
	job->info = std::dynamic_pointer_cast<const PageInfo>(entry->value);
	update_page(job);
}

void SearchEngine::update_page(std::shared_ptr<page_update_job_t> job)
{
	std::unique_lock lock(index_mutex);
	
	const auto info = job->info;
	const auto index = job->index;
	const auto url_key = job->url_key;
	const auto url_index = job->url_index;
	const auto page_id = (info && info->id) ? info->id : next_page_id++;
	
	auto& page = page_index[page_id];
	page.id = page_id;
	page.index_version = job->index_version;
	page.link_version = job->index_version;
	page.word_version = job->index_version;
	page.scheme = url_index->scheme;
	page.last_modified = url_index->last_modified;
	
	if(!page.domain_id) {
		auto& domain = get_domain(Url::Url(url_key).host());
		page.domain_id = domain.id;
		domain.pages.push_back(page.id);
	}
	
	if(job->update_info)
	{
		// initialize or update page_info
		auto copy = info ? vnx::clone(info) : PageInfo::create();
		copy->id = page_id;
		copy->engine_version = engine_version;
		copy->is_deleted = false;
		copy->index_version = job->index_version;
		copy->first_seen = url_index->first_seen;
		copy->last_modified = url_index->last_modified;
		copy->scheme = url_index->scheme;
		copy->title = index->title;
		page_info_async->store_value(Variant(url_key), copy);
	}
	else {
		page_info_async->unlock(Variant(url_key));
	}
	
	if(job->update_links)
	{
		std::set<uint32_t> new_links;
		auto p_link_cache = get_link_cache(url_key);
		p_link_cache->link_version = job->index_version;
		p_link_cache->is_page_update = true;
		
		for(const auto& entry : job->links)
		{
			const auto link_id = entry.second;
			if(link_id) {
				if(link_id != page_id) {
					// link to existing page
					new_links.insert(link_id);
				}
			} else {
				// link to not yet loaded page
				const auto cached = get_link_cache(entry.first);
				if(cached) {
					cached->add_reverse_links.push_back(page_id);
				}
			}
		}
		if(info) {
			// check for new or removed links
			for(const auto link_id : info->links)
			{
				const auto iter = new_links.find(link_id);
				if(iter != new_links.end()) {
					// link already exists
					new_links.erase(iter);
				} else {
					// link was removed
					const auto cached = get_link_cache(link_id);
					if(cached) {
						cached->rem_reverse_links.push_back(page_id);
					}
					p_link_cache->rem_links.push_back(link_id);
				}
			}
		}
		for(const auto link_id : new_links)
		{
			// make new links
			auto cached = get_link_cache(link_id);
			if(cached) {
				cached->add_reverse_links.push_back(page_id);
			}
			auto* child = find_page(link_id);
			if(child) {
				unique_push_back(child->reverse_domains, page.domain_id);
			}
			p_link_cache->add_links.push_back(link_id);
		}
		if(!info || info->id == 0)
		{
			// new page was loaded
			auto reverse_links = p_link_cache->add_reverse_links;
			if(info) {
				append(reverse_links, info->reverse_links);
			}
			for(const auto parent_id : reverse_links)
			{
				auto* parent = find_page(parent_id);
				if(parent) {
					unique_push_back(page.reverse_domains, parent->domain_id);
				}
				auto cached = get_link_cache(parent_id);
				if(cached) {
					cached->add_links.push_back(page_id);
				}
			}
		}
		// make sure our link cache is saved last
		p_link_cache->schedule_time_us = vnx::get_wall_time_micros();
	}
	
	// update word index if version is greater and previous update has finished
	if(job->update_words && !page_cache.count(page_id))
	{
		const float inv_word_count = 1.f / index->word_count;
		
		for(const auto& entry : job->new_words)
		{
			const auto id = next_word_id++;
			word_map[entry.first] = id;
			
			auto& index = word_index[id];
			index.id = id;
			index.value = entry.first;
			
			auto& info = job->words[id];
			info.mode++;
			info.count = entry.second;
		}
		
		auto& p_page_cache = page_cache[page_id];
		if(p_page_cache) {
			throw std::logic_error("p_page_cache not null");
		}
		p_page_cache = std::make_shared<page_cache_t>();
		p_page_cache->word_version = job->index_version;
		p_page_cache->url_key = url_key;
		
		for(const auto& entry : job->words)
		{
			const auto word_id = entry.first;
			const auto p_word_cache = get_word_cache(word_id);
			if(entry.second.mode >= 0) {
				p_word_cache->add_pages.emplace_back(page_id, entry.second.count * inv_word_count);
				p_page_cache->words_pending++;
			}
			if(entry.second.mode <= 0) {
				p_word_cache->rem_pages.emplace_back(page_id);
				p_page_cache->words_pending++;
			}
		}
		if(p_page_cache->words_pending == 0) {
			page_word_update_finished(page_id);
		}
	}
}

void SearchEngine::update_word_array(std::shared_ptr<const keyvalue::Entry> entry)
{
	auto content = std::dynamic_pointer_cast<const PageContent>(entry->value);
	if(content) {
		auto job = std::make_shared<word_process_job_t>();
		job->url_key = entry->key;
		job->content = content;
		update_threads->add_task(std::bind(&SearchEngine::word_process_task, this, job));
	}
}

void SearchEngine::word_process_callback(std::shared_ptr<word_process_job_t> job)
{
	word_array_async->store_value(job->url_key, job->word_array);
}

void SearchEngine::check_queues()
{
	check_link_queue();
	check_word_queue();
	check_load_queue();
}

void SearchEngine::check_load_queue()
{
	while(!load_queue.empty()
			&& url_index_async->vnx_get_num_pending() < max_num_pending
			&& page_index_async->vnx_get_num_pending() < max_num_pending
			&& page_info_async->vnx_get_num_pending() < max_num_pending
			&& update_threads->get_num_pending() < max_num_pending
			&& word_cache.size() <= 1.1 * max_word_cache)
	{
		const auto job = load_queue.front();
		url_index_async->get_value(Variant(job->url_key),
						std::bind(&SearchEngine::update_page_callback_0, this, job, std::placeholders::_1));
		load_queue.pop();
	}
}

void SearchEngine::check_link_queue()
{
	const auto now = vnx::get_wall_time_micros();
	while(!link_queue.empty())
	{
		const auto iter = link_queue.begin();
		if(now - iter->first >= int64_t(link_commit_interval) * 1000000)
		{
			const auto cache = iter->second;
			if(cache->schedule_time_us) {
				link_queue.emplace(cache->schedule_time_us, cache);
				cache->schedule_time_us = 0;
			} else {
				if(cache->url_key.empty()) {
					const auto* page = find_page(cache->page_id);
					if(page) {
						page_info_async->get_key(page->info_version,
								std::bind(&SearchEngine::link_update_callback, this, cache, std::placeholders::_1));
					}
					link_cache.erase(cache->page_id);
				} else {
					link_update_callback(cache, Variant(cache->url_key));
					link_cache_2.erase(cache->url_key);
				}
			}
			link_queue.erase(iter);
		} else {
			break;
		}
	}
}

void SearchEngine::check_word_queue()
{
	const auto now = vnx::get_wall_time_seconds();
	while(!word_queue.empty())
	{
		const auto iter = word_queue.begin();
		if(now - iter->first >= word_commit_interval || word_cache.size() > max_word_cache)
		{
			const auto word_id = iter->second;
			const auto iter2 = word_cache.find(word_id);
			if(iter2 != word_cache.end())
			{
				auto job = std::make_shared<word_update_job_t>();
				job->cached = iter2->second;
				word_context_async->get_value(Variant(job->cached->word),
						std::bind(&SearchEngine::word_update_callback, this, job, std::placeholders::_1));
				word_cache.erase(iter2);
			}
			word_queue.erase(iter);
		} else {
			break;
		}
	}
}

void SearchEngine::link_update_callback(std::shared_ptr<link_cache_t> cache, Variant url_key)
{
	if(!url_key.is_null()) {
		page_info_async->get_value_locked(url_key, lock_timeout * 1000,
				std::bind(&SearchEngine::link_update, this, cache, std::placeholders::_1));
	} else {
		link_queue.emplace(vnx::get_wall_time_micros(), cache);
	}
}

void SearchEngine::link_update(	std::shared_ptr<link_cache_t> cache,
								std::shared_ptr<const keyvalue::Entry> entry)
{
	auto page_info = std::dynamic_pointer_cast<PageInfo>(vnx::clone(entry->value));
	if(!page_info) {
		page_info = PageInfo::create();
	}
	if(cache->is_page_update) {
		page_info->link_version = cache->link_version;
	}
	for(const auto link_id : cache->rem_links) {
		remove(page_info->links, link_id);
	}
	for(const auto link_id : cache->add_links) {
		unique_push_back(page_info->links, link_id);
	}
	for(const auto page_id : cache->rem_reverse_links) {
		remove(page_info->reverse_links, page_id);
	}
	for(const auto page_id : cache->add_reverse_links) {
		unique_push_back(page_info->reverse_links, page_id);
	}
	page_info_async->store_value(entry->key, page_info);
	page_update_counter++;
	check_link_queue();
}

void SearchEngine::word_update_callback(std::shared_ptr<word_update_job_t> job,
										std::shared_ptr<const keyvalue::Entry> entry)
{
	job->context = std::dynamic_pointer_cast<const WordContext>(entry->value);
	update_threads->add_task(std::bind(&SearchEngine::word_update_task, this, job));
}

void SearchEngine::word_update_finished(std::shared_ptr<word_update_job_t> job)
{
	word_context_async->store_value(Variant(job->cached->word), job->result);
	word_update_counter++;
	
	for(const auto& entry : job->cached->add_pages)
	{
		const auto page_id = entry.first;
		const auto iter = page_cache.find(page_id);
		if(iter != page_cache.end())
		{
			auto p_page_cache = iter->second;
			unique_push_back(p_page_cache->words, job->cached->word_id);
			if(--(p_page_cache->words_pending) == 0) {
				page_word_update_finished(page_id);
			}
		}
	}
	for(const auto page_id : job->cached->rem_pages)
	{
		const auto iter = page_cache.find(page_id);
		if(iter != page_cache.end())
		{
			auto p_page_cache = iter->second;
			if(--(p_page_cache->words_pending) == 0) {
				page_word_update_finished(page_id);
			}
		}
	}
	const auto iter = word_index.find(job->cached->word_id);
	if(iter != word_index.end()) {
		iter->second.num_pages = job->num_pages;
	}
}

void SearchEngine::page_word_update_finished(uint32_t page_id)
{
	const auto iter = page_cache.find(page_id);
	if(iter != page_cache.end())
	{
		const auto cached = iter->second;
		if(cached->words_pending == 0)
		{
			page_info_async->get_value_locked(Variant(cached->url_key), lock_timeout * 1000,
					std::bind(&SearchEngine::page_word_update_callback, this, cached, std::placeholders::_1));
			page_cache.erase(iter);
		}
	}
}

void SearchEngine::page_word_update_callback(	std::shared_ptr<page_cache_t> cached,
												std::shared_ptr<const keyvalue::Entry> entry)
{
	const auto info = std::dynamic_pointer_cast<PageInfo>(vnx::clone(entry->value));
	if(info) {
		info->word_version = cached->word_version;
		info->words = cached->words;
		page_info_async->store_value(entry->key, info);
		page_update_counter++;
	} else {
		page_info_async->unlock(entry->key);
	}
}

void SearchEngine::print_stats()
{
	log(INFO).out << (60000 * word_update_counter) / stats_interval_ms << " words/min, "
			<< (60000 * page_update_counter) / stats_interval_ms << " pages/min, "
			<< (60000 * query_counter) / stats_interval_ms << " query/min, "
			<< domain_index.size() << " domains, "
			<< load_queue.size() << " / " << link_queue.size() << " / "
			<< page_cache.size() << " / " << page_index.size() << " pages, "
			<< word_cache.size() << " / " << word_index.size() << " words";
	
	word_update_counter = 0;
	page_update_counter = 0;
	query_counter = 0;
}

static
bool has_flag(const std::vector<search_flags_e>& flags, search_flags_e flag)
{
	return std::find(flags.begin(), flags.end(), flag) != flags.end();
}

void SearchEngine::query_task(std::shared_ptr<query_job_t> job) const noexcept
{
	const auto time_begin = vnx::get_wall_time_micros();
	const auto result = job->result;
	const uint32_t num_words = job->context.size();
	
	size_t num_found = 0;
	std::vector<std::pair<uint32_t, uint32_t>> found(max_query_pages);
	
	if(num_words > 1)
	{
		std::unordered_map<uint32_t, std::pair<uint32_t, uint32_t>> page_hits;
		std::vector<std::vector<std::pair<uint32_t, uint16_t>>::const_iterator> iter(num_words);
		std::vector<std::vector<std::pair<uint32_t, uint16_t>>::const_iterator> end(num_words);
		for(uint32_t i = 0; i < num_words; ++i) {
			iter[i] = job->context[i]->pages.begin();
			end[i] = job->context[i]->pages.end();
		}
		uint32_t k = 0;
		uint32_t num_iter = num_words;
		
		while(num_iter > 0 && num_found < found.size())
		{
			for(int i = 0; iter[k] != end[k] && i < 10; ++iter[k], ++i)
			{
				const auto page_id = iter[k]->first;
				const uint32_t weight = iter[k]->second;
				auto& entry = page_hits[page_id];
				if(entry.first == 0) {
					entry.second = weight;
				} else {
					entry.second = std::min(weight, entry.second);
				}
				if(++entry.first == num_words) {
					const auto index = num_found++;
					if(index < found.size()) {
						found[index] = std::make_pair(page_id, entry.second);
					}
				}
			}
			if(iter[k] == end[k]) {
				iter.erase(iter.begin() + k);
				end.erase(end.begin() + k);
				num_iter--;
			} else {
				k++;
			}
			if(k >= num_iter) {
				k = 0;
			}
		}
	}
	else {
		const auto& list = job->context[0]->pages;
		num_found = std::min(found.size(), list.size());
		for(size_t i = 0; i < num_found; ++i) {
			found[i] = list[i];
		}
	}
	
	struct result_t : result_item_t {
		uint32_t domain_id = 0;
	};
	
	std::vector<result_t> results;
	{
		std::shared_lock lock(index_mutex);
		
		for(size_t i = 0; i < std::min(num_found, found.size()); ++i)
		{
			const auto& entry = found[i];
			const auto* page = find_page(entry.first);
			if(page) {
				result_t item;
				item.page_id = page->id;
				item.domain_id = page->domain_id;
				item.last_modified = page->last_modified;
				item.score = entry.second * page->reverse_domains.size();
				results.push_back(item);
			}
		}
	}
	
	std::vector<std::pair<int64_t, const result_t*>> sorted;
	
	if(has_flag(job->flags, search_flags_e::GROUP_BY_DOMAIN))
	{
		std::unordered_map<uint32_t, std::pair<int64_t, const result_t*>> best_of;
		
		for(const auto& item : results) {
			auto& current = best_of[item.domain_id];
			if(!current.second || item.score > current.first) {
				current.first = item.score;
				current.second = &item;
			}
		}
		for(const auto& entry : best_of) {
			sorted.emplace_back(entry.second.first, entry.second.second);
		}
	}
	else {
		for(const auto& result : results) {
			sorted.emplace_back(result.score, &result);
		}
	}
	std::sort(sorted.begin(), sorted.end(), std::greater<std::pair<int64_t, const result_t*>>());
	
	for(uint32_t i = 0; i < uint32_t(job->limit) && job->offset + i < sorted.size(); ++i)
	{
		result->items.push_back(*sorted[job->offset + i].second);
	}
	const auto time_mid = vnx::get_wall_time_micros();
	result->compute_time_us = time_mid - time_begin;
	result->num_results_total = sorted.size();
	result->is_fail = false;
	
	add_task(std::bind(&SearchEngine::query_callback_1, this, job));
}

void SearchEngine::word_collect_task(std::shared_ptr<page_update_job_t> job) noexcept
{
	std::shared_lock lock(index_mutex);
	
	for(const auto& entry : job->index->words)
	{
		const auto iter = word_map.find(entry.first);
		if(iter != word_map.end()) {
			auto& info = job->words[iter->second];
			info.mode++;
			info.count = entry.second;
		} else {
			job->new_words.push_back(entry);
		}
	}
	if(job->info) {
		for(const auto word_id : job->info->words) {
			if(word_index.count(word_id)) {
				job->words[word_id].mode--;
			}
		}
	}
	
	add_task(std::bind(&SearchEngine::update_page_callback_4, this, job));
}

void SearchEngine::word_process_task(std::shared_ptr<word_process_job_t> job) noexcept
{
	std::vector<std::string> word_list;
	word_list = parse_text(job->content->text);
	
	std::shared_lock lock(index_mutex);
	
	auto array = WordArray::create();
	array->last_update = std::time(0);
	for(const auto& word : word_list) {
		const auto iter = word_map.find(word);
		if(iter != word_map.end()) {
			array->list.push_back(iter->second);
		}
	}
	job->word_array = array;
	
	add_task(std::bind(&SearchEngine::word_process_callback, this, job));
}

void SearchEngine::word_update_task(std::shared_ptr<word_update_job_t> job) noexcept
{
	const auto word_id = job->cached->word_id;
	
	std::unordered_set<uint32_t> rem_pages;
	std::unordered_map<uint32_t, uint16_t> new_pages;
	
	for(const auto& entry : job->cached->add_pages)
	{
		const float tmp = powf(entry.second, word_power);
		new_pages.emplace(entry.first, std::max(std::min(int(tmp * (0xFFFF * word_cutoff)), 0xFFFF), 1));
	}
	for(const auto page_id : job->cached->rem_pages)
	{
		if(!new_pages.count(page_id)) {
			rem_pages.insert(page_id);
		}
	}
	if(job->context) {
		for(const auto& entry : job->context->pages)
		{
			const auto page_id = entry.first;
			if(!rem_pages.count(page_id) && !new_pages.count(page_id)) {
				new_pages.emplace(entry);
			}
		}
	}
	
	std::vector<std::pair<uint64_t, std::pair<uint32_t, uint16_t>>> list;
	{
		std::shared_lock lock(index_mutex);
		
		size_t i = 0;
		for(const auto& entry : new_pages)
		{
			const auto* page = find_page(entry.first);
			if(page) {
				const auto weight = entry.second;
				list.emplace_back(uint64_t(weight) * page->reverse_domains.size(), entry);
			}
			if(i++ % 8192 == 8000) {
				lock.unlock();
				lock.lock();
			}
		}
		job->num_pages = list.size();
	}
	std::sort(list.begin(), list.end(), std::greater<std::pair<uint64_t, std::pair<uint32_t, uint16_t>>>());
	
	auto result = WordContext::create();
	result->id = word_id;
	result->last_update = std::time(0);
	result->pages.reserve(list.size());
	for(const auto& entry : list) {
		result->pages.emplace_back(entry.second);
	}
	job->result = result;
	
	add_task(std::bind(&SearchEngine::word_update_finished, this, job));
}


} // search
} // vnx
