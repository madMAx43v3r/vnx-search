/*
 * vnx_search_crawl.cpp
 *
 *  Created on: Apr 19, 2020
 *      Author: mad
 */

#include <vnx/search/CrawlProcessor.h>

#include <vnx/Config.h>
#include <vnx/Process.h>
#include <vnx/Terminal.h>
#include <vnx/Server.h>


int main(int argc, char** argv)
{
	std::map<std::string, std::string> options;
	options["s"] = "server";
	options["f"] = "frontend";
	options["server"] = "backend server url";
	options["frontend"] = "frontend server url";
	
	vnx::init("vnx_search_crawl", argc, argv, options);
	
	std::string server = ".vnx_search_backend.sock";
	std::string frontend = ".vnx_search_frontend.sock";
	vnx::read_config("server", server);
	vnx::read_config("frontend", frontend);
	
	{
		vnx::Handle<vnx::Terminal> terminal = new vnx::Terminal("Terminal");
		terminal.start_detached();
	}
	{
		vnx::Handle<vnx::Proxy> proxy = new vnx::Proxy("FrontendProxy", vnx::Endpoint::from_url(frontend));
		proxy->forward_list.push_back("CrawlFrontend");
		proxy->import_list.push_back("frontend.text_responses");
		proxy->max_queue_ms = 0;		// quick fix
		proxy.start_detached();
	}
	
	vnx::Handle<vnx::Proxy> proxy = new vnx::Proxy("BackendProxy", vnx::Endpoint::from_url(server));
	proxy->max_queue_ms = 0;		// quick fix
	
	vnx::Handle<vnx::search::CrawlProcessor> module = new vnx::search::CrawlProcessor("CrawlProcessor");
	proxy->forward_list.push_back(module->url_index_server);
	proxy->forward_list.push_back(module->page_index_server);
	proxy->forward_list.push_back(module->page_content_server);
	proxy->import_list.push_back(module->input_url_index->get_name());
	proxy->import_list.push_back(module->input_url_index_sync->get_name());
	proxy->import_list.push_back(module->input_page_index_sync->get_name());
	
	{
		vnx::Handle<vnx::Server> server = new vnx::Server("Server", 
				vnx::Endpoint::from_url(".vnx_search_crawl." + module->profile + ".sock"));
		server.start_detached();
	}
	
	proxy.start_detached();
	module.start_detached();
	
	vnx::wait();
}

