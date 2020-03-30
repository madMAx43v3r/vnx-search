/*
 * vnx_search_frontend.cpp
 *
 *  Created on: Mar 30, 2020
 *      Author: mad
 */

#include <vnx/search/CrawlFrontend.h>

#include <vnx/Config.h>
#include <vnx/Process.h>
#include <vnx/Terminal.h>
#include <vnx/Server.h>


int main(int argc, char** argv)
{
	std::map<std::string, std::string> options;
	
	vnx::init("vnx_search_frontend", argc, argv, options);
	
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
	
	vnx::wait();
}

