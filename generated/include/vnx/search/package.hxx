
// AUTO GENERATED by vnxcppcodegen

#ifndef INCLUDE_vnx_search_PACKAGE_HXX_
#define INCLUDE_vnx_search_PACKAGE_HXX_

#include <vnx/Type.h>

#include <vnx/package.hxx>
#include <vnx/keyvalue/package.hxx>


namespace vnx {
namespace search {


class ContentParserBase;
class CrawlFrontendBase;
class CrawlProcessorBase;
class CrawlStats;
class HttpResponse;
class PageContent;
class PageIndex;
class PageProcessorBase;
class Response;
class SearchEngineBase;
class SearchResult;
class TextResponse;
class UrlIndex;
struct crawl_domain_stats_t;
class result_item_t;
struct search_flags_e;
extern const vnx::TypeCode* const vnx_native_type_code_ContentParser; ///< \private
extern const vnx::TypeCode* const vnx_native_type_code_ContentParser_parse; ///< \private
extern const vnx::TypeCode* const vnx_native_type_code_ContentParser_parse_return; ///< \private
extern const vnx::TypeCode* const vnx_native_type_code_CrawlFrontend; ///< \private
extern const vnx::TypeCode* const vnx_native_type_code_CrawlFrontend_fetch; ///< \private
extern const vnx::TypeCode* const vnx_native_type_code_CrawlFrontend_fetch_return; ///< \private
extern const vnx::TypeCode* const vnx_native_type_code_CrawlFrontend_handle_vnx_search_HttpResponse; ///< \private
extern const vnx::TypeCode* const vnx_native_type_code_CrawlFrontend_handle_vnx_search_HttpResponse_return; ///< \private
extern const vnx::TypeCode* const vnx_native_type_code_CrawlFrontend_register_parser; ///< \private
extern const vnx::TypeCode* const vnx_native_type_code_CrawlFrontend_register_parser_return; ///< \private
extern const vnx::TypeCode* const vnx_native_type_code_CrawlProcessor; ///< \private
extern const vnx::TypeCode* const vnx_native_type_code_CrawlProcessor_handle_vnx_keyvalue_KeyValuePair; ///< \private
extern const vnx::TypeCode* const vnx_native_type_code_CrawlProcessor_handle_vnx_keyvalue_KeyValuePair_return; ///< \private
extern const vnx::TypeCode* const vnx_native_type_code_CrawlStats; ///< \private
extern const vnx::TypeCode* const vnx_native_type_code_HttpResponse; ///< \private
extern const vnx::TypeCode* const vnx_native_type_code_PageContent; ///< \private
extern const vnx::TypeCode* const vnx_native_type_code_PageIndex; ///< \private
extern const vnx::TypeCode* const vnx_native_type_code_PageProcessor; ///< \private
extern const vnx::TypeCode* const vnx_native_type_code_PageProcessor_handle_vnx_search_TextResponse; ///< \private
extern const vnx::TypeCode* const vnx_native_type_code_PageProcessor_handle_vnx_search_TextResponse_return; ///< \private
extern const vnx::TypeCode* const vnx_native_type_code_Response; ///< \private
extern const vnx::TypeCode* const vnx_native_type_code_SearchEngine; ///< \private
extern const vnx::TypeCode* const vnx_native_type_code_SearchEngine_handle_vnx_keyvalue_KeyValuePair; ///< \private
extern const vnx::TypeCode* const vnx_native_type_code_SearchEngine_handle_vnx_keyvalue_KeyValuePair_return; ///< \private
extern const vnx::TypeCode* const vnx_native_type_code_SearchEngine_handle_vnx_keyvalue_SyncInfo; ///< \private
extern const vnx::TypeCode* const vnx_native_type_code_SearchEngine_handle_vnx_keyvalue_SyncInfo_return; ///< \private
extern const vnx::TypeCode* const vnx_native_type_code_SearchEngine_query; ///< \private
extern const vnx::TypeCode* const vnx_native_type_code_SearchEngine_query_return; ///< \private
extern const vnx::TypeCode* const vnx_native_type_code_SearchEngine_suggest_domains; ///< \private
extern const vnx::TypeCode* const vnx_native_type_code_SearchEngine_suggest_domains_return; ///< \private
extern const vnx::TypeCode* const vnx_native_type_code_SearchEngine_suggest_words; ///< \private
extern const vnx::TypeCode* const vnx_native_type_code_SearchEngine_suggest_words_return; ///< \private
extern const vnx::TypeCode* const vnx_native_type_code_SearchResult; ///< \private
extern const vnx::TypeCode* const vnx_native_type_code_TextResponse; ///< \private
extern const vnx::TypeCode* const vnx_native_type_code_UrlIndex; ///< \private
extern const vnx::TypeCode* const vnx_native_type_code_crawl_domain_stats_t; ///< \private
extern const vnx::TypeCode* const vnx_native_type_code_result_item_t; ///< \private
extern const vnx::TypeCode* const vnx_native_type_code_search_flags_e; ///< \private

} // namespace vnx
} // namespace search


