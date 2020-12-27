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
	frontend->register_parser_async(service_pipe->get_mac_addr(),
			std::vector<std::string>(g_html_content_types.begin(), g_html_content_types.end()), 1);
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
	if(auto element = dynamic_cast<const xmlpp::Element*>(node))
	{
		if(node->get_name() == "a") {
			page_link_t link;
			if(auto* href = element->get_attribute("href")) {
				std::string tmp(href->get_value());
				link.url = clean(trim(tmp));
			}
			if(auto* text = element->get_child_text()) {
				std::string tmp(text->get_content());
				link.text = clean(trim(tmp));
			}
			result->links.push_back(link);
		}
		if(node->get_name() == "img") {
			image_link_t link;
			if(auto* src = element->get_attribute("src")) {
				std::string tmp(src->get_value());
				link.url = clean(trim(tmp));
			}
			if(auto* alt = element->get_attribute("alt")) {
				std::string tmp(alt->get_value());
				link.text = clean(trim(tmp));
			}
			if(auto* width = element->get_attribute("width")) {
				vnx::from_string(width->get_value(), link.width);
			}
			if(auto* height = element->get_attribute("height")) {
				vnx::from_string(height->get_value(), link.height);
			}
			result->images.push_back(link);
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
		if(auto* text = dynamic_cast<const xmlpp::TextNode*>(node))
		{
			result->text += text->get_content();
			if(!text->is_white_space()) {
				result->text += " ";
			}
		}
	}
}

std::shared_ptr<const TextResponse>
HtmlParser::parse(std::shared_ptr<const HttpResponse> response) const
{
	auto result = TextResponse::create();
	result->Response::operator=(*response);
	result->base_url = response->url;
	
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
	
	const auto meta = root->find("//head/meta");
	for(auto node : meta) {
		if(auto element = dynamic_cast<const xmlpp::Element*>(node)) {
			if(element->get_attribute_value("http-equiv") == "Refresh") {
				const std::string content = element->get_attribute_value("content");
				auto pos = content.find("url=");
				if(pos != std::string::npos) {
					page_link_t link;
					link.url = content.substr(pos + 4);
					result->links.push_back(link);
				}
			}
		}
	}
	
	const auto title = root->find("//head/title");
	if(!title.empty()) {
		if(auto element = dynamic_cast<const xmlpp::Element*>(title[0])) {
			if(auto text = element->get_child_text()) {
				result->title = text->get_content();
				trim(result->title);
				clean(result->title);
				result->text += result->title;
				result->text += "\n\n";
			}
		}
	}
	
	const auto base = root->find("//head/base");
	if(!base.empty()) {
		if(auto element = dynamic_cast<const xmlpp::Element*>(base[0])) {
			if(auto href = element->get_attribute("href")) {
				result->base_url = href->get_value();
			}
		}
	}
	
	const auto body = root->find("//body");
	if(!body.empty()) {
		parse_node(body[0], result);
	}
	
	delete doc_pp;
	
	std::map<std::string, page_link_t> links;
	std::map<std::string, image_link_t> images;
	
	for(const auto& link : result->links) {
		auto& dst = links[link.url];
		dst.url = link.url;
		if(!link.text.empty()) {
			dst.text += (dst.text.empty() ? "" : " | ") + link.text;
		}
	}
	for(const auto& link : result->images) {
		auto& dst = images[link.url];
		dst.url = link.url;
		dst.width = std::max(dst.width, link.width);
		dst.height = std::max(dst.height, link.height);
		if(!link.text.empty()) {
			dst.text += (dst.text.empty() ? "" : " | ") + link.text;
		}
	}
	result->links.clear();
	result->images.clear();
	
	for(const auto& entry : links) {
		result->links.push_back(entry.second);
	}
	for(const auto& entry : images) {
		result->images.push_back(entry.second);
	}
	return result;
}


} // search
} // vnx
