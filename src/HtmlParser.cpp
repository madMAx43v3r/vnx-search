/*
 * HtmlParser.cpp
 *
 *  Created on: Apr 1, 2020
 *      Author: mad
 */

#include <vnx/search/HtmlParser.h>
#include <vnx/search/Util.h>

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
	service_pipe = vnx::open_pipe(Hash64::rand(), this, 1000);		// need to block here since we are a bottleneck
	
	frontend = std::make_shared<CrawlFrontendClient>(frontend_server);
	
	set_timer_millis(1000, std::bind(&HtmlParser::update, this));
	
	update();
	
	Super::main();
}

void HtmlParser::update()
{
	// always try to connect so frontend can reach us (non-blocking)
	vnx::connect(service_pipe, vnx::get_pipe(frontend_server), 0);
	
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
				std::string link(href->get_value());
				result->links.push_back(clean(trim(link)));
			}
		}
		if(node->get_name() == "img") {
			const auto src = element->get_attribute("src");
			if(src) {
				std::string link(src->get_value());
				result->images.push_back(clean(trim(link)));
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
		auto text = dynamic_cast<const xmlpp::TextNode*>(node);
		if(text) {
			result->text += text->get_content();
			if(!text->is_white_space()) {
				result->text += " ";
			}
		}
	}
}

std::shared_ptr<const TextResponse>
HtmlParser::parse(const std::shared_ptr<const HttpResponse>& response) const
{
	auto result = TextResponse::create();
	result->Response::operator=(*response);
	
	const std::string payload((const char*)response->payload.data(), response->payload.size());
	
	xmlDoc* doc = ::htmlReadDoc((xmlChar*)payload.data(), 0, response->content_charset.c_str(),
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
				auto pos = content.find("url=");
				if(pos != std::string::npos) {
					result->links.push_back(content.substr(pos + 4));
				}
			}
		}
	}
	
	auto title = root->find("//head/title");
	if(!title.empty()) {
		auto element = dynamic_cast<const xmlpp::Element*>(title[0]);
		if(element) {
			auto text = element->get_child_text();
			if(text) {
				result->title = text->get_content();
				trim(result->title);
				clean(result->title);
				result->text += result->title;
				result->text += "\n\n";
			}
		}
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
