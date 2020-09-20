/*
 * vnx_search_query_engine.cpp
 *
 *  Created on: Sep 20, 2020
 *      Author: mad
 */

#include <vnx/search/QueryEngine.h>

#include <vnx/vnx.h>
#include <vnx/Proxy.h>
#include <vnx/Server.h>
#include <vnx/Terminal.h>
#include <vnx/TcpEndpoint.hxx>


int main(int argc, char** argv)
{
	std::map<std::string, std::string> options;
	options["s"] = "server";
	options["p"] = "port";
	options["server"] = "engine server url";
	options["port"] = "query port";
	
	vnx::init("vnx_search_query_engine", argc, argv, options);
	
	int port = 8990;
	std::string server = ".vnx_search_engine.sock";
	vnx::read_config("server", server);
	vnx::read_config("port", port);
	
	{
		vnx::Handle<vnx::Terminal> terminal = new vnx::Terminal("Terminal");
		terminal.start_detached();
	}
	{
		vnx::Handle<vnx::Server> module = new vnx::Server("UnixServer", vnx::Endpoint::from_url(".vnx_search_query_engine.sock"));
		module.start_detached();
	}
	{
		vnx::Handle<vnx::Server> module = new vnx::Server("TcpServer", vnx::TcpEndpoint::create("0.0.0.0", port));
		module.start_detached();
	}
	
	vnx::Handle<vnx::Proxy> proxy = new vnx::Proxy("Proxy", vnx::Endpoint::from_url(server));
	
	vnx::Handle<vnx::search::QueryEngine> module = new vnx::search::QueryEngine("QueryEngine");
	proxy->forward_list.push_back(module->page_index_server);
	proxy->forward_list.push_back(module->page_content_server);
	proxy->forward_list.push_back(module->word_context_server);
	proxy->forward_list.push_back(module->word_array_server);
	proxy->forward_list.push_back(module->search_engine_server);
	
	proxy.start_detached();
	module.start_detached();
	
	vnx::wait();
}

