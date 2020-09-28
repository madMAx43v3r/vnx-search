/*
 * vnx_search_engine.cpp
 *
 *  Created on: Jun 11, 2020
 *      Author: mad
 */

#include <vnx/search/SearchEngine.h>
#include <vnx/search/QueryEngine.h>

#include <vnx/vnx.h>
#include <vnx/Terminal.h>
#include <vnx/Server.h>
#include <vnx/TcpEndpoint.hxx>


int main(int argc, char** argv)
{
	std::map<std::string, std::string> options;
	options["s"] = "server";
	options["p"] = "port";
	options["server"] = "backend server url";
	options["port"] = "query port";
	
	vnx::init("vnx_search_engine", argc, argv, options);
	
	int port = 8989;
	std::string server = ".vnx_search_backend.sock";
	vnx::read_config("server", server);
	vnx::read_config("port", port);
	
	{
		vnx::Handle<vnx::Terminal> terminal = new vnx::Terminal("Terminal");
		terminal.start_detached();
	}
	{
		vnx::Handle<vnx::Server> server = new vnx::Server("UnixServer", vnx::Endpoint::from_url(".vnx_search_engine.sock"));
		server.start_detached();
	}
	{
		vnx::Handle<vnx::Server> module = new vnx::Server("TcpServer", vnx::TcpEndpoint::create("0.0.0.0", port));
		module.start_detached();
	}
	{
		vnx::Handle<vnx::Proxy> proxy = new vnx::Proxy("BackendProxy", vnx::Endpoint::from_url(server));
		
		vnx::Handle<vnx::search::QueryEngine> module = new vnx::search::QueryEngine("QueryEngine");
		proxy->tunnel_map[vnx::Hash64("fast/" + module->page_index_server)] = module->page_index_server;
		proxy->tunnel_map[vnx::Hash64("fast/" + module->page_content_server)] = module->page_content_server;
		module->page_index_server = "fast/" + module->page_index_server;
		module->page_content_server = "fast/" + module->page_content_server;
		
		proxy.start_detached();
		module.start_detached();
	}
	
	vnx::Handle<vnx::Proxy> proxy = new vnx::Proxy("BackendProxy", vnx::Endpoint::from_url(server));
	proxy->max_queue_ms = 0;		// quick fix
	
	vnx::Handle<vnx::search::SearchEngine> module = new vnx::search::SearchEngine("SearchEngine");
	proxy->forward_list.push_back(module->url_index_server);
	proxy->forward_list.push_back(module->page_index_server);
	proxy->forward_list.push_back(module->page_content_server);
	proxy->import_list.push_back(module->input_url_index->get_name());
	proxy->import_list.push_back(module->input_page_index->get_name());
	proxy->import_list.push_back(module->input_page_content->get_name());
	proxy->import_list.push_back(module->input_url_index_sync->get_name());
	proxy->import_list.push_back(module->input_page_index_sync->get_name());
	proxy->import_list.push_back(module->input_page_content_sync->get_name());
	
	proxy.start_detached();
	module.start_detached();
	
	vnx::wait();
}

