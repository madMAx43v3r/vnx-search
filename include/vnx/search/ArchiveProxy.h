/*
 * ArchiveProxy.h
 *
 *  Created on: Jul 1, 2020
 *      Author: mad
 */

#ifndef INCLUDE_VNX_SEARCH_ARCHIVEPROXY_H_
#define INCLUDE_VNX_SEARCH_ARCHIVEPROXY_H_

#include <vnx/search/ArchiveProxyBase.hxx>
#include <vnx/keyvalue/ServerClient.hxx>


namespace vnx {
namespace search {

class ArchiveProxy : public ArchiveProxyBase {
public:
	ArchiveProxy(const std::string& _vnx_name);
	
protected:
	void main() override;
	
	void handle(std::shared_ptr<const HttpResponse> value) override;
	
private:
	void flush();
	
private:
	std::shared_ptr<keyvalue::ServerClient> client;
	
	std::vector<std::pair<Variant, std::shared_ptr<const Value>>> buffer;
	
};


} // search
} // vnx

#endif /* INCLUDE_VNX_SEARCH_ARCHIVEPROXY_H_ */
