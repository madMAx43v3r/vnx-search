/*
 * vnx_search_frontend.cpp
 *
 *  Created on: Mar 30, 2020
 *      Author: mad
 */

#include <vnx/search/CrawlFrontend.h>
#include <vnx/search/HtmlParser.h>
#include <vnx/search/PlainTextParser.h>
#include <vnx/search/CrawlFrontendClient.hxx>

#include <vnx/Config.h>
#include <vnx/Process.h>
#include <vnx/Terminal.h>
#include <vnx/Server.h>


int main(int argc, char** argv)
{
	std::map<std::string, std::string> options;
	options["n"] = "num-parsers";
	options["num-parsers"] = "number of parsers";
	
	vnx::init("vnx_search_frontend", argc, argv, options);
	
	int num_parsers = 1;
	vnx::read_config("num-parsers", num_parsers);
	
	{
		vnx::Handle<vnx::Terminal> terminal = new vnx::Terminal("Terminal");
		terminal.start_detached();
	}
	{
		vnx::Handle<vnx::Server> server = new vnx::Server("Server", vnx::Endpoint::from_url(".vnx_search_frontend.sock"));
		server.start_detached();
	}
	{
		vnx::Handle<vnx::search::CrawlFrontend> module = new vnx::search::CrawlFrontend("CrawlFrontend");
		module.start_detached();
	}
	for(int i = 0; i < num_parsers; ++i) {
		vnx::Handle<vnx::search::HtmlParser> module = new vnx::search::HtmlParser("HtmlParser");
		module->frontend_server = "CrawlFrontend";
		module.start_detached();
	}
	{
		vnx::Handle<vnx::search::PlainTextParser> module = new vnx::search::PlainTextParser("PlainTextParser");
		module->frontend_server = "CrawlFrontend";
		module.start_detached();
	}
	
	vnx::wait();
}

