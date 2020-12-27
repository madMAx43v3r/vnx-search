/*
 * QueryEngine.h
 *
 *  Created on: Sep 15, 2020
 *      Author: mad
 */

#ifndef INCLUDE_VNX_SEARCH_QUERYENGINE_H_
#define INCLUDE_VNX_SEARCH_QUERYENGINE_H_

#include <vnx/search/QueryEngineBase.hxx>
#include <vnx/search/SearchEngineAsyncClient.hxx>
#include <vnx/keyvalue/StorageAsyncClient.hxx>
#include <vnx/ThreadPool.h>


namespace vnx {
namespace search {

class QueryEngine : public QueryEngineBase {
public:
	QueryEngine(const std::string& _vnx_name);
	
protected:
	void init() override;
	
	void main() override;
	
	void query_async(	const std::vector<std::string>& words,
						const query_options_t& options,
						const request_id_t& req_id) const override;
	
private:
	struct result_t {
		uint32_t page_id = 0;
		uint32_t domain_id = 0;
		std::string scheme;
		std::string url_key;
		std::pair<int64_t, int64_t> context;
		float score = 0;
	};
	
	struct query_job_t {
		std::vector<std::string> words;
		query_options_t options;
		request_id_t req_id;
		size_t pivot_offset = 0;
		size_t pivot_size = 0;
		int64_t time_begin = 0;
		std::atomic<size_t> num_left {0};
		std::atomic<size_t> num_found {0};
		std::vector<Variant> url_keys;
		std::vector<result_t> items;
		std::vector<std::pair<uint32_t, float>> found;
		std::unordered_map<uint32_t, uint32_t> word_set;
		std::unordered_map<std::string, uint32_t> domain_set;
		std::unordered_map<uint32_t, std::pair<std::atomic<uint32_t>, std::atomic<uint32_t>>> page_hits;
		std::vector<std::shared_ptr<const WordContext>> word_context;
		std::vector<std::shared_ptr<const WordContext>> sorted_context;
		std::vector<std::shared_ptr<const WordArray>> word_arrays;
		std::function<void(const std::exception& ex)> error_callback;
		std::shared_ptr<SearchResult> result;
	};
	
	void query_callback_0(	std::shared_ptr<query_job_t> job,
							std::vector<std::shared_ptr<const keyvalue::Entry>> entries) const;
	
	void query_callback_1(	std::shared_ptr<query_job_t> job) const;
	
	void query_callback_2(	std::shared_ptr<query_job_t> job,
							std::vector<page_entry_t> entries) const;
	
	void query_callback_3(	std::shared_ptr<query_job_t> job,
							std::vector<std::shared_ptr<const keyvalue::Entry>> entries) const;
	
	void query_callback_4(std::shared_ptr<query_job_t> job) const;
	
	void query_callback_5(	std::shared_ptr<query_job_t> job,
							std::vector<std::shared_ptr<const keyvalue::Entry>> entries) const;
	
	void query_callback_6(	std::shared_ptr<query_job_t> job,
							std::vector<std::shared_ptr<const keyvalue::Entry>> entries) const;
	
	void query_task_0(std::shared_ptr<query_job_t> job, size_t index) const noexcept;
	
	void query_task_1(	std::shared_ptr<query_job_t> job, size_t index,
						std::shared_ptr<const WordArray> word_array) const noexcept;
	
private:
	std::shared_ptr<ThreadPool> query_threads;
	
	std::shared_ptr<SearchEngineAsyncClient> search_engine_async;
	std::shared_ptr<keyvalue::StorageAsyncClient> page_index_async;
	std::shared_ptr<keyvalue::StorageAsyncClient> page_content_async;
	std::shared_ptr<keyvalue::StorageAsyncClient> word_context_async;
	std::shared_ptr<keyvalue::StorageAsyncClient> word_array_async;
	
	mutable size_t query_counter = 0;
	
};


} // search
} // vnx

#endif /* INCLUDE_VNX_SEARCH_QUERYENGINE_H_ */
