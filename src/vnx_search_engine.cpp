/*
 * vnx_search_engine.cpp
 *
 *  Created on: Apr 10, 2020
 *      Author: mad
 */

#include <vnx/search/SearchEngine.h>

#include <vnx/Config.h>
#include <vnx/Process.h>
#include <vnx/Terminal.h>
#include <vnx/Proxy.h>
#include <vnx/Server.h>


int main(int argc, char** argv)
{
	std::map<std::string, std::string> options;
	options["s"] = "server";
	options["server"] = "backend server url";
	
	vnx::init("vnx_search_engine", argc, argv, options);
	
	std::string server = ".vnx_search_backend.sock";
	vnx::read_config("server", server);
	
	{
		vnx::Handle<vnx::Terminal> terminal = new vnx::Terminal("Terminal");
		terminal.start_detached();
	}
	{
		vnx::Handle<vnx::Server> server = new vnx::Server("Server", vnx::Endpoint::from_url(".vnx_search_engine.sock"));
		server.start_detached();
	}
	
	vnx::Handle<vnx::Proxy> proxy = new vnx::Proxy("Proxy", vnx::Endpoint::from_url(server));
	
	vnx::Handle<vnx::search::SearchEngine> module = new vnx::search::SearchEngine("SearchEngine");
	proxy->forward_list.push_back(module->url_index_server);
	proxy->forward_list.push_back(module->page_index_server);
	proxy->forward_list.push_back(module->page_content_server);
	proxy->import_list.push_back(module->input_page_index->get_name());
	proxy->import_list.push_back(module->input_page_index_sync->get_name());
	
	proxy.start_detached();
	module.start_detached();
	
	vnx::wait();
}

