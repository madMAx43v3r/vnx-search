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
	subscribe(input_http, max_queue_sec * 1000, max_queue_size);
	
	client = std::make_shared<keyvalue::StorageClient>(server_name);
	
	set_timer_millis(1000, std::bind(&ArchiveProxy::flush, this));
	
	Super::main();
	
	flush();
}

void ArchiveProxy::handle(std::shared_ptr<const HttpResponse> value)
{
	buffer.emplace_back(get_url_key(value->url), value);
	
	if(buffer.size() >= max_buffer_size) {
		flush();
	}
}

void ArchiveProxy::flush()
{
	try {
		client->store_values(buffer);
	} catch(const std::exception& ex) {
		if(vnx_do_run()) {
			log(WARN) << "flush(): " << ex.what();
		}
	}
	buffer.clear();
}


} // search
} // vnx
