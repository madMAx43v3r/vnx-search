/*
 * vnx_search_archive.cpp
 *
 *  Created on: Jul 1, 2020
 *      Author: mad
 */

#include <vnx/keyvalue/Server.h>
#include <vnx/search/ArchiveProxy.h>
#include <vnx/search/ArchiveServer.h>
#include <vnx/addons/HttpServer.h>

#include <vnx/vnx.h>
#include <vnx/Terminal.h>
#include <vnx/Server.h>
#include <vnx/Proxy.h>


int main(int argc, char** argv)
{
	std::map<std::string, std::string> options;
	options["f"] = "frontend";
	options["frontend"] = "frontend server url";
	
	vnx::init("vnx_search_archive", argc, argv, options);
	
	std::string frontend = ".vnx_search_frontend.sock";
	vnx::read_config("frontend", frontend);
	
	{
		vnx::Handle<vnx::Terminal> terminal = new vnx::Terminal("Terminal");
		terminal.start_detached();
	}
	{
		vnx::Handle<vnx::Server> server = new vnx::Server("Server", vnx::Endpoint::from_url(".vnx_search_archive.sock"));
		server.start_detached();
	}
	{
		vnx::Handle<vnx::Proxy> proxy = new vnx::Proxy("Proxy", vnx::Endpoint::from_url(frontend));
		proxy->import_list.push_back("frontend.http_responses");
		proxy.start_detached();
	}
	{
		vnx::Handle<vnx::keyvalue::Server> module = new vnx::keyvalue::Server("HttpArchive");
		module->do_compress = true;
		module->collection = "http_archive";
		module->update_topic = "http_archive.updates";
		module->update_topic_keys = "http_archive.key_updates";
		module.start_detached();
	}
	{
		vnx::Handle<vnx::search::ArchiveProxy> module = new vnx::search::ArchiveProxy("ArchiveProxy");
		module->input_http = "frontend.http_responses";
		module.start_detached();
	}
	std::string archive_path;
	{
		vnx::Handle<vnx::search::ArchiveServer> module = new vnx::search::ArchiveServer("ArchiveServer");
		archive_path = module->path;
		module.start_detached();
	}
	{
		vnx::Handle<vnx::addons::HttpServerBase> module = vnx::addons::new_HttpServer("HttpServer");
		module->chunk_size = 16 * 1024 * 1024;
		module->components[archive_path] = "ArchiveServer";
		module->output_request = "http.request";
		module->output_response = "http.response";
		module.start_detached();
	}
	
	vnx::wait();
}


