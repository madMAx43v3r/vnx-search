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
#include <algorithm>

#include <url.h>


namespace vnx {
namespace search {

extern const std::set<std::string> g_html_content_types;

// trim from left
inline
std::string& ltrim(std::string& s, const char* t = " \t\n\r\f\v")
{
    s.erase(0, s.find_first_not_of(t));
    return s;
}

// trim from right
inline
std::string& rtrim(std::string& s, const char* t = " \t\n\r\f\v")
{
    s.erase(s.find_last_not_of(t) + 1);
    return s;
}

// trim from left & right
inline
std::string& trim(std::string& s, const char* t = " \t\n\r\f\v")
{
    return ltrim(rtrim(s, t), t);
}

// remove chars from string
inline
std::string& clean(std::string& s, const char* t = "\n\r\f\v")
{
	while(true) {
		const auto pos = s.find_first_of(t);
		if(pos != std::string::npos) {
			s.erase(pos, 1);
		} else {
			break;
		}
	}
	return s;
}

// remove duplicate whitespace
std::string& clean_text(std::string& s);

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
std::vector<T> get_unique(const std::vector<T>& in)
{
	std::set<T> tmp(in.begin(), in.end());
	return std::vector<T>(tmp.begin(), tmp.end());
}

template<typename T>
std::vector<T>& append(std::vector<T>& vector, const std::vector<T>& new_elements)
{
	vector.insert(vector.end(), new_elements.begin(), new_elements.end());
	return vector;
}

template<typename T>
std::vector<T> concat(const std::vector<T>& A, const std::vector<T>& B)
{
	std::vector<T> vector;
	vector.reserve(A.size() + B.size());
	vector.insert(vector.end(), A.begin(), A.end());
	vector.insert(vector.end(), B.begin(), B.end());
	return vector;
}

template<typename T>
std::vector<T>& remove(std::vector<T>& vector, const T& element)
{
	const auto end = std::remove(vector.begin(), vector.end(), element);
	vector.erase(end, vector.end());
	return vector;
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
	}
	else if(!queue.empty()) {
		const auto back = std::prev(queue.end());
		typename T::key_compare compare;
		if(compare(key, back->first)) {
			queue.erase(back);
			queue.emplace(key, value);
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

std::vector<std::string> parse_text(const std::string& content, std::vector<std::pair<uint32_t, uint32_t>>* byte_positions = nullptr);


} // search
} // vnx

#endif /* INCLUDE_VNX_SEARCH_UTIL_H_ */
