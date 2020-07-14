/*
 * Util.cpp
 *
 *  Created on: Jul 14, 2020
 *      Author: mad
 */

#include <vnx/search/Util.h>

#include <unicode/unistr.h>
#include <unicode/brkiter.h>
using namespace icu;


namespace vnx {
namespace search {

std::vector<std::string> parse_text(const std::string& content)
{
	std::vector<std::string> result;
	const UnicodeString text = UnicodeString::fromUTF8(content);
	
	UErrorCode status = U_ZERO_ERROR;
	BreakIterator* bi = BreakIterator::createWordInstance(Locale::getUS(), status);
	bi->setText(text);
	
	auto pos = bi->first();
	auto begin = pos;
	while(pos != BreakIterator::DONE) {
		begin = pos;
		pos = bi->next();
		if(pos != BreakIterator::DONE) {
			if(bi->getRuleStatus() != UBRK_WORD_NONE) {
				UnicodeString word;
				text.extractBetween(begin, pos, word);
				word.toLower();
				std::string tmp;
				word.toUTF8String(tmp);
				result.push_back(tmp);
			}
		}
	}
	delete bi;
	return result;
}


} // search
} // vnx
