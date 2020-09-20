/*
 * SearchFrontend.h
 *
 *  Created on: Sep 13, 2020
 *      Author: mad
 */

#include <vnx/search/SearchFrontendBase.hxx>
#include <vnx/search/QueryEngineAsyncClient.hxx>


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
	
	void query_callback(const vnx::request_id_t& req_id,
						std::shared_ptr<const SearchResult> result) const;
	
private:
	std::shared_ptr<QueryEngineAsyncClient> engine_async;
	
};


} // search
} // vnx
