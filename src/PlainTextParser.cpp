/*
 * PlainTextParser.cpp
 *
 *  Created on: Apr 14, 2020
 *      Author: mad
 */

#include <vnx/search/PlainTextParser.h>


namespace vnx {
namespace search {

PlainTextParser::PlainTextParser(const std::string& _vnx_name)
	:	ContentParserBase(_vnx_name)
{
}

void PlainTextParser::main()
{
	service_pipe = vnx::open_pipe(Hash64::rand(), this, 1000);		// need to block here since we are a bottleneck
	
	frontend = std::make_shared<CrawlFrontendClient>(frontend_server);
	
	set_timer_millis(1000, std::bind(&PlainTextParser::update, this));
	
	update();
	
	Super::main();
}

void PlainTextParser::update()
{
	// always try to connect so frontend can reach us (non-blocking)
	vnx::connect(service_pipe, vnx::get_pipe(frontend_server), 0);
	
	frontend->register_parser_async(service_pipe->get_mac_addr(), {"text", "text/plain"}, 1);
}

std::shared_ptr<const TextResponse>
PlainTextParser::parse(const std::shared_ptr<const HttpResponse>& response) const
{
	auto result = TextResponse::create();
	result->Response::operator=(*response);
	result->text = std::string((const char*)response->payload.data(), response->payload.size());
	return result;
}


} // search
} // vnx
