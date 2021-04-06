/*
 * QueryEngine.cpp
 *
 *  Created on: Sep 15, 2020
 *      Author: mad
 */

#include <vnx/search/QueryEngine.h>
#include <vnx/search/PageIndex.hxx>
#include <vnx/search/PageContent.hxx>
#include <vnx/search/WordContext.hxx>
#include <vnx/search/WordArray.hxx>
#include <vnx/search/Util.h>

#include <cmath>


namespace vnx {
namespace search {

static
bool has_flag(const std::vector<search_flags_e>& flags, search_flags_e flag)
{
	return std::find(flags.begin(), flags.end(), flag) != flags.end();
}

QueryEngine::QueryEngine(const std::string& _vnx_name)
	:	QueryEngineBase(_vnx_name)
{
}

void QueryEngine::init()
{
	vnx::open_pipe(vnx_name, this, 1000);
}

void QueryEngine::main()
{
	page_index_async = std::make_shared<keyvalue::StorageAsyncClient>(page_index_server);
	page_content_async = std::make_shared<keyvalue::StorageAsyncClient>(page_content_server);
	word_context_async = std::make_shared<keyvalue::StorageAsyncClient>(word_context_server);
	word_array_async = std::make_shared<keyvalue::StorageAsyncClient>(word_array_server);
	search_engine_async = std::make_shared<SearchEngineAsyncClient>(search_engine_server);
	
	add_async_client(page_index_async);
	add_async_client(page_content_async);
	add_async_client(word_context_async);
	add_async_client(word_array_async);
	add_async_client(search_engine_async);
	
	query_threads = std::make_shared<ThreadPool>(num_threads);
	
	Super::main();
	
	query_threads->close();
}

void QueryEngine::query_async(	const std::vector<std::string>& words,
								const query_options_t& options,
								const vnx::request_id_t& req_id) const
{
	auto job = std::make_shared<query_job_t>();
	job->words = unique(words);
	job->options = options;
	job->req_id = req_id;
	
	auto result = SearchResult::create();
	result->is_fail = true;
	result->options = options;
	
	job->result = result;
	job->time_begin = vnx::get_wall_time_micros();
	job->error_callback =
			[this, w_job = std::weak_ptr<query_job_t>(job)](const std::exception& ex) {
				if(auto job = w_job.lock()) {
					vnx_async_return(job->req_id, vnx::InternalError::from_what(ex.what()));
				}
			};
	
	word_context_async->get_values(
			std::vector<Variant>(job->words.begin(), job->words.end()),
			std::bind(&QueryEngine::query_callback_0, this, job, std::placeholders::_1),
			job->error_callback);
	query_counter++;
}

void QueryEngine::query_callback_0(	std::shared_ptr<query_job_t> job,
									std::vector<std::shared_ptr<const keyvalue::Entry>> entries) const
{
	{
		const auto now = vnx::get_wall_time_micros();
		const auto delta = now - job->time_begin;
		job->result->load_time_us += delta;
		job->result->timing_info["word_context"] = delta;
		job->time_begin = now;
	}
	{
		int i = 0;
		for(auto entry : entries) {
			if(auto context = std::dynamic_pointer_cast<const WordContext>(entry->value)) {
				job->word_context.push_back(context);
				job->word_set[context->id] = i;
				job->result->words.push_back(entry->key.to_string_value());
			}
			i++;
		}
	}
	if(job->word_context.empty()) {
		query_async_return(job->req_id, job->result);
		return;
	}
	
	job->sorted_context = job->word_context;
	std::sort(job->sorted_context.begin(), job->sorted_context.end(),
		[](std::shared_ptr<const WordContext> A, std::shared_ptr<const WordContext> B) -> bool {
			return A->pages.size() < B->pages.size();
		});
	
	job->found.resize(job->options.max_results);
	query_callback_1(job);
}

void QueryEngine::query_callback_1(	std::shared_ptr<query_job_t> job) const
{
	const uint32_t num_words = job->word_context.size();
	const auto& pivot = job->sorted_context[0]->pages;
	
	if(job->num_found < job->found.size() && job->pivot_offset < pivot.size())
	{
		const auto num_task = job->sorted_context.size() - 1;
		if(num_task) {
			job->page_hits.clear();
			job->page_hits.reserve(std::min(pivot.size() - job->pivot_offset, size_t(max_pivot_size)));
			job->pivot_offset += job->pivot_size;
			job->pivot_size = 0;
			for(; job->pivot_offset + job->pivot_size < pivot.size()
					&& job->pivot_size < size_t(max_pivot_size); ++job->pivot_size)
			{
				const auto& entry = pivot[job->pivot_offset + job->pivot_size];
				job->page_hits[entry.first] = std::make_pair(1, entry.second);
			}
			job->num_left = num_task;
			for(size_t i = 0; i < num_task; ++i) {
				query_threads->add_task(std::bind(&QueryEngine::query_task_0, this, job, i + 1));
			}
			return;
		} else {
			for(const auto& entry : pivot) {
				const auto index = job->num_found++;
				if(index < job->found.size()) {
					job->found[index] = entry;
				} else {
					break;
				}
			}
		}
	}
	{
		const auto now = vnx::get_wall_time_micros();
		const auto delta = now - job->time_begin;
		job->result->compute_time_us += delta;
		job->result->timing_info["query_task_0"] = delta;
		job->time_begin = now;
	}
	if(job->num_found >= job->found.size()) {
		job->result->has_more = true;
	}
	if(job->num_found < job->found.size()) {
		job->found.resize(job->num_found);
	}
	std::vector<uint32_t> page_ids;
	for(const auto& entry : job->found) {
		page_ids.push_back(entry.first);
	}
	search_engine_async->get_page_entries(page_ids,
			std::bind(&QueryEngine::query_callback_2, this, job, std::placeholders::_1),
			job->error_callback);
}

void QueryEngine::query_callback_2(std::shared_ptr<query_job_t> job,
									std::vector<page_entry_t> entries) const
{
	{
		const auto now = vnx::get_wall_time_micros();
		const auto delta = now - job->time_begin;
		job->result->compute_time_us += delta;
		job->result->timing_info["get_page_entries"] = delta;
		job->time_begin = now;
	}
	for(size_t i = 0; i < entries.size(); ++i)
	{
		const auto& entry = entries[i];
		const auto& found = job->found[i];
		if(entry.id != found.first) {
			continue;
		}
		const Url::Url parsed(entry.url);
		auto& domain_id = job->domain_set[parsed.host()];
		if(!domain_id) {
			domain_id = job->domain_set.size();
		}
		result_t item;
		item.page_id = entry.id;
		item.domain_id = domain_id;
		item.scheme = parsed.scheme();
		item.url_key = get_url_key(parsed);
		item.score = found.second;
		job->items.emplace_back(std::move(item));
	}
	std::vector<Variant> url_keys;
	for(const auto& item : job->items) {
		url_keys.emplace_back(item.url_key);
	}
	word_array_async->get_values(url_keys,
			std::bind(&QueryEngine::query_callback_3, this, job, std::placeholders::_1),
			job->error_callback);
}

void QueryEngine::query_callback_3(	std::shared_ptr<query_job_t> job,
									std::vector<std::shared_ptr<const keyvalue::Entry>> entries) const
{
	{
		const auto now = vnx::get_wall_time_micros();
		const auto delta = now - job->time_begin;
		job->result->load_time_us += delta;
		job->result->timing_info["word_array"] = delta;
		job->time_begin = now;
	}
	job->num_left = 0;
	job->word_arrays.clear();
	job->word_arrays.resize(entries.size());
	
	for(size_t i = 0; i < entries.size(); ++i) {
		if(auto array = std::dynamic_pointer_cast<const WordArray>(entries[i]->value)) {
			job->word_arrays[i] = array;
			job->num_left++;
		}
	}
	if(job->num_left) {
		for(size_t i = 0; i < entries.size(); ++i) {
			if(auto array = job->word_arrays[i]) {
				query_threads->add_task(std::bind(&QueryEngine::query_task_1, this, job, i));
			}
		}
	} else {
		query_callback_4(job);
	}
}

void QueryEngine::query_callback_4(std::shared_ptr<query_job_t> job) const
{
	{
		const auto now = vnx::get_wall_time_micros();
		const auto delta = now - job->time_begin;
		job->result->compute_time_us += delta;
		job->result->timing_info["query_task_1"] = delta;
		job->time_begin = now;
	}
	std::multimap<float, const result_t*, std::greater<float>> sorted;
	
	if(has_flag(job->options.flags, search_flags_e::GROUP_BY_DOMAIN))
	{
		std::unordered_map<uint32_t, std::multimap<float, const result_t*, std::greater<float>>> best_of;
		for(const auto& item : job->items) {
			best_of[item.domain_id].emplace(item.score, &item);
		}
		for(const auto& entry : best_of) {
			int i = 0;
			const auto& list = entry.second;
			for(auto iter = list.begin(); i < job->options.max_group_size && iter != list.end(); ++iter, ++i) {
				sorted.emplace(*iter);
			}
		}
	}
	else {
		for(const auto& result : job->items) {
			sorted.emplace(result.score, &result);
		}
	}
	
	std::vector<result_t> selected;
	{
		uint32_t i = 0;
		for(const auto& entry : sorted) {
			if(i++ >= job->options.offset) {
				if(selected.size() < size_t(job->options.limit)) {
					selected.emplace_back(*entry.second);
				} else {
					break;
				}
			}
		}
	}
	job->url_keys.clear();
	for(const auto& sel : selected) {
		result_item_t item;
		item.score = sel.score;
		item.url = sel.scheme + ":" + sel.url_key;
		job->url_keys.emplace_back(sel.url_key);
		job->result->items.emplace_back(std::move(item));
	}
	job->items = selected;
	job->result->num_results_total = sorted.size();
	job->result->is_fail = false;
	{
		const auto time_mid = vnx::get_wall_time_micros();
		const auto delta = time_mid - job->time_begin;
		job->result->compute_time_us += delta;
		job->result->timing_info["query_callback_4"] = delta;
		job->time_begin = time_mid;
	}
	page_index_async->get_values(job->url_keys,
			std::bind(&QueryEngine::query_callback_5, this, job, std::placeholders::_1),
			job->error_callback);
}

void QueryEngine::query_callback_5( std::shared_ptr<query_job_t> job,
									std::vector<std::shared_ptr<const keyvalue::Entry>> entries) const
{
	{
		const auto now = vnx::get_wall_time_micros();
		const auto delta = now - job->time_begin;
		job->result->load_time_us += delta;
		job->result->timing_info["page_index"] = delta;
		job->time_begin = now;
	}
	for(size_t i = 0; i < entries.size(); ++i) {
		if(auto index = std::dynamic_pointer_cast<const PageIndex>(entries[i]->value)) {
			job->result->items[i].title = index->title;
			job->result->items[i].last_modified = index->last_modified;
		}
	}
	page_content_async->get_values(job->url_keys,
			std::bind(&QueryEngine::query_callback_6, this, job, std::placeholders::_1),
			job->error_callback);
}

void QueryEngine::query_callback_6( std::shared_ptr<query_job_t> job,
									std::vector<std::shared_ptr<const keyvalue::Entry>> entries) const
{
	{
		const auto now = vnx::get_wall_time_micros();
		const auto delta = now - job->time_begin;
		job->result->load_time_us += delta;
		job->result->timing_info["page_content"] = delta;
		job->time_begin = now;
	}
	for(size_t i = 0; i < entries.size(); ++i) {
		if(auto content = std::dynamic_pointer_cast<const PageContent>(entries[i]->value)) {
			const auto& item = job->items[i];
			const auto begin = std::max(item.context.first, int64_t(0));
			const auto end = std::min(item.context.second, int64_t(content->text.size()));
			if(end > begin) {
				auto text = content->text.substr(begin, end - begin);
				job->result->items[i].context =
						(begin > 0 ? "... " : "") + clean_text(text) + (end < content->text.size() ? " ..." : "");
			}
		}
	}
	query_async_return(job->req_id, job->result);
}

void QueryEngine::query_task_0(std::shared_ptr<query_job_t> job, size_t index) const noexcept
{
	const uint32_t num_words = job->word_context.size();
	for(const auto& entry : job->sorted_context[index]->pages) {
		const auto page_id = entry.first;
		const auto iter = job->page_hits.find(page_id);
		if(iter != job->page_hits.end()) {
			auto& page = iter->second;
			page.second += entry.second;
			if(++page.first == num_words) {
				const auto offset = job->num_found++;
				if(offset < job->found.size()) {
					job->found[offset] = std::make_pair(page_id, float(page.second) / num_words);
				} else {
					break;
				}
			}
		}
		if(job->num_found >= job->found.size()) {
			break;
		}
	}
	if(--job->num_left == 0) {
		add_task(std::bind(&QueryEngine::query_callback_1, this, job));
	}
}

void QueryEngine::query_task_1(	std::shared_ptr<query_job_t> job, size_t index) const noexcept
{
	auto& item = job->items[index];
	const auto& array = job->word_arrays[index]->list;
	
	std::vector<uint16_t> word_list(array.size());
	for(ssize_t k = 0; k < array.size(); ++k) {
		const auto iter = job->word_set.find(array[k].word_id);
		if(iter != job->word_set.end()) {
			word_list[k] = 1 + iter->second;
		}
	}
	
	struct window_t {
		const int size;
		std::vector<float> coeff;
		std::vector<float> word_hits;
		window_t(int size_) : size(size_) {
			coeff.resize(size * 2 + 1);
			for(int i = -size; i <= size; ++i) {
				coeff[i + size] = float(size - std::abs(i) + 1) / (size + 1) / size;
			}
		}
	};
	
	window_t win_1(1);
	window_t win_2(2);
	window_t win_3(3);
	window_t win_5(5);
	window_t win_8(8);
	window_t win_13(13);
	
	const std::array<window_t*, 6> windows = {&win_1, &win_2, &win_3, &win_5, &win_8, &win_13};
	
	for(auto win : windows) {
		win->word_hits.resize(job->words.size());
	}
	
	ssize_t best_pos = -1;
	float best_score = 0;
	double total_score = 0;
	
	for(size_t k = 0; k < word_list.size(); ++k)
	{
		float score = 0;
		for(size_t j = 0; j < windows.size(); ++j)
		{
			auto* const win = windows[j];
			for(int i = -win->size; i <= win->size; ++i)
			{
				const auto k_i = ssize_t(k) + i;
				if(k_i >= 0 && k_i < word_list.size())
				{
					const auto w_i = word_list[k_i];
					if(w_i > 0) {
						auto& value = win->word_hits[w_i - 1];
						value = fmaxf(value, win->coeff[i + win->size]);
					}
				}
			}
			float sum = 0;
			for(auto& value : win->word_hits) {
				sum += value;
				value = 0;
			}
			score += powf(sum, job->options.score_power);
		}
		if(score > best_score) {
			best_pos = k;
			best_score = score;
		}
		total_score += score;
	}
	
	switch(job->options.score_type) {
		default:
		case score_type_e::MAX_SCORE:
			item.score *= best_score;
			break;
		case score_type_e::AVG_SCORE:
			item.score *= total_score / array.size();
			break;
		case score_type_e::TOTAL_SCORE:
			item.score *= total_score;
			break;
	}
	
	if(best_pos >= 0) {
		item.context.first = array[std::max(best_pos - job->options.context, ssize_t(0))].offset;
		const auto& entry =  array[std::min(best_pos + job->options.context, ssize_t(array.size() - 1))];
		item.context.second = entry.offset + entry.size;
	}
	
	if(--job->num_left == 0) {
		add_task(std::bind(&QueryEngine::query_callback_4, this, job));
	}
}


} // search
} // vnx