namespace vnx {

void read(TypeInput& in, ::vnx::search::ContentParserBase& value, const TypeCode* type_code, const uint16_t* code); ///< \private
void read(TypeInput& in, ::vnx::search::CrawlFrontendBase& value, const TypeCode* type_code, const uint16_t* code); ///< \private
void read(TypeInput& in, ::vnx::search::CrawlProcessorBase& value, const TypeCode* type_code, const uint16_t* code); ///< \private
void read(TypeInput& in, ::vnx::search::CrawlStats& value, const TypeCode* type_code, const uint16_t* code); ///< \private
void read(TypeInput& in, ::vnx::search::HttpResponse& value, const TypeCode* type_code, const uint16_t* code); ///< \private
void read(TypeInput& in, ::vnx::search::PageContent& value, const TypeCode* type_code, const uint16_t* code); ///< \private
void read(TypeInput& in, ::vnx::search::PageIndex& value, const TypeCode* type_code, const uint16_t* code); ///< \private
void read(TypeInput& in, ::vnx::search::PageProcessorBase& value, const TypeCode* type_code, const uint16_t* code); ///< \private
void read(TypeInput& in, ::vnx::search::Response& value, const TypeCode* type_code, const uint16_t* code); ///< \private
void read(TypeInput& in, ::vnx::search::SearchEngineBase& value, const TypeCode* type_code, const uint16_t* code); ///< \private
void read(TypeInput& in, ::vnx::search::SearchResult& value, const TypeCode* type_code, const uint16_t* code); ///< \private
void read(TypeInput& in, ::vnx::search::TextResponse& value, const TypeCode* type_code, const uint16_t* code); ///< \private
void read(TypeInput& in, ::vnx::search::UrlIndex& value, const TypeCode* type_code, const uint16_t* code); ///< \private
void read(TypeInput& in, ::vnx::search::crawl_domain_stats_t& value, const TypeCode* type_code, const uint16_t* code); ///< \private
void read(TypeInput& in, ::vnx::search::result_item_t& value, const TypeCode* type_code, const uint16_t* code); ///< \private
void read(TypeInput& in, ::vnx::search::search_flags_e& value, const TypeCode* type_code, const uint16_t* code); ///< \private

void write(TypeOutput& out, const ::vnx::search::ContentParserBase& value, const TypeCode* type_code, const uint16_t* code); ///< \private
void write(TypeOutput& out, const ::vnx::search::CrawlFrontendBase& value, const TypeCode* type_code, const uint16_t* code); ///< \private
void write(TypeOutput& out, const ::vnx::search::CrawlProcessorBase& value, const TypeCode* type_code, const uint16_t* code); ///< \private
void write(TypeOutput& out, const ::vnx::search::CrawlStats& value, const TypeCode* type_code, const uint16_t* code); ///< \private
void write(TypeOutput& out, const ::vnx::search::HttpResponse& value, const TypeCode* type_code, const uint16_t* code); ///< \private
void write(TypeOutput& out, const ::vnx::search::PageContent& value, const TypeCode* type_code, const uint16_t* code); ///< \private
void write(TypeOutput& out, const ::vnx::search::PageIndex& value, const TypeCode* type_code, const uint16_t* code); ///< \private
void write(TypeOutput& out, const ::vnx::search::PageProcessorBase& value, const TypeCode* type_code, const uint16_t* code); ///< \private
void write(TypeOutput& out, const ::vnx::search::Response& value, const TypeCode* type_code, const uint16_t* code); ///< \private
void write(TypeOutput& out, const ::vnx::search::SearchEngineBase& value, const TypeCode* type_code, const uint16_t* code); ///< \private
void write(TypeOutput& out, const ::vnx::search::SearchResult& value, const TypeCode* type_code, const uint16_t* code); ///< \private
void write(TypeOutput& out, const ::vnx::search::TextResponse& value, const TypeCode* type_code, const uint16_t* code); ///< \private
void write(TypeOutput& out, const ::vnx::search::UrlIndex& value, const TypeCode* type_code, const uint16_t* code); ///< \private
void write(TypeOutput& out, const ::vnx::search::crawl_domain_stats_t& value, const TypeCode* type_code, const uint16_t* code); ///< \private
void write(TypeOutput& out, const ::vnx::search::result_item_t& value, const TypeCode* type_code, const uint16_t* code); ///< \private
void write(TypeOutput& out, const ::vnx::search::search_flags_e& value, const TypeCode* type_code, const uint16_t* code); ///< \private

void read(std::istream& in, ::vnx::search::ContentParserBase& value); ///< \private
void read(std::istream& in, ::vnx::search::CrawlFrontendBase& value); ///< \private
void read(std::istream& in, ::vnx::search::CrawlProcessorBase& value); ///< \private
void read(std::istream& in, ::vnx::search::CrawlStats& value); ///< \private
void read(std::istream& in, ::vnx::search::HttpResponse& value); ///< \private
void read(std::istream& in, ::vnx::search::PageContent& value); ///< \private
void read(std::istream& in, ::vnx::search::PageIndex& value); ///< \private
void read(std::istream& in, ::vnx::search::PageProcessorBase& value); ///< \private
void read(std::istream& in, ::vnx::search::Response& value); ///< \private
void read(std::istream& in, ::vnx::search::SearchEngineBase& value); ///< \private
void read(std::istream& in, ::vnx::search::SearchResult& value); ///< \private
void read(std::istream& in, ::vnx::search::TextResponse& value); ///< \private
void read(std::istream& in, ::vnx::search::UrlIndex& value); ///< \private
void read(std::istream& in, ::vnx::search::crawl_domain_stats_t& value); ///< \private
void read(std::istream& in, ::vnx::search::result_item_t& value); ///< \private
void read(std::istream& in, ::vnx::search::search_flags_e& value); ///< \private

void write(std::ostream& out, const ::vnx::search::ContentParserBase& value); ///< \private
void write(std::ostream& out, const ::vnx::search::CrawlFrontendBase& value); ///< \private
void write(std::ostream& out, const ::vnx::search::CrawlProcessorBase& value); ///< \private
void write(std::ostream& out, const ::vnx::search::CrawlStats& value); ///< \private
void write(std::ostream& out, const ::vnx::search::HttpResponse& value); ///< \private
void write(std::ostream& out, const ::vnx::search::PageContent& value); ///< \private
void write(std::ostream& out, const ::vnx::search::PageIndex& value); ///< \private
void write(std::ostream& out, const ::vnx::search::PageProcessorBase& value); ///< \private
void write(std::ostream& out, const ::vnx::search::Response& value); ///< \private
void write(std::ostream& out, const ::vnx::search::SearchEngineBase& value); ///< \private
void write(std::ostream& out, const ::vnx::search::SearchResult& value); ///< \private
void write(std::ostream& out, const ::vnx::search::TextResponse& value); ///< \private
void write(std::ostream& out, const ::vnx::search::UrlIndex& value); ///< \private
void write(std::ostream& out, const ::vnx::search::crawl_domain_stats_t& value); ///< \private
void write(std::ostream& out, const ::vnx::search::result_item_t& value); ///< \private
void write(std::ostream& out, const ::vnx::search::search_flags_e& value); ///< \private

void accept(Visitor& visitor, const ::vnx::search::ContentParserBase& value); ///< \private
void accept(Visitor& visitor, const ::vnx::search::CrawlFrontendBase& value); ///< \private
void accept(Visitor& visitor, const ::vnx::search::CrawlProcessorBase& value); ///< \private
void accept(Visitor& visitor, const ::vnx::search::CrawlStats& value); ///< \private
void accept(Visitor& visitor, const ::vnx::search::HttpResponse& value); ///< \private
void accept(Visitor& visitor, const ::vnx::search::PageContent& value); ///< \private
void accept(Visitor& visitor, const ::vnx::search::PageIndex& value); ///< \private
void accept(Visitor& visitor, const ::vnx::search::PageProcessorBase& value); ///< \private
void accept(Visitor& visitor, const ::vnx::search::Response& value); ///< \private
void accept(Visitor& visitor, const ::vnx::search::SearchEngineBase& value); ///< \private
void accept(Visitor& visitor, const ::vnx::search::SearchResult& value); ///< \private
void accept(Visitor& visitor, const ::vnx::search::TextResponse& value); ///< \private
void accept(Visitor& visitor, const ::vnx::search::UrlIndex& value); ///< \private
void accept(Visitor& visitor, const ::vnx::search::crawl_domain_stats_t& value); ///< \private
void accept(Visitor& visitor, const ::vnx::search::result_item_t& value); ///< \private
void accept(Visitor& visitor, const ::vnx::search::search_flags_e& value); ///< \private

/// \private
template<>
struct type<::vnx::search::ContentParserBase> {
	void read(TypeInput& in, ::vnx::search::ContentParserBase& value, const TypeCode* type_code, const uint16_t* code) {
		vnx::read(in, value, type_code, code);
	}
	void write(TypeOutput& out, const ::vnx::search::ContentParserBase& value, const TypeCode* type_code, const uint16_t* code) {
		vnx::write(out, value, type_code, code);
	}
	void read(std::istream& in, ::vnx::search::ContentParserBase& value) {
		vnx::read(in, value);
	}
	void write(std::ostream& out, const ::vnx::search::ContentParserBase& value) {
		vnx::write(out, value);
	}
	void accept(Visitor& visitor, const ::vnx::search::ContentParserBase& value) {
		vnx::accept(visitor, value);
	}
	void create_dynamic_code(std::vector<uint16_t>& code) {
		code.push_back(CODE_ANY);
	}
};

/// \private
template<>
struct type<::vnx::search::CrawlFrontendBase> {
	void read(TypeInput& in, ::vnx::search::CrawlFrontendBase& value, const TypeCode* type_code, const uint16_t* code) {
		vnx::read(in, value, type_code, code);
	}
	void write(TypeOutput& out, const ::vnx::search::CrawlFrontendBase& value, const TypeCode* type_code, const uint16_t* code) {
		vnx::write(out, value, type_code, code);
	}
	void read(std::istream& in, ::vnx::search::CrawlFrontendBase& value) {
		vnx::read(in, value);
	}
	void write(std::ostream& out, const ::vnx::search::CrawlFrontendBase& value) {
		vnx::write(out, value);
	}
	void accept(Visitor& visitor, const ::vnx::search::CrawlFrontendBase& value) {
		vnx::accept(visitor, value);
	}
	void create_dynamic_code(std::vector<uint16_t>& code) {
		code.push_back(CODE_ANY);
	}
};

/// \private
template<>
struct type<::vnx::search::CrawlProcessorBase> {
	void read(TypeInput& in, ::vnx::search::CrawlProcessorBase& value, const TypeCode* type_code, const uint16_t* code) {
		vnx::read(in, value, type_code, code);
	}
	void write(TypeOutput& out, const ::vnx::search::CrawlProcessorBase& value, const TypeCode* type_code, const uint16_t* code) {
		vnx::write(out, value, type_code, code);
	}
	void read(std::istream& in, ::vnx::search::CrawlProcessorBase& value) {
		vnx::read(in, value);
	}
	void write(std::ostream& out, const ::vnx::search::CrawlProcessorBase& value) {
		vnx::write(out, value);
	}
	void accept(Visitor& visitor, const ::vnx::search::CrawlProcessorBase& value) {
		vnx::accept(visitor, value);
	}
	void create_dynamic_code(std::vector<uint16_t>& code) {
		code.push_back(CODE_ANY);
	}
};

/// \private
template<>
struct type<::vnx::search::CrawlStats> {
	void read(TypeInput& in, ::vnx::search::CrawlStats& value, const TypeCode* type_code, const uint16_t* code) {
		vnx::read(in, value, type_code, code);
	}
	void write(TypeOutput& out, const ::vnx::search::CrawlStats& value, const TypeCode* type_code, const uint16_t* code) {
		vnx::write(out, value, type_code, code);
	}
	void read(std::istream& in, ::vnx::search::CrawlStats& value) {
		vnx::read(in, value);
	}
	void write(std::ostream& out, const ::vnx::search::CrawlStats& value) {
		vnx::write(out, value);
	}
	void accept(Visitor& visitor, const ::vnx::search::CrawlStats& value) {
		vnx::accept(visitor, value);
	}
	void create_dynamic_code(std::vector<uint16_t>& code) {
		code.push_back(CODE_ANY);
	}
};

/// \private
template<>
struct type<::vnx::search::HttpResponse> {
	void read(TypeInput& in, ::vnx::search::HttpResponse& value, const TypeCode* type_code, const uint16_t* code) {
		vnx::read(in, value, type_code, code);
	}
	void write(TypeOutput& out, const ::vnx::search::HttpResponse& value, const TypeCode* type_code, const uint16_t* code) {
		vnx::write(out, value, type_code, code);
	}
	void read(std::istream& in, ::vnx::search::HttpResponse& value) {
		vnx::read(in, value);
	}
	void write(std::ostream& out, const ::vnx::search::HttpResponse& value) {
		vnx::write(out, value);
	}
	void accept(Visitor& visitor, const ::vnx::search::HttpResponse& value) {
		vnx::accept(visitor, value);
	}
	void create_dynamic_code(std::vector<uint16_t>& code) {
		code.push_back(CODE_ANY);
	}
};

/// \private
template<>
struct type<::vnx::search::PageContent> {
	void read(TypeInput& in, ::vnx::search::PageContent& value, const TypeCode* type_code, const uint16_t* code) {
		vnx::read(in, value, type_code, code);
	}
	void write(TypeOutput& out, const ::vnx::search::PageContent& value, const TypeCode* type_code, const uint16_t* code) {
		vnx::write(out, value, type_code, code);
	}
	void read(std::istream& in, ::vnx::search::PageContent& value) {
		vnx::read(in, value);
	}
	void write(std::ostream& out, const ::vnx::search::PageContent& value) {
		vnx::write(out, value);
	}
	void accept(Visitor& visitor, const ::vnx::search::PageContent& value) {
		vnx::accept(visitor, value);
	}
	void create_dynamic_code(std::vector<uint16_t>& code) {
		code.push_back(CODE_ANY);
	}
};

/// \private
template<>
struct type<::vnx::search::PageIndex> {
	void read(TypeInput& in, ::vnx::search::PageIndex& value, const TypeCode* type_code, const uint16_t* code) {
		vnx::read(in, value, type_code, code);
	}
	void write(TypeOutput& out, const ::vnx::search::PageIndex& value, const TypeCode* type_code, const uint16_t* code) {
		vnx::write(out, value, type_code, code);
	}
	void read(std::istream& in, ::vnx::search::PageIndex& value) {
		vnx::read(in, value);
	}
	void write(std::ostream& out, const ::vnx::search::PageIndex& value) {
		vnx::write(out, value);
	}
	void accept(Visitor& visitor, const ::vnx::search::PageIndex& value) {
		vnx::accept(visitor, value);
	}
	void create_dynamic_code(std::vector<uint16_t>& code) {
		code.push_back(CODE_ANY);
	}
};

/// \private
template<>
struct type<::vnx::search::PageProcessorBase> {
	void read(TypeInput& in, ::vnx::search::PageProcessorBase& value, const TypeCode* type_code, const uint16_t* code) {
		vnx::read(in, value, type_code, code);
	}
	void write(TypeOutput& out, const ::vnx::search::PageProcessorBase& value, const TypeCode* type_code, const uint16_t* code) {
		vnx::write(out, value, type_code, code);
	}
	void read(std::istream& in, ::vnx::search::PageProcessorBase& value) {
		vnx::read(in, value);
	}
	void write(std::ostream& out, const ::vnx::search::PageProcessorBase& value) {
		vnx::write(out, value);
	}
	void accept(Visitor& visitor, const ::vnx::search::PageProcessorBase& value) {
		vnx::accept(visitor, value);
	}
	void create_dynamic_code(std::vector<uint16_t>& code) {
		code.push_back(CODE_ANY);
	}
};

/// \private
template<>
struct type<::vnx::search::Response> {
	void read(TypeInput& in, ::vnx::search::Response& value, const TypeCode* type_code, const uint16_t* code) {
		vnx::read(in, value, type_code, code);
	}
	void write(TypeOutput& out, const ::vnx::search::Response& value, const TypeCode* type_code, const uint16_t* code) {
		vnx::write(out, value, type_code, code);
	}
	void read(std::istream& in, ::vnx::search::Response& value) {
		vnx::read(in, value);
	}
	void write(std::ostream& out, const ::vnx::search::Response& value) {
		vnx::write(out, value);
	}
	void accept(Visitor& visitor, const ::vnx::search::Response& value) {
		vnx::accept(visitor, value);
	}
	void create_dynamic_code(std::vector<uint16_t>& code) {
		code.push_back(CODE_ANY);
	}
};

/// \private
template<>
struct type<::vnx::search::SearchEngineBase> {
	void read(TypeInput& in, ::vnx::search::SearchEngineBase& value, const TypeCode* type_code, const uint16_t* code) {
		vnx::read(in, value, type_code, code);
	}
	void write(TypeOutput& out, const ::vnx::search::SearchEngineBase& value, const TypeCode* type_code, const uint16_t* code) {
		vnx::write(out, value, type_code, code);
	}
	void read(std::istream& in, ::vnx::search::SearchEngineBase& value) {
		vnx::read(in, value);
	}
	void write(std::ostream& out, const ::vnx::search::SearchEngineBase& value) {
		vnx::write(out, value);
	}
	void accept(Visitor& visitor, const ::vnx::search::SearchEngineBase& value) {
		vnx::accept(visitor, value);
	}
	void create_dynamic_code(std::vector<uint16_t>& code) {
		code.push_back(CODE_ANY);
	}
};

/// \private
template<>
struct type<::vnx::search::SearchResult> {
	void read(TypeInput& in, ::vnx::search::SearchResult& value, const TypeCode* type_code, const uint16_t* code) {
		vnx::read(in, value, type_code, code);
	}
	void write(TypeOutput& out, const ::vnx::search::SearchResult& value, const TypeCode* type_code, const uint16_t* code) {
		vnx::write(out, value, type_code, code);
	}
	void read(std::istream& in, ::vnx::search::SearchResult& value) {
		vnx::read(in, value);
	}
	void write(std::ostream& out, const ::vnx::search::SearchResult& value) {
		vnx::write(out, value);
	}
	void accept(Visitor& visitor, const ::vnx::search::SearchResult& value) {
		vnx::accept(visitor, value);
	}
	void create_dynamic_code(std::vector<uint16_t>& code) {
		code.push_back(CODE_ANY);
	}
};

/// \private
template<>
struct type<::vnx::search::TextResponse> {
	void read(TypeInput& in, ::vnx::search::TextResponse& value, const TypeCode* type_code, const uint16_t* code) {
		vnx::read(in, value, type_code, code);
	}
	void write(TypeOutput& out, const ::vnx::search::TextResponse& value, const TypeCode* type_code, const uint16_t* code) {
		vnx::write(out, value, type_code, code);
	}
	void read(std::istream& in, ::vnx::search::TextResponse& value) {
		vnx::read(in, value);
	}
	void write(std::ostream& out, const ::vnx::search::TextResponse& value) {
		vnx::write(out, value);
	}
	void accept(Visitor& visitor, const ::vnx::search::TextResponse& value) {
		vnx::accept(visitor, value);
	}
	void create_dynamic_code(std::vector<uint16_t>& code) {
		code.push_back(CODE_ANY);
	}
};

/// \private
template<>
struct type<::vnx::search::UrlIndex> {
	void read(TypeInput& in, ::vnx::search::UrlIndex& value, const TypeCode* type_code, const uint16_t* code) {
		vnx::read(in, value, type_code, code);
	}
	void write(TypeOutput& out, const ::vnx::search::UrlIndex& value, const TypeCode* type_code, const uint16_t* code) {
		vnx::write(out, value, type_code, code);
	}
	void read(std::istream& in, ::vnx::search::UrlIndex& value) {
		vnx::read(in, value);
	}
	void write(std::ostream& out, const ::vnx::search::UrlIndex& value) {
		vnx::write(out, value);
	}
	void accept(Visitor& visitor, const ::vnx::search::UrlIndex& value) {
		vnx::accept(visitor, value);
	}
	void create_dynamic_code(std::vector<uint16_t>& code) {
		code.push_back(CODE_ANY);
	}
};

/// \private
template<>
struct type<::vnx::search::crawl_domain_stats_t> {
	void read(TypeInput& in, ::vnx::search::crawl_domain_stats_t& value, const TypeCode* type_code, const uint16_t* code) {
		vnx::read(in, value, type_code, code);
	}
	void write(TypeOutput& out, const ::vnx::search::crawl_domain_stats_t& value, const TypeCode* type_code, const uint16_t* code) {
		vnx::write(out, value, type_code, code);
	}
	void read(std::istream& in, ::vnx::search::crawl_domain_stats_t& value) {
		vnx::read(in, value);
	}
	void write(std::ostream& out, const ::vnx::search::crawl_domain_stats_t& value) {
		vnx::write(out, value);
	}
	void accept(Visitor& visitor, const ::vnx::search::crawl_domain_stats_t& value) {
		vnx::accept(visitor, value);
	}
	void create_dynamic_code(std::vector<uint16_t>& code) {
		code.push_back(CODE_ANY);
	}
};

/// \private
template<>
struct type<::vnx::search::result_item_t> {
	void read(TypeInput& in, ::vnx::search::result_item_t& value, const TypeCode* type_code, const uint16_t* code) {
		vnx::read(in, value, type_code, code);
	}
	void write(TypeOutput& out, const ::vnx::search::result_item_t& value, const TypeCode* type_code, const uint16_t* code) {
		vnx::write(out, value, type_code, code);
	}
	void read(std::istream& in, ::vnx::search::result_item_t& value) {
		vnx::read(in, value);
	}
	void write(std::ostream& out, const ::vnx::search::result_item_t& value) {
		vnx::write(out, value);
	}
	void accept(Visitor& visitor, const ::vnx::search::result_item_t& value) {
		vnx::accept(visitor, value);
	}
	void create_dynamic_code(std::vector<uint16_t>& code) {
		code.push_back(CODE_ANY);
	}
};

/// \private
template<>
struct type<::vnx::search::search_flags_e> {
	void read(TypeInput& in, ::vnx::search::search_flags_e& value, const TypeCode* type_code, const uint16_t* code) {
		vnx::read(in, value, type_code, code);
	}
	void write(TypeOutput& out, const ::vnx::search::search_flags_e& value, const TypeCode* type_code, const uint16_t* code) {
		vnx::write(out, value, type_code, code);
	}
	void read(std::istream& in, ::vnx::search::search_flags_e& value) {
		vnx::read(in, value);
	}
	void write(std::ostream& out, const ::vnx::search::search_flags_e& value) {
		vnx::write(out, value);
	}
	void accept(Visitor& visitor, const ::vnx::search::search_flags_e& value) {
		vnx::accept(visitor, value);
	}
	void create_dynamic_code(std::vector<uint16_t>& code) {
		code.push_back(CODE_ANY);
	}
};


} // namespace vnx

#endif // INCLUDE_vnx_search_PACKAGE_HXX_
