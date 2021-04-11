/*
 * SearchFrontend.cpp
 *
 *  Created on: Sep 13, 2020
 *      Author: mad
 */

#include <vnx/search/SearchFrontend.h>
#include <vnx/search/Util.h>

#include <utf8.h>


namespace vnx {
namespace search {

SearchFrontend::SearchFrontend(const std::string& _vnx_name)
	:	SearchFrontendBase(_vnx_name)
{
}

void SearchFrontend::init()
{
	vnx::open_pipe(vnx_name, this, 100);
}

void SearchFrontend::main()
{
	query_engine_async = std::make_shared<QueryEngineAsyncClient>(query_engine_server);
	search_engine_async = std::make_shared<SearchEngineAsyncClient>(search_engine_server);
	
	query_engine_async->vnx_set_non_blocking(true);
	search_engine_async->vnx_set_non_blocking(true);
	
	add_async_client(query_engine_async);
	add_async_client(search_engine_async);
	
	Super::main();
}

void SearchFrontend::http_request_async(std::shared_ptr<const addons::HttpRequest> request,
										const std::string& sub_path,
										const vnx::request_id_t& req_id) const
{
	if(sub_path == "/query") {
		int page = 0;
		std::string text;
		{
			const auto iter = request->query_params.find("t");
			if(iter != request->query_params.end()) {
				text = iter->second;
			}
		}
		{
			const auto iter = request->query_params.find("p");
			if(iter != request->query_params.end()) {
				vnx::from_string(iter->second, page);
			}
		}
		const auto words = parse_text(text);
		if(words.empty()) {
			http_request_async_return(req_id, addons::HttpResponse::from_value_json(nullptr));
			return;
		}
		query_options_t options_ = options;
		options_.offset = std::max(page, 0) * options.limit;
		
		query_engine_async->query(words, options_,
			std::bind(&SearchFrontend::query_callback, this, req_id, std::placeholders::_1),
			[this, req_id](const std::exception& ex) {
				log(WARN) << "query() failed with: " << ex.what();
				http_request_async_return(req_id, addons::HttpResponse::from_text_ex(ex.what(), 500));
			});
	}
	else if(sub_path == "/get_page_info") {
		std::string url_key;
		{
			const auto iter = request->query_params.find("k");
			if(iter != request->query_params.end()) {
				url_key = iter->second;
			}
		}
		search_engine_async->get_page_info(url_key,
			[this, req_id](const Object& result) {
				http_request_async_return(req_id, addons::HttpResponse::from_object_json(result));
			},
			[this, req_id](const std::exception& ex) {
				http_request_async_return(req_id, addons::HttpResponse::from_text_ex(ex.what(), 500));
			});
	}
	else if(sub_path == "/get_page_ranking") {
		int page = 0;
		{
			const auto iter = request->query_params.find("p");
			if(iter != request->query_params.end()) {
				vnx::from_string(iter->second, page);
			}
		}
		search_engine_async->get_page_ranking(table_page_size, std::max(page, 0) * table_page_size,
			[this, req_id](const std::vector<std::pair<std::string, float>>& result) {
				http_request_async_return(req_id, addons::HttpResponse::from_string_json(vnx::to_string(result)));
			},
			[this, req_id](const std::exception& ex) {
				http_request_async_return(req_id, addons::HttpResponse::from_text_ex(ex.what(), 500));
			});
	}
	else if(sub_path == "/get_word_context") {
		std::string word;
		int page = 0;
		{
			const auto iter = request->query_params.find("w");
			if(iter != request->query_params.end()) {
				word = iter->second;
			}
		}
		{
			const auto iter = request->query_params.find("p");
			if(iter != request->query_params.end()) {
				vnx::from_string(iter->second, page);
			}
		}
		search_engine_async->get_word_context(word, table_page_size, std::max(page, 0) * table_page_size,
			[this, req_id](const std::vector<std::pair<std::string, float>>& result) {
				http_request_async_return(req_id, addons::HttpResponse::from_string_json(vnx::to_string(result)));
			},
			[this, req_id](const std::exception& ex) {
				http_request_async_return(req_id, addons::HttpResponse::from_text_ex(ex.what(), 500));
			});
	}
	else {
		http_request_async_return(req_id, addons::HttpResponse::from_status(404));
	}
}

void SearchFrontend::http_request_chunk_async(	std::shared_ptr<const addons::HttpRequest> request,
												const std::string& sub_path,
												const int64_t& offset,
												const int64_t& max_bytes,
												const vnx::request_id_t& req_id) const
{
	throw std::logic_error("not implemented");
}

void SearchFrontend::query_callback(const vnx::request_id_t& req_id,
									std::shared_ptr<const SearchResult> result) const
{
	const std::string bold_start = "<b>";
	const std::string bold_end = "</b>";
	const std::set<std::string> word_set(result->words.begin(), result->words.end());
	
	auto out = vnx::clone(result);
	for(auto& item : out->items)
	{
		if(utf8valid(item.title.c_str())) {
			item.title.clear();
		}
		if(utf8valid(item.context.c_str())) {
			item.context.clear();
		}
		size_t offset = 0;
		std::vector<std::pair<uint32_t, uint32_t>> positions;
		const auto words = parse_text(item.context, &positions);
		for(size_t i = 0; i < words.size(); ++i) {
			if(word_set.count(words[i])) {
				item.context.insert(positions[i].first + offset, bold_start);
				offset += bold_start.size();
				item.context.insert(positions[i].second + offset, bold_end);
				offset += bold_end.size();
			}
		}
	}
	http_request_async_return(req_id, addons::HttpResponse::from_value_json(out));
}


} // search
} // vnx
