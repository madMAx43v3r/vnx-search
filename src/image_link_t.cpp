/*
 * image_link_t.cpp
 *
 *  Created on: Dec 11, 2020
 *      Author: mad
 */

#include <vnx/search/image_link_t.hxx>


namespace vnx {
namespace search {

void image_link_t::vnx_read_fallback(const Variant& var)
{
	page_link_t::vnx_read_fallback(var);
}


} // search
} // vnx
