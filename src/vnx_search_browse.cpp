/*
 * vnx_search_browse.cpp
 *
 *  Created on: May 21, 2020
 *      Author: mad
 */

#include <vnx/search/SearchEngineClient.hxx>

#include <vnx/Config.h>
#include <vnx/Process.h>
#include <vnx/Proxy.h>
#include <vnx/PrettyPrinter.h>


int main(int argc, char** argv)
{
	std::map<std::string, std::string> options;
	options["s"] = "server";
	options["k"] = "key";
	options["l"] = "limit";
	options["p"] = "page";
	options["server"] = "backend server url";
	options["key"] = "search key";
	options["limit"] = "max results";
	options["page"] = "page index";
	options["get-page-info"] = "get_page_info(key)";
	options["get-domain-info"] = "get_domain_info(key, limit, offset)";
	options["get-domain-list"] = "get_domain_list(limit, offset)";
	options["reverse-lookup"] = "reverse_lookup(key)";
	options["reverse-domain-lookup"] = "reverse_domain_lookup(key)";
	
	vnx::init("vnx_search_browse", argc, argv, options);
	
	std::string server = ".vnx_search_backend.sock";
	std::string key;
	int32_t limit = 10;
	uint32_t page = 0;
	bool get_page_info = false;
	bool get_domain_info = false;
	bool get_domain_list = false;
	bool reverse_lookup = false;
	bool reverse_domain_lookup = false;
	
	vnx::read_config("server", server);
	vnx::read_config("key", key);
	vnx::read_config("limit", limit);
	vnx::read_config("page", page);
	vnx::read_config("get-page-info", get_page_info);
	vnx::read_config("get-domain-info", get_domain_info);
	vnx::read_config("get-domain-list", get_domain_list);
	vnx::read_config("reverse-lookup", reverse_lookup);
	vnx::read_config("reverse-domain-lookup", reverse_domain_lookup);
	
	{
		vnx::Handle<vnx::Proxy> proxy = new vnx::Proxy("Proxy", vnx::Endpoint::from_url(server));
		proxy->forward_list.push_back("SearchEngine");
		proxy.start_detached();
	}
	
	vnx::search::SearchEngineClient client("SearchEngine");
	if(get_page_info) {
		auto result = client.get_page_info(key);
		vnx::PrettyPrinter print(std::cout);
		vnx::accept(print, result);
		std::cout << std::endl;
	}
	if(get_domain_info) {
		auto result = client.get_domain_info(key, limit, limit >= 0 ? page * limit : 0);
		vnx::PrettyPrinter print(std::cout);
		vnx::accept(print, result);
		std::cout << std::endl;
	}
	if(get_domain_list) {
		auto result = client.get_domain_list(limit, limit >= 0 ? page * limit : 0);
		vnx::PrettyPrinter print(std::cout);
		vnx::accept(print, result);
		std::cout << std::endl;
	}
	if(reverse_lookup) {
		auto result = client.reverse_lookup(key);
		vnx::PrettyPrinter print(std::cout);
		vnx::accept(print, result);
		std::cout << std::endl;
	}
	if(reverse_domain_lookup) {
		auto result = client.reverse_domain_lookup(key);
		vnx::PrettyPrinter print(std::cout);
		vnx::accept(print, result);
		std::cout << std::endl;
	}
	
	vnx::close();
}
