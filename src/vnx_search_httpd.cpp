/*
 * vnx_search_httpd.cpp
 *
 *  Created on: Sep 13, 2020
 *      Author: mad
 */

#include <vnx/search/SearchFrontend.h>
#include <vnx/addons/HttpServer.h>
#include <vnx/addons/FileServer.h>

#include <vnx/vnx.h>
#include <vnx/Terminal.h>
#include <vnx/Server.h>
#include <vnx/Proxy.h>


int main(int argc, char** argv) 
{
	std::map<std::string, std::string> options;
	options["s"] = "engine";
	options["a"] = "archive";
	options["engine"] = "engine server url";
	options["archive"] = "archive server url";
	
	vnx::init("vnx_search_httpd", argc, argv, options);
	
	std::string engine = ".vnx_search_query_engine.sock";
	std::string archive = ".vnx_search_archive.sock";
	vnx::read_config("engine", engine);
	vnx::read_config("archive", archive);
	
	{
		vnx::Handle<vnx::Terminal> terminal = new vnx::Terminal("Terminal");
		terminal.start_detached();
	}
	{
		vnx::Handle<vnx::Server> server = new vnx::Server("Server", vnx::Endpoint::from_url(".vnx_search_httpd.sock"));
		server.start_detached();
	}
	{
		vnx::Handle<vnx::Proxy> proxy = new vnx::Proxy("EngineProxy", vnx::Endpoint::from_url(engine));
		proxy->forward_list.push_back("QueryEngine");
		proxy->forward_list.push_back("SearchEngine");
		proxy.start_detached();
	}
	{
		vnx::Handle<vnx::Proxy> proxy = new vnx::Proxy("ArchiveProxy", vnx::Endpoint::from_url(archive));
		proxy->forward_list.push_back("ArchiveServer");
		proxy.start_detached();
	}
	{
		vnx::Handle<vnx::addons::HttpServerBase> module = vnx::addons::new_HttpServer("HttpServer");
		module->components["/"] = "FileServer";
		module->components["/search/"] = "SearchFrontend";
		module->components["/archive/"] = "ArchiveServer";
		module.start_detached();
	}
	{
		vnx::Handle<vnx::addons::FileServer> module = new vnx::addons::FileServer("FileServer");
		module.start_detached();
	}
	{
		vnx::Handle<vnx::search::SearchFrontend> module = new vnx::search::SearchFrontend("SearchFrontend");
		module.start_detached();
	}
	
	vnx::wait();
}

