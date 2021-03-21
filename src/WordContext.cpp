/*
 * WordContext.cpp
 *
 *  Created on: Mar 21, 2021
 *      Author: mad
 */

#include <vnx/search/WordContext.hxx>

#include <unordered_set>


namespace vnx {
namespace search {

std::shared_ptr<const WordContext>
WordContext::apply(std::shared_ptr<const WordContext> delta) const
{
	auto out = WordContext::create();
	out->id = id;
	out->last_update = delta->last_update;
	out->pages.reserve(pages.size() + delta->pages.size());
	
	std::unordered_set<uint32_t> update_set;
	update_set.reserve(delta->pages.size());
	for(const auto& entry : delta->pages) {
		update_set.insert(entry.first);
	}
	
	auto iter = pages.begin();
	auto iter2 = delta->pages.begin();
	while(true)
	{
		if(iter2 != delta->pages.end()) {
			if(iter2->second < 0) {
				iter2++;
				continue;
			}
			if(iter == pages.end() || iter2->second >= iter->second) {
				out->pages.emplace_back(*iter2);
				iter2++;
				continue;
			}
		} else if(iter == pages.end()) {
			break;
		}
		if(!update_set.count(iter->first)) {
			out->pages.emplace_back(*iter);
		}
		iter++;
	}
	return out;
}


} // search
} // vnx
