/*
 * PdfParser.cpp
 *
 *  Created on: Apr 25, 2021
 *      Author: mad
 */

#include <vnx/search/PdfParser.h>

#include <utf8.h>


namespace vnx {
namespace search {

PdfParser::PdfParser(const std::string& _vnx_name)
	:	ContentParserBase(_vnx_name)
{
}

void PdfParser::main()
{
	service_pipe = vnx::open_pipe(Hash64::rand(), this, 1000);		// need to block here since we are a bottleneck
	
	frontend = std::make_shared<CrawlFrontendClient>(frontend_server);
	
	set_timer_millis(1000, std::bind(&PdfParser::update, this));
	
	update();
	
	Super::main();
}

void PdfParser::update()
{
	frontend->register_parser_async(service_pipe->get_mac_addr(), {"application/pdf"}, 1);
}

std::shared_ptr<const TextResponse>
PdfParser::parse(std::shared_ptr<const HttpResponse> response) const
{
	auto result = TextResponse::create();
	result->Response::operator=(*response);
	
	const std::string name = vnx::to_string(vnx_get_id());
	
	File pdf(name + ".pdf");
	pdf.open("wb");
	pdf.out.write(response->payload.data(), response->payload.size());
	pdf.flush();
	
	if(::system(("pdftotext " + name + ".pdf").c_str())) {
		throw std::runtime_error("pdftotext failed");
	}
	std::ifstream stream(name + ".txt");
	const std::string text((std::istreambuf_iterator<char>(stream)), std::istreambuf_iterator<char>());
	
	if(utf8valid(text.c_str())) {
		throw std::runtime_error("invalid UTF-8");
	}
	result->text = text;
	return result;
}


} // search
} // vnx
