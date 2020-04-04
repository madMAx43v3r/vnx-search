/*
 * PageProcessor.cpp
 *
 *  Created on: Apr 4, 2020
 *      Author: mad
 */

#include <vnx/search/PageProcessor.h>
#include <vnx/search/PageIndex.hxx>
#include <vnx/search/PageContent.hxx>

#include <unicode/unistr.h>
#include <unicode/brkiter.h>
using namespace icu;


namespace vnx {
namespace search {

PageProcessor::PageProcessor(const std::string& _vnx_name)
	:	PageProcessorBase(_vnx_name)
{
}

void PageProcessor::main()
{
	subscribe(input);
	
	page_index = std::make_shared<keyvalue::ServerAsyncClient>(page_index_server);
	page_content = std::make_shared<keyvalue::ServerAsyncClient>(page_content_server);
	
	page_index->vnx_set_error_callback(std::bind(&PageProcessor::page_index_error, this, std::placeholders::_1, std::placeholders::_2));
	page_content->vnx_set_error_callback(std::bind(&PageProcessor::page_content_error, this, std::placeholders::_1, std::placeholders::_2));
	
	add_async_client(page_index);
	add_async_client(page_content);
	
	Super::main();
}

void PageProcessor::handle(std::shared_ptr<const TextResponse> value)
{
	const UnicodeString text = UnicodeString::fromUTF8(value->text);
	
	UErrorCode status = U_ZERO_ERROR;
    BreakIterator* bi = BreakIterator::createWordInstance(Locale::getUS(), status);
    
    std::set<std::string> word_set;
    
    bi->setText(text);
    {
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
					word_set.insert(tmp);
				}
			}
		}
    }
	delete bi;
	
	auto index = PageIndex::create();
	index->last_modified = value->last_modified;
	index->links = value->links;
	index->images = value->images;
	for(const auto& word : word_set) {
		index->words.push_back(word);
	}
	
	try {
		page_index->store_value(value->url, index);
	}
	catch(const std::exception& ex) {
		log(WARN).out << "page_index->store_value(): " << ex.what();
		return;
	}
	
	auto content = PageContent::create();
	content->text = value->text;
	
	try {
		page_content->store_value(value->url, value);
	}
	catch(const std::exception& ex) {
		log(WARN).out << "page_index->store_value(): " << ex.what();
		return;
	}
	
	log(INFO).out << "Processed '" << value->url << "': " << index->words.size() << " index words";
}

void PageProcessor::page_index_error(uint64_t request_id, const std::exception& ex)
{
	log(WARN).out << "page_index->store_value(): " << ex.what();
}

void PageProcessor::page_content_error(uint64_t request_id, const std::exception& ex)
{
	log(WARN).out << "page_content->store_value(): " << ex.what();
}


} // search
} // vnx
