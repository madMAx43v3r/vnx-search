/*
 * Util.h
 *
 *  Created on: Apr 15, 2020
 *      Author: mad
 */

#ifndef INCLUDE_VNX_SEARCH_UTIL_H_
#define INCLUDE_VNX_SEARCH_UTIL_H_

#include <vector>
#include <string>

#include <url.h>


namespace vnx {
namespace search {

inline
std::string get_url_key(const Url::Url& url)
{
	Url::Url parsed(url);
	parsed.setScheme("");
	return parsed.str();
}

inline
std::string get_url_key(const std::string& url)
{
	return get_url_key(Url::Url(url));
}

inline
bool is_robots_txt(const Url::Url& url)
{
	return url.fullpath() == "/robots.txt";
}

inline
Url::Url process_url(const Url::Url& url)
{
	Url::Url parsed(url);
	parsed.defrag();
	if(parsed.scheme().empty()) {
		parsed.setScheme("https");
	}
	parsed.remove_default_port();
	parsed.strip();
	parsed.abspath();
	return parsed;
}

inline
Url::Url process_link(const Url::Url& link, const Url::Url& parent)
{
	Url::Url parsed(link);
	parsed.defrag();
	if(parsed.scheme().empty()) {
		parsed.setScheme(parent.scheme());
		if(parsed.host().empty()) {
			parsed.setHost(parent.host());
			// check if path is relative
			if(!parsed.path().empty() && parsed.path()[0] != '/') {
				parsed.relative_to(parent);
			}
		}
	}
	parsed.remove_default_port();
	parsed.strip();
	parsed.abspath();
	return parsed;
}

template<typename T>
std::vector<T> get_unique(std::vector<T> in)
{
	std::set<T> tmp(in.begin(), in.end());
	return std::vector<T>(tmp.begin(), tmp.end());
}

template<typename T, typename K, typename V>
void limited_emplace(T& queue, const K& key, const V& value, size_t limit)
{
	if(queue.size() < limit) {
		queue.emplace(key, value);
	} else {
		const auto back = std::prev(queue.end());
		typename T::key_compare compare;
		if(compare(key, back->first)) {
			queue.emplace(key, value);
			queue.erase(back);
		}
	}
}


} // search
} // vnx

#endif /* INCLUDE_VNX_SEARCH_UTIL_H_ */
