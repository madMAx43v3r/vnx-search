/*
 * SearchFrontend.cpp
 *
 *  Created on: Sep 13, 2020
 *      Author: mad
 */

#include <vnx/search/SearchFrontend.h>
#include <vnx/search/Util.h>


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
	engine_async = std::make_shared<QueryEngineAsyncClient>(engine_server);
	engine_async->vnx_set_non_blocking(true);
	
	add_async_client(engine_async);
	
	Super::main();
}

void SearchFrontend::http_request_async(std::shared_ptr<const addons::HttpRequest> request,
										const std::string& sub_path,
										const vnx::request_id_t& req_id) const
{
	if(sub_path == "/query")
	{
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
				page = std::max(page, 0);
			}
		}
		const auto words = parse_text(text);
		if(words.empty()) {
			http_request_async_return(req_id, addons::HttpResponse::from_value_json(nullptr));
			return;
		}
		query_options_t options_ = options;
		options_.offset = page * options.limit;
		
		engine_async->query(words, options_,
			std::bind(&SearchFrontend::query_callback, this, req_id, std::placeholders::_1),
			[this, req_id](const std::exception& ex) {
				log(WARN) << "query() failed with: " << ex.what();
				http_request_async_return(req_id, addons::HttpResponse::from_status(500));
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
