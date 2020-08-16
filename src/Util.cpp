/*
 * Util.cpp
 *
 *  Created on: Jul 14, 2020
 *      Author: mad
 */

#include <vnx/search/Util.h>

#include <unicode/utext.h>
#include <unicode/unistr.h>
#include <unicode/brkiter.h>
using namespace icu;

#include <iostream>


namespace vnx {
namespace search {

std::string& clean_text(std::string& s)
{
	clean(s);
	size_t pos = 0;
	bool last_space = true;
	while(pos < s.size()) {
		const auto is_space = ::isspace(s[pos]);
		if(is_space && last_space) {
			s.erase(pos, 1);
			continue;
		}
		last_space = is_space;
		pos++;
	}
	return s;
}

std::vector<std::string> parse_text(const std::string& content, std::vector<std::pair<uint32_t, uint32_t>>* byte_positions)
{
	std::vector<std::string> result;
	
	UText text = UTEXT_INITIALIZER;
	UErrorCode status = U_ZERO_ERROR;
	utext_openUTF8(&text, content.c_str(), content.size(), &status);
	if(status != U_ZERO_ERROR) {
		throw std::runtime_error("utext_openUTF8() failed!");
	}
	status = U_ZERO_ERROR;
	BreakIterator* bi = BreakIterator::createWordInstance(Locale::getUS(), status);
	if(status != U_ZERO_ERROR && status != U_USING_DEFAULT_WARNING) {
		throw std::runtime_error("BreakIterator::createWordInstance() failed!");
	}
	status = U_ZERO_ERROR;
	bi->setText(&text, status);
	if(status != U_ZERO_ERROR) {
		throw std::runtime_error("bi->setText() failed!");
	}
	
	UnicodeString uword;
	std::array<UChar, 2048> word;
	auto pos = bi->first();
	auto begin = pos;
	while(pos != BreakIterator::DONE) {
		begin = pos;
		pos = bi->next();
		if(pos != BreakIterator::DONE) {
			if(bi->getRuleStatus() != UBRK_WORD_NONE) {
				status = U_ZERO_ERROR;
				const auto num_uchars = utext_extract(&text, begin, pos, &word[0], word.size(), &status);
				if(status == U_ZERO_ERROR) {
					uword.setTo(&word[0], num_uchars);
					uword.toLower();
					std::string tmp;
					uword.toUTF8String(tmp);
					result.push_back(tmp);
					if(byte_positions) {
						byte_positions->emplace_back(begin, pos);
					}
				}
			}
		}
	}
	delete bi;
	utext_close(&text);
	return result;
}


} // search
} // vnx
