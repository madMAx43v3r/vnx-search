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

#include <url.h>


namespace vnx {
namespace search {

PageProcessor::PageProcessor(const std::string& _vnx_name)
	:	PageProcessorBase(_vnx_name)
{
}

void PageProcessor::main()
{
	subscribe(input, max_queue_ms);
	
	page_index = std::make_shared<keyvalue::ServerAsyncClient>(page_index_server);
	page_content = std::make_shared<keyvalue::ServerAsyncClient>(page_content_server);
	
	page_index->vnx_set_error_callback(std::bind(&PageProcessor::page_index_error, this, std::placeholders::_1, std::placeholders::_2));
	page_content->vnx_set_error_callback(std::bind(&PageProcessor::page_content_error, this, std::placeholders::_1, std::placeholders::_2));
	
	add_async_client(page_index);
	add_async_client(page_content);
	
	Super::main();
}

static
std::string process_link(const std::string& link, const Url::Url& parent)
{
	Url::Url parsed(link);
	parsed.defrag();
	
	if(parsed.scheme().empty()) {
		parsed.setScheme(parent.scheme());
	}
	parsed.remove_default_port();
	
	if(parsed.host().empty()) {
		parsed.setHost(parent.host());
		// check if path is relative
		if(!parsed.path().empty() && parsed.path()[0] != '/') {
			parsed.relative_to(parent);
		}
	}
	
	parsed.strip();
	parsed.abspath();
	return parsed.str();
}

template<typename T>
std::vector<T> get_unique(std::vector<T> in)
{
	std::set<T> tmp(in.begin(), in.end());
	return std::vector<T>(tmp.begin(), tmp.end());
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
	
	const Url::Url parent(value->url);
	
	auto index = PageIndex::create();
	index->title = value->title;
	index->last_modified = value->last_modified;
	for(const auto& link : value->links) {
		const auto full_link = process_link(link, parent);
		if(full_link.size() <= max_url_length) {
			index->links.push_back(full_link);
		}
	}
	for(const auto& link : value->images) {
		const auto full_link = process_link(link, parent);
		if(full_link.size() <= max_url_length) {
			index->images.push_back(full_link);
		}
	}
	for(const auto& word : word_set) {
		index->words.push_back(word);
	}
	index->links = get_unique(index->links);
	index->images = get_unique(index->images);
	
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
		page_content->store_value(value->url, content);
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
