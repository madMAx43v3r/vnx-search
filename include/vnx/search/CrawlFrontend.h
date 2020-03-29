/*
 * CrawlFrontend.h
 *
 *  Created on: Mar 29, 2020
 *      Author: mad
 */

#ifndef INCLUDE_VNX_SEARCH_CRAWLFRONTEND_H_
#define INCLUDE_VNX_SEARCH_CRAWLFRONTEND_H_

#include <vnx/search/CrawlFrontendBase.hxx>


namespace vnx {
namespace search {

class CrawlFrontend : public CrawlFrontendBase {
public:
	CrawlFrontend(const std::string& _vnx_name);
	
protected:
	void main() override;
	
	void fetch_async(	const std::string& url,
						const std::function<void()>& _callback,
						const vnx::request_id_t& _request_id) override;
	
	void register_parser(	const vnx::Hash64& address,
							const std::string& mime_type,
							const int32_t& num_threads) override;
	
private:
	
	
};


} // search
} // vnx

#endif /* INCLUDE_VNX_SEARCH_CRAWLFRONTEND_H_ */
