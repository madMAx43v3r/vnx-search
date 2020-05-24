/*
 * Util.h
 *
 *  Created on: Apr 15, 2020
 *      Author: mad
 */

#ifndef INCLUDE_VNX_SEARCH_UTIL_H_
#define INCLUDE_VNX_SEARCH_UTIL_H_

#include <set>
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
	parsed.defrag();
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
	parsed.remove_default_port();
	parsed.strip();
	parsed.abspath();
	return parsed;
}

inline
Url::Url process_link(const Url::Url& link, const Url::Url& parent)
{
	Url::Url tmp(link);
	tmp.relative_to(parent);
	return process_url(tmp);
}

template<typename T>
std::vector<T> get_unique(std::vector<T> in)
{
	std::set<T> tmp(in.begin(), in.end());
	return std::vector<T>(tmp.begin(), tmp.end());
}

template<typename T>
bool unique_push_back(std::vector<T>& vector, const T& value)
{
	if(std::find(vector.begin(), vector.end(), value) == vector.end()) {
		vector.push_back(value);
		return true;
	}
	return false;
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

template<typename T>
T advance_until(T iter, const T& end, const ssize_t offset)
{
	if(offset > 0) {
		for(ssize_t i = 0; i < offset && iter != end; ++i) {
			iter++;
		}
	}
	return iter;
}


} // search
} // vnx

#endif /* INCLUDE_VNX_SEARCH_UTIL_H_ */
