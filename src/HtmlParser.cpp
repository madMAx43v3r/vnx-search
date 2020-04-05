/*
 * HtmlParser.cpp
 *
 *  Created on: Apr 1, 2020
 *      Author: mad
 */

#include <vnx/search/HtmlParser.h>

#include <libxml/HTMLparser.h>
#include <libxml++/libxml++.h>


namespace vnx {
namespace search {

HtmlParser::HtmlParser(const std::string& _vnx_name)
	:	ContentParserBase(_vnx_name)
{
}

void HtmlParser::main()
{
	service_pipe = vnx::open_pipe(Hash64::rand(), this, max_queue_ms);
	
	frontend = std::make_shared<CrawlFrontendClient>(frontend_server);
	
	set_timer_millis(1000, std::bind(&HtmlParser::update, this));
	
	Super::main();
}

void HtmlParser::update()
{
	// always try to connect so frontend can reach us
	vnx::connect(service_pipe, vnx::get_pipe(frontend_server));
	
	frontend->register_parser_async(service_pipe->get_mac_addr(),
			{"text/html", "text/xml", "application/xml", "application/xhtml+xml"}, 1);
}

static void parse_node(const xmlpp::Node* node, std::shared_ptr<TextResponse> result)
{
	auto parent = node->get_parent();
	const std::string parent_name = parent ? parent->get_name() : "";
	
	if(node->get_name() == "p") {
		result->text += "\n";
	}
	for(auto child : node->get_children()) {
		parse_node(child, result);
	}
	auto element = dynamic_cast<const xmlpp::Element*>(node);
	if(element) {
		if(node->get_name() == "a") {
			const auto href = element->get_attribute("href");
			if(href) {
				result->links.push_back(href->get_value());
			}
		}
		if(node->get_name() == "img") {
			const auto src = element->get_attribute("src");
			if(src) {
				result->images.push_back(src->get_value());
			}
		}
	}
	if(node->get_name() == "br") {
		result->text += "\n";
	}
	if(node->get_name() == "p") {
		result->text += "\n";
	}
	if(parent_name != "script" && parent_name != "style")
	{
		auto content = dynamic_cast<const xmlpp::ContentNode*>(node);
		if(content) {
			result->text += content->get_content();
			result->text += " ";
		}
	}
}

template<typename T>
std::vector<T> get_unique(std::vector<T> in)
{
	std::set<T> tmp(in.begin(), in.end());
	return std::vector<T>(tmp.begin(), tmp.end());
}

std::shared_ptr<const TextResponse>
HtmlParser::parse(const std::shared_ptr<const HttpResponse>& response) const
{
	auto result = TextResponse::create();
	result->Response::operator=(*response);
	
	xmlDoc* doc = ::htmlReadDoc((xmlChar*)response->payload.data(), 0, 0,
			HTML_PARSE_RECOVER | HTML_PARSE_NOERROR | HTML_PARSE_NOWARNING);
	
	if(!doc) {
		throw std::runtime_error("htmlReadDoc() failed");
	}
	
	xmlpp::Document* doc_pp = new xmlpp::Document(doc);
	xmlpp::Element* root = doc_pp->get_root_node();
	
	if(!root) {
		throw std::runtime_error("get_root_node() failed");
	}
	
	auto meta = root->find("//head/meta");
	for(auto node : meta) {
		auto element = dynamic_cast<const xmlpp::Element*>(node);
		if(element) {
			if(element->get_attribute_value("http-equiv") == "Refresh") {
				const std::string content = element->get_attribute_value("content");
				auto pos = content.find_first_of("url=");
				if(pos != std::string::npos) {
					result->links.push_back(content.substr(pos + 4));
				}
			}
		}
	}
	
	auto title = root->find("//head/title");
	if(!title.empty()) {
		parse_node(title[0], result);
		result->text += "\n\n";
	}
	
	auto body = root->find("//body");
	if(!body.empty()) {
		parse_node(body[0], result);
	}
	
	delete doc_pp;
	
	result->links = get_unique(result->links);
	result->images = get_unique(result->images);
	
	return result;
}


} // search
} // vnx
