/*
 * PlainTextParser.h
 *
 *  Created on: Apr 14, 2020
 *      Author: mad
 */

#ifndef INCLUDE_VNX_SEARCH_PLAINTEXTPARSER_H_
#define INCLUDE_VNX_SEARCH_PLAINTEXTPARSER_H_

#include <vnx/search/ContentParserBase.hxx>
#include <vnx/search/CrawlFrontendClient.hxx>


namespace vnx {
namespace search {

class PlainTextParser : public ContentParserBase {
public:
	PlainTextParser(const std::string& _vnx_name);
	
protected:
	void main() override;
	
	void update();
	
	std::shared_ptr<const TextResponse> parse(const std::shared_ptr<const HttpResponse>& response) const;
	
private:
	std::shared_ptr<Pipe> service_pipe;
	std::shared_ptr<CrawlFrontendClient> frontend;
	
};


} // search
} // vnx

#endif /* INCLUDE_VNX_SEARCH_PLAINTEXTPARSER_H_ */
