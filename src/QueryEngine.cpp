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
	job->words = get_unique(words);
	job->options = options;
	job->req_id = req_id;
	
	auto result = SearchResult::create();
	result->is_fail = true;
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
		job->result->load_time_us += now - job->time_begin;
		job->time_begin = now;
	}
	int i = 0;
	for(auto entry : entries) {
		auto context = std::dynamic_pointer_cast<const WordContext>(entry->value);
		if(context) {
			job->word_context.push_back(context);
			job->word_set[context->id] = i;
			job->result->words.push_back(entry->key.to_string_value());
		}
		i++;
	}
	if(job->word_context.empty()) {
		query_async_return(job->req_id, job->result);
	} else {
		job->num_left = num_threads;
		job->found.resize(job->options.max_results);
		for(int i = 0; i < num_threads; ++i) {
			query_threads->add_task(std::bind(&QueryEngine::query_task_0, this, job, num_threads, i));
		}
	}
}

void QueryEngine::query_callback_1(std::shared_ptr<query_job_t> job) const
{
	{
		const auto now = vnx::get_wall_time_micros();
		job->result->compute_time_us += now - job->time_begin;
		job->time_begin = now;
	}
	if(job->num_found >= job->found.size()) {
		job->result->has_more = true;
	}
	for(size_t i = 0; i < std::min(size_t(job->num_found), job->found.size()); ++i)
	{
		const auto* page = find_page(job->found[i]);
		if(page) {
			result_t item;
			item.page_id = page->id;
			item.domain_id = page->domain_id;
			item.url_key = page->url_key;
			item.score = page->rank_value;
			job->items.push_back(item);
		}
	}
	std::vector<Variant> keys;
	for(const auto& item : job->items) {
		keys.emplace_back(item.url_key);
	}
	word_array_async->get_values(keys,
			std::bind(&QueryEngine::query_callback_2, this, job, std::placeholders::_1),
			job->error_callback);
}

void QueryEngine::query_callback_2(	std::shared_ptr<query_job_t> job,
									std::vector<std::shared_ptr<const keyvalue::Entry>> entries) const
{
	{
		const auto now = vnx::get_wall_time_micros();
		job->result->load_time_us += now - job->time_begin;
		job->time_begin = now;
	}
	job->num_left = 0;
	for(size_t i = 0; i < entries.size(); ++i) {
		auto word_array = std::dynamic_pointer_cast<const WordArray>(entries[i]->value);
		job->word_arrays.push_back(word_array);
		if(word_array) {
			job->num_left++;
		}
	}
	if(job->num_left) {
		for(size_t i = 0; i < entries.size(); ++i) {
			if(job->word_arrays[i]) {
				query_threads->add_task(std::bind(&QueryEngine::query_task_1, this, job, i, job->word_arrays[i]));
			}
		}
	} else {
		query_callback_3(job);
	}
}

