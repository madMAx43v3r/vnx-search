/*
 * ArchiveProxy.cpp
 *
 *  Created on: Jul 1, 2020
 *      Author: mad
 */

#include <vnx/search/ArchiveProxy.h>
#include <vnx/search/Util.h>


namespace vnx {
namespace search {

ArchiveProxy::ArchiveProxy(const std::string& _vnx_name)
	:	ArchiveProxyBase(_vnx_name)
{
}

void ArchiveProxy::main()
{
	subscribe(input_http, 300 * 1000);
	
	client = std::make_shared<keyvalue::StorageClient>(server_name);
	
	set_timer_millis(1000, std::bind(&ArchiveProxy::flush, this));
	
	Super::main();
	
	flush();
}

void ArchiveProxy::handle(std::shared_ptr<const HttpResponse> value)
{
	buffer.emplace_back(get_url_key(value->url), value);
	
	if(buffer.size() >= buffer_size) {
		flush();
	}
}

void ArchiveProxy::flush()
{
	try {
		client->store_values(buffer);
	} catch(const std::exception& ex) {
		log(WARN) << "flush(): " << ex.what();
	}
	buffer.clear();
}


} // search
} // vnx
