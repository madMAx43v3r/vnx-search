/*
 * PageProcessor.h
 *
 *  Created on: Apr 2, 2020
 *      Author: mad
 */

#ifndef INCLUDE_VNX_SEARCH_PAGEPROCESSOR_H_
#define INCLUDE_VNX_SEARCH_PAGEPROCESSOR_H_

#include <vnx/search/PageProcessorBase.hxx>
#include <vnx/keyvalue/ServerAsyncClient.hxx>


namespace vnx {
namespace search {

class PageProcessor : public PageProcessorBase {
public:
	PageProcessor(const std::string& _vnx_name);
	
protected:
	void main() override;
	
	void handle(std::shared_ptr<const TextResponse> value) override;
	
private:
	void page_index_error(uint64_t request_id, const std::exception& ex);
	
	void page_content_error(uint64_t request_id, const std::exception& ex);
	
private:
	std::shared_ptr<keyvalue::ServerAsyncClient> page_index;
	std::shared_ptr<keyvalue::ServerAsyncClient> page_content;
	
};


} // search
} // vnx

#endif /* INCLUDE_VNX_SEARCH_PAGEPROCESSOR_H_ */