void QueryEngine::query_callback_3(std::shared_ptr<query_job_t> job) const
{
	{
		const auto now = vnx::get_wall_time_micros();
		job->result->compute_time_us += now - job->time_begin;
		job->time_begin = now;
	}
	std::vector<std::pair<float, const result_t*>> sorted;
	
	if(has_flag(job->options.flags, search_flags_e::GROUP_BY_DOMAIN))
	{
		std::unordered_map<uint32_t, std::pair<float, const result_t*>> best_of;
		
		for(const auto& item : job->items) {
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
		for(const auto& result : job->items) {
			sorted.emplace_back(result.score, &result);
		}
	}
	std::sort(sorted.begin(), sorted.end(), std::greater<std::pair<float, const result_t*>>());
	
	job->url_keys.clear();
	std::vector<result_t> selected;
	
	for(uint32_t i = 0; i < uint32_t(job->options.limit) && job->options.offset + i < sorted.size(); ++i)
	{
		const auto* tmp_item = sorted[job->options.offset + i].second;
		selected.emplace_back(*tmp_item);
		job->url_keys.emplace_back(tmp_item->url_key);
		
		result_item_t item;
		item.score = tmp_item->score;
		item.url = tmp_item->scheme + ":" + tmp_item->url_key;
		job->result->items.emplace_back(std::move(item));
	}
	job->items = selected;
	job->result->num_results_total = sorted.size();
	job->result->is_fail = false;
	{
		const auto time_mid = vnx::get_wall_time_micros();
		job->result->compute_time_us += time_mid - job->time_begin;
		job->time_begin = time_mid;
	}
	page_index_async->get_values(job->url_keys,
			std::bind(&QueryEngine::query_callback_4, this, job, std::placeholders::_1),
			job->error_callback);
}

void QueryEngine::query_callback_4( std::shared_ptr<query_job_t> job,
									std::vector<std::shared_ptr<const keyvalue::Entry>> entries) const
{
	{
		const auto now = vnx::get_wall_time_micros();
		job->result->load_time_us += now - job->time_begin;
		job->time_begin = now;
	}
	for(size_t i = 0; i < entries.size(); ++i) {
		auto index = std::dynamic_pointer_cast<const PageIndex>(entries[i]->value);
		if(index) {
			job->result->items[i].title = index->title;
			job->result->items[i].last_modified = index->last_modified;
		}
	}
	page_content_async->get_values(job->url_keys,
			std::bind(&QueryEngine::query_callback_5, this, job, std::placeholders::_1),
			job->error_callback);
}

void QueryEngine::query_callback_5( std::shared_ptr<query_job_t> job,
									std::vector<std::shared_ptr<const keyvalue::Entry>> entries) const
{
	{
		const auto now = vnx::get_wall_time_micros();
		job->result->load_time_us += now - job->time_begin;
		job->time_begin = now;
	}
	for(size_t i = 0; i < entries.size(); ++i) {
		auto content = std::dynamic_pointer_cast<const PageContent>(entries[i]->value);
		if(content) {
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

void QueryEngine::query_task_0(	std::shared_ptr<query_job_t> job,
								uint32_t num_threads, uint32_t index) const noexcept
{
	const uint32_t num_words = job->word_context.size();
	std::unordered_map<uint32_t, uint32_t> page_hits;
	std::vector<std::vector<uint32_t>::const_iterator> iter(num_words);
	std::vector<std::vector<uint32_t>::const_iterator> end(num_words);
	for(uint32_t i = 0; i < num_words; ++i) {
		iter[i] = job->word_context[i]->pages.begin();
		end[i] = job->word_context[i]->pages.end();
	}
	uint32_t k = 0;
	uint32_t num_iter = num_words;
	
	while(num_iter > 0 && job->num_found < job->found.size())
	{
		for(int i = 0; iter[k] != end[k] && i < 10; ++iter[k], ++i)
		{
			const auto page_id = *iter[k];
			if(page_id % num_threads == index) {
				if(++page_hits[page_id] == num_words) {
					const auto index = job->num_found++;
					if(index < job->found.size()) {
						job->found[index] = page_id;
					}
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
	
	if(--job->num_left == 0) {
		add_task(std::bind(&QueryEngine::query_callback_1, this, job));
	}
}

void QueryEngine::query_task_1(	std::shared_ptr<query_job_t> job, size_t index,
								std::shared_ptr<const WordArray> word_array) const noexcept
{
	auto& item = job->items[index];
	const auto& array = word_array->list;
	
	std::vector<uint16_t> word_list(array.size());
	for(ssize_t k = 0; k < array.size(); ++k) {
		const auto iter = job->word_set.find(array[k].word_id);
		if(iter != job->word_set.end()) {
			word_list[k] = 1 + iter->second;
		}
	}
	
	struct window_t {
		ssize_t size = 0;
		std::vector<float> coeff;
		std::vector<float> word_hits;
	};
	
	std::array<window_t, 1> windows;
	windows[0].size = 16;
	
	for(auto& win : windows) {
		win.coeff.resize(win.size * 2 + 1);
		for(ssize_t i = -win.size; i <= win.size; ++i) {
			win.coeff[i + win.size] = fabsf(2 * (win.size + 1) - i) / float(2 * (win.size + 1));
		}
		win.word_hits.resize(job->words.size());
	}
	
	ssize_t best_pos = -1;
	float best_score = 0;
	double total_score = 0;
	
	for(ssize_t k = 0; k < word_list.size(); ++k)
	{
		for(auto& win : windows) {
			for(ssize_t i = -win.size; i <= win.size; ++i) {
				const auto k_i = k + i;
				if(k_i >= 0 && k_i < word_list.size()) {
					const auto w_i = word_list[k_i];
					if(w_i > 0) {
						auto& value = win.word_hits[w_i - 1];
						value = fmaxf(value, win.coeff[i + win.size]);
					}
				}
			}
		}
		float score = 0;
		for(auto& win : windows) {
			for(auto& value : win.word_hits) {
				score += value;
				value = 0;
			}
		}
		if(score > best_score) {
			best_pos = k;
			best_score = score;
		}
		total_score += score;
	}
	
	switch(job->options.score_type) {
		case score_type_e::MAX_SCORE:
			item.score *= best_score;
			break;
		default:
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
		add_task(std::bind(&QueryEngine::query_callback_3, this, job));
	}
}


} // search
} // vnx
