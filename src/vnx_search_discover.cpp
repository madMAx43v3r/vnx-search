/*
 * vnx_search_discover.cpp
 *
 *  Created on: May 29, 2020
 *      Author: mad
 */

#include <vnx/search/SearchEngineClient.hxx>

#include <vnx/vnx.h>
#include <vnx/Proxy.h>


int main(int argc, char** argv)
{
	std::map<std::string, std::string> options;
	options["s"] = "server";
	options["l"] = "limit";
	options["p"] = "page";
	options["server"] = "backend server url";
	options["limit"] = "max results";
	options["page"] = "page index";
	
	vnx::init("vnx_search_discover", argc, argv, options);
	
	std::string server = ".vnx_search_engine.sock";
	int32_t limit = 100;
	uint32_t page = 0;
	
	vnx::read_config("server", server);
	vnx::read_config("limit", limit);
	vnx::read_config("page", page);
	
	{
		vnx::Handle<vnx::Proxy> proxy = new vnx::Proxy("Proxy", vnx::Endpoint::from_url(server));
		proxy->forward_list.push_back("SearchEngine");
		proxy.start_detached();
	}
	
	std::vector<std::string> root_urls;
	vnx::read_config("CrawlProcessor.root_urls", root_urls);
	
	vnx::log_info().out << "Got " << root_urls.size() << " root urls.";
	
	vnx::search::SearchEngineClient client("SearchEngine");
	
	auto result = client.get_domain_list(limit, limit >= 0 ? page * limit : 0);
	
	for(const auto& domain : result)
	{
		bool found = false;
		auto host = domain["host"].to_string_value();
		if(host.size() > 4 && host.substr(0, 4) == "www.") {
			host = host.substr(4);
		}
		for(const auto& url : root_urls) {
			if(url.find(host) != std::string::npos) {
				found = true;
				break;
			}
		}
		if(!found) {
			vnx::PrettyPrinter print(std::cout);
			vnx::accept(print, domain);
			std::cout << std::endl;
		}
	}
	
	vnx::close();
}
