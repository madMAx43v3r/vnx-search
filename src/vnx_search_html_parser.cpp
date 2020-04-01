/*
 * vnx_search_html_parser.cpp
 *
 *  Created on: Apr 1, 2020
 *      Author: mad
 */

#include <vnx/search/HtmlParser.h>

#include <vnx/Config.h>
#include <vnx/Process.h>
#include <vnx/Terminal.h>
#include <vnx/Proxy.h>


int main(int argc, char** argv)
{
	std::map<std::string, std::string> options;
	options["s"] = "server";
	options["server"] = "frontend server url";
	
	vnx::init("vnx_search_html_parser", argc, argv, options);
	
	std::string server = ".vnx_search_frontend.sock";
	vnx::read_config("server", server);
	
	{
		vnx::Handle<vnx::Terminal> terminal = new vnx::Terminal("Terminal");
		terminal.start_detached();
	}
	
	vnx::Handle<vnx::Proxy> proxy = new vnx::Proxy("Proxy", vnx::Endpoint::from_url(server));
	
	vnx::Handle<vnx::search::HtmlParser> module = new vnx::search::HtmlParser("HtmlParser");
	if(module->frontend_server.empty()) {
		module->frontend_server = "CrawlFrontend";
	}
	proxy->forward_list.push_back(module->frontend_server);
	
	proxy.start_detached();
	module.start_detached();
	
	vnx::wait();
}

