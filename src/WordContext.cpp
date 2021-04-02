/*
 * WordContext.cpp
 *
 *  Created on: Mar 21, 2021
 *      Author: mad
 */

#include <vnx/Time.h>
#include <vnx/search/WordContext.hxx>

#include <unordered_map>


namespace vnx {
namespace search {

inline bool sort_pair_second_desc(	const std::pair<uint32_t, float32_t>& lhs,
									const std::pair<uint32_t, float32_t>& rhs)
{
	return lhs.second > rhs.second;
}

std::shared_ptr<const WordContext>
WordContext::apply(const std::vector<std::pair<uint32_t, float32_t>>& delta, const int64_t& update_time) const
{
	auto out = WordContext::create();
	out->id = id;
	out->last_update = update_time ? update_time : vnx::get_time_seconds();
	out->pages.reserve(pages.size() + delta.size());
	
	std::unordered_map<uint32_t, float32_t> update_map(delta.size());
	for(const auto& entry : delta) {
		update_map[entry.first] = entry.second;
	}
	
	std::vector<std::pair<uint32_t, float32_t>> sorted;
	for(const auto& entry : update_map) {
		if(entry.first && entry.second >= 0) {
			sorted.emplace_back(entry);
		}
	}
	std::sort(sorted.begin(), sorted.end(), &sort_pair_second_desc);
	
	if(pages.empty()) {
		out->pages = sorted;
	} else {
		auto iter = pages.begin();
		auto iter2 = sorted.begin();
		while(true) {
			if(iter2 != sorted.end()) {
				if(iter == pages.end() || iter2->second >= iter->second) {
					out->pages.emplace_back(*iter2);
					iter2++;
					continue;
				}
			} else if(iter == pages.end()) {
				break;
			}
			if(!update_map.count(iter->first)) {
				out->pages.emplace_back(*iter);
			}
			iter++;
		}
	}
	return out;
}


} // search
} // vnx
