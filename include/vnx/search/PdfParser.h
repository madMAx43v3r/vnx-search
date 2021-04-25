/*
 * PdfParser.h
 *
 *  Created on: Apr 25, 2021
 *      Author: mad
 */

#ifndef INCLUDE_VNX_SEARCH_PDFPARSER_H_
#define INCLUDE_VNX_SEARCH_PDFPARSER_H_

#include <vnx/search/ContentParserBase.hxx>
#include <vnx/search/CrawlFrontendClient.hxx>


namespace vnx {
namespace search {

class PdfParser : public ContentParserBase {
public:
	PdfParser(const std::string& _vnx_name);
	
protected:
	void main() override;
	
	void update();
	
	std::shared_ptr<const TextResponse> parse(std::shared_ptr<const HttpResponse> response) const;
	
private:
	std::shared_ptr<Pipe> service_pipe;
	std::shared_ptr<CrawlFrontendClient> frontend;
	
};


} // search
} // vnx

#endif /* INCLUDE_VNX_SEARCH_PDFPARSER_H_ */
