/*
 * vnx_search_query.cpp
 *
 *  Created on: Apr 10, 2020
 *      Author: mad
 */

#include <vnx/search/SearchEngineClient.hxx>

#include <vnx/vnx.h>
#include <vnx/Proxy.h>


int main(int argc, char** argv)
{
	std::map<std::string, std::string> options;
	options["s"] = "server";
	options["w"] = "words";
	options["l"] = "limit";
	options["p"] = "page";
	options["t"] = "type";
	options["f"] = "flags";
	options["server"] = "backend server url";
	options["words"] = "search words";
	options["limit"] = "max results";
	options["page"] = "page index";
	options["type"] = "score type";
	options["flags"] = "search flags";
	options["complete"] = "complete word";
	
	vnx::init("vnx_search_query", argc, argv, options);
	
	std::string server = ".vnx_search_engine.sock";
	std::vector<std::string> words;
	vnx::search::query_options_t query_options;
	int32_t limit = 10;
	uint32_t page = 0;
	bool complete = false;
	
	vnx::read_config("server", server);
	vnx::read_config("words", words);
	vnx::read_config("limit", limit);
	vnx::read_config("page", page);
	vnx::read_config("type", query_options.score_type);
	vnx::read_config("flags", query_options.flags);
	vnx::read_config("complete", complete);
	
	query_options.limit = limit;
	query_options.offset = page * limit;
	
	{
		vnx::Handle<vnx::Proxy> proxy = new vnx::Proxy("Proxy", vnx::Endpoint::from_url(server));
		proxy->forward_list.push_back("SearchEngine");
		proxy.start_detached();
	}
	
	vnx::search::SearchEngineClient client("SearchEngine");
	if(complete) {
		if(!words.empty()) {
			auto result = client.suggest_words(words[0], limit);
			std::cout << vnx::to_string(result) << std::endl;
		}
	} else {
		auto result = client.query(words, query_options);
		vnx::PrettyPrinter print(std::cout);
		vnx::accept(print, result);
		std::cout << std::endl;
	}
	
	vnx::close();
}

