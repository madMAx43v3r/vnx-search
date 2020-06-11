/*
 * vnx_search_backend.cpp
 *
 *  Created on: Apr 4, 2020
 *      Author: mad
 */

#include <vnx/keyvalue/Server.h>

#include <vnx/vnx.h>
#include <vnx/Terminal.h>
#include <vnx/Server.h>


int main(int argc, char** argv)
{
	std::map<std::string, std::string> options;
	
	vnx::init("vnx_search_backend", argc, argv, options);
	
	{
		vnx::Handle<vnx::Terminal> terminal = new vnx::Terminal("Terminal");
		terminal.start_detached();
	}
	{
		vnx::Handle<vnx::Server> server = new vnx::Server("Server", vnx::Endpoint::from_url(".vnx_search_backend.sock"));
		server.start_detached();
	}
	{
		vnx::Handle<vnx::keyvalue::Server> module = new vnx::keyvalue::Server("UrlIndex");
		module->collection = "url_index";
		module->update_topic = "backend.url_index.updates";
		module.start_detached();
	}
	{
		vnx::Handle<vnx::keyvalue::Server> module = new vnx::keyvalue::Server("PageIndex");
		module->collection = "page_index";
		module->update_topic = "backend.page_index.updates";
		module.start_detached();
	}
	{
		vnx::Handle<vnx::keyvalue::Server> module = new vnx::keyvalue::Server("PageContent");
		module->collection = "page_content";
		module->update_topic = "backend.page_content.updates";
		module.start_detached();
	}
	
	vnx::wait();
}

