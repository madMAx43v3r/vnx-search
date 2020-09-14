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
	engine_async = std::make_shared<SearchEngineAsyncClient>(engine_server);
	
	add_async_client(engine_async);
	
	Super::main();
}

void SearchFrontend::http_request_async(const std::shared_ptr<const addons::HttpRequest>& request,
										const std::string& sub_path,
										const vnx::request_id_t& req_id) const
{
	if(sub_path == "/query")
	{
		std::string text;
		{
			const auto iter = request->query_params.find("t");
			if(iter != request->query_params.end()) {
				text = iter->second;
			}
		}
		const auto words = parse_text(text);
		if(words.empty()) {
			http_request_async_return(req_id, addons::HttpResponse::from_value_json(nullptr));
			return;
		}
		query_options_t options;
		options.limit = 20;
		options.context = 30;
		
		engine_async->query(words, options,
			[this, req_id](std::shared_ptr<const SearchResult> result) {
				http_request_async_return(req_id, addons::HttpResponse::from_value_json(result));
			},
			[this, req_id](const std::exception& ex) {
				log(WARN) << "query() failed with: " << ex.what();
				http_request_async_return(req_id, addons::HttpResponse::from_status(500));
			});
	}
	else {
		http_request_async_return(req_id, addons::HttpResponse::from_status(404));
	}
}


} // search
} // vnx
