/*
 * vnx_search_frontend.cpp
 *
 *  Created on: Mar 30, 2020
 *      Author: mad
 */

#include <vnx/search/CrawlFrontend.h>
#include <vnx/search/CrawlFrontendClient.hxx>

#include <vnx/Config.h>
#include <vnx/Process.h>
#include <vnx/Terminal.h>
#include <vnx/Server.h>

#include <unistd.h>


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
	
	// test
	vnx::search::CrawlFrontendClient client("CrawlFrontend");
	
	::usleep(2000 * 1000);
	client.fetch("https://example.com/");
	client.fetch("https://google.de/");
	
	vnx::wait();
}

