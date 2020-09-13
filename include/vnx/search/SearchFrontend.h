/*
 * SearchFrontend.h
 *
 *  Created on: Sep 13, 2020
 *      Author: mad
 */

#include <vnx/search/SearchFrontendBase.hxx>
#include <vnx/search/SearchEngineAsyncClient.hxx>


namespace vnx {
namespace search {

class SearchFrontend : public SearchFrontendBase {
public:
	SearchFrontend(const std::string& _vnx_name);
	
protected:
	void init() override;
	
	void main() override;
	
	void http_request_async(const std::shared_ptr<const addons::HttpRequest>& request,
							const std::string& sub_path,
							const vnx::request_id_t& req_id) const override;
	
private:
	std::shared_ptr<SearchEngineAsyncClient> engine_async;
	
};


} // search
} // vnx
