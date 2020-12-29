/*
 * reverse_link_t.cpp
 *
 *  Created on: Dec 11, 2020
 *      Author: mad
 */


#include <vnx/search/reverse_link_t.hxx>


namespace vnx {
namespace search {

void reverse_link_t::vnx_read_fallback(const Variant& var)
{
	if(var.is_string()) {
		var.to(url_key);
	}
}


} // search
} // vnx
