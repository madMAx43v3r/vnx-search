/*
 * vnx_search_query.cpp
 *
 *  Created on: Apr 10, 2020
 *      Author: mad
 */

#include <vnx/search/SearchEngineClient.hxx>

#include <vnx/Config.h>
#include <vnx/Process.h>
#include <vnx/Terminal.h>
#include <vnx/Proxy.h>
#include <vnx/PrettyPrinter.h>


int main(int argc, char** argv)
{
	std::map<std::string, std::string> options;
	options["s"] = "server";
	options["w"] = "words";
	options["l"] = "limit";
	options["server"] = "engine server url";
	options["words"] = "search words";
	options["limit"] = "max results";
	
	vnx::init("vnx_search_query", argc, argv, options);
	
	std::string server = ".vnx_search_engine.sock";
	std::vector<std::string> words;
	int limit = 10;
	
	vnx::read_config("server", server);
	vnx::read_config("words", words);
	vnx::read_config("limit", limit);
	
	{
		vnx::Handle<vnx::Terminal> terminal = new vnx::Terminal("Terminal");
		terminal.start_detached();
	}
	{
		vnx::Handle<vnx::Proxy> proxy = new vnx::Proxy("Proxy", vnx::Endpoint::from_url(server));
		proxy->forward_list.push_back("SearchEngine");
		proxy.start_detached();
	}
	
	vnx::search::SearchEngineClient client("SearchEngine");
	auto result = client.query(words, limit);
	
	vnx::PrettyPrinter print(std::cout);
	vnx::accept(print, result);
	
	vnx::close();
}

