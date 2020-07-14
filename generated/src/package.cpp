
// AUTO GENERATED by vnxcppcodegen

#include <vnx/search/ArchiveProxyBase.hxx>
#include <vnx/search/ContentParserBase.hxx>
#include <vnx/search/ContentParser_parse.hxx>
#include <vnx/search/ContentParser_parse_return.hxx>
#include <vnx/search/CrawlFrontendBase.hxx>
#include <vnx/search/CrawlFrontend_fetch.hxx>
#include <vnx/search/CrawlFrontend_fetch_return.hxx>
#include <vnx/search/CrawlFrontend_register_parser.hxx>
#include <vnx/search/CrawlFrontend_register_parser_return.hxx>
#include <vnx/search/CrawlProcessorBase.hxx>
#include <vnx/search/CrawlProcessor_get_stats.hxx>
#include <vnx/search/CrawlProcessor_get_stats_return.hxx>
#include <vnx/search/FetchResult.hxx>
#include <vnx/search/HttpResponse.hxx>
#include <vnx/search/PageContent.hxx>
#include <vnx/search/PageIndex.hxx>
#include <vnx/search/PageInfo.hxx>
#include <vnx/search/Response.hxx>
#include <vnx/search/SearchEngineBase.hxx>
#include <vnx/search/SearchEngine_get_domain_info.hxx>
#include <vnx/search/SearchEngine_get_domain_info_return.hxx>
#include <vnx/search/SearchEngine_get_domain_list.hxx>
#include <vnx/search/SearchEngine_get_domain_list_return.hxx>
#include <vnx/search/SearchEngine_get_page_info.hxx>
#include <vnx/search/SearchEngine_get_page_info_return.hxx>
#include <vnx/search/SearchEngine_query.hxx>
#include <vnx/search/SearchEngine_query_return.hxx>
#include <vnx/search/SearchEngine_reverse_domain_lookup.hxx>
#include <vnx/search/SearchEngine_reverse_domain_lookup_return.hxx>
#include <vnx/search/SearchEngine_reverse_lookup.hxx>
#include <vnx/search/SearchEngine_reverse_lookup_return.hxx>
#include <vnx/search/SearchEngine_suggest_domains.hxx>
#include <vnx/search/SearchEngine_suggest_domains_return.hxx>
#include <vnx/search/SearchEngine_suggest_words.hxx>
#include <vnx/search/SearchEngine_suggest_words_return.hxx>
#include <vnx/search/SearchResult.hxx>
#include <vnx/search/TextResponse.hxx>
#include <vnx/search/UrlIndex.hxx>
#include <vnx/search/UrlInfo.hxx>
#include <vnx/search/WordArray.hxx>
#include <vnx/search/WordContext.hxx>
#include <vnx/search/result_item_t.hxx>
#include <vnx/search/search_flags_e.hxx>

#include <vnx/vnx.h>



namespace vnx {
namespace search {


static void register_all_types() {
	vnx::register_type_code(::vnx::search::ArchiveProxyBase::static_create_type_code());
	vnx::register_type_code(::vnx::search::ContentParserBase::static_create_type_code());
	vnx::register_type_code(::vnx::search::ContentParser_parse::static_create_type_code());
	vnx::register_type_code(::vnx::search::ContentParser_parse_return::static_create_type_code());
	vnx::register_type_code(::vnx::search::CrawlFrontendBase::static_create_type_code());
	vnx::register_type_code(::vnx::search::CrawlFrontend_fetch::static_create_type_code());
	vnx::register_type_code(::vnx::search::CrawlFrontend_fetch_return::static_create_type_code());
	vnx::register_type_code(::vnx::search::CrawlFrontend_register_parser::static_create_type_code());
	vnx::register_type_code(::vnx::search::CrawlFrontend_register_parser_return::static_create_type_code());
	vnx::register_type_code(::vnx::search::CrawlProcessorBase::static_create_type_code());
	vnx::register_type_code(::vnx::search::CrawlProcessor_get_stats::static_create_type_code());
	vnx::register_type_code(::vnx::search::CrawlProcessor_get_stats_return::static_create_type_code());
	vnx::register_type_code(::vnx::search::FetchResult::static_create_type_code());
	vnx::register_type_code(::vnx::search::HttpResponse::static_create_type_code());
	vnx::register_type_code(::vnx::search::PageContent::static_create_type_code());
	vnx::register_type_code(::vnx::search::PageIndex::static_create_type_code());
	vnx::register_type_code(::vnx::search::PageInfo::static_create_type_code());
	vnx::register_type_code(::vnx::search::Response::static_create_type_code());
	vnx::register_type_code(::vnx::search::SearchEngineBase::static_create_type_code());
	vnx::register_type_code(::vnx::search::SearchEngine_get_domain_info::static_create_type_code());
	vnx::register_type_code(::vnx::search::SearchEngine_get_domain_info_return::static_create_type_code());
	vnx::register_type_code(::vnx::search::SearchEngine_get_domain_list::static_create_type_code());
	vnx::register_type_code(::vnx::search::SearchEngine_get_domain_list_return::static_create_type_code());
	vnx::register_type_code(::vnx::search::SearchEngine_get_page_info::static_create_type_code());
	vnx::register_type_code(::vnx::search::SearchEngine_get_page_info_return::static_create_type_code());
	vnx::register_type_code(::vnx::search::SearchEngine_query::static_create_type_code());
	vnx::register_type_code(::vnx::search::SearchEngine_query_return::static_create_type_code());
	vnx::register_type_code(::vnx::search::SearchEngine_reverse_domain_lookup::static_create_type_code());
	vnx::register_type_code(::vnx::search::SearchEngine_reverse_domain_lookup_return::static_create_type_code());
	vnx::register_type_code(::vnx::search::SearchEngine_reverse_lookup::static_create_type_code());
	vnx::register_type_code(::vnx::search::SearchEngine_reverse_lookup_return::static_create_type_code());
	vnx::register_type_code(::vnx::search::SearchEngine_suggest_domains::static_create_type_code());
	vnx::register_type_code(::vnx::search::SearchEngine_suggest_domains_return::static_create_type_code());
	vnx::register_type_code(::vnx::search::SearchEngine_suggest_words::static_create_type_code());
	vnx::register_type_code(::vnx::search::SearchEngine_suggest_words_return::static_create_type_code());
	vnx::register_type_code(::vnx::search::SearchResult::static_create_type_code());
	vnx::register_type_code(::vnx::search::TextResponse::static_create_type_code());
	vnx::register_type_code(::vnx::search::UrlIndex::static_create_type_code());
	vnx::register_type_code(::vnx::search::UrlInfo::static_create_type_code());
	vnx::register_type_code(::vnx::search::WordArray::static_create_type_code());
	vnx::register_type_code(::vnx::search::WordContext::static_create_type_code());
	vnx::register_type_code(::vnx::search::result_item_t::static_create_type_code());
	vnx::register_type_code(::vnx::search::search_flags_e::static_create_type_code());
}

static struct vnx_static_init {
	vnx_static_init() {
		register_all_types();
	}
} vnx_static_init_;

const vnx::TypeCode* const vnx_native_type_code_ArchiveProxyBase = vnx::get_type_code(vnx::Hash64(0x9e3c150e38d34ccaull));
const vnx::TypeCode* const vnx_native_type_code_ContentParserBase = vnx::get_type_code(vnx::Hash64(0xbe968e62c4bea207ull));
const vnx::TypeCode* const vnx_native_type_code_ContentParser_parse = vnx::get_type_code(vnx::Hash64(0x3a3496c5361fbf35ull));
const vnx::TypeCode* const vnx_native_type_code_ContentParser_parse_return = vnx::get_type_code(vnx::Hash64(0xa4f19c7005e2d444ull));
const vnx::TypeCode* const vnx_native_type_code_CrawlFrontendBase = vnx::get_type_code(vnx::Hash64(0xd91536edf3f184e2ull));
const vnx::TypeCode* const vnx_native_type_code_CrawlFrontend_fetch = vnx::get_type_code(vnx::Hash64(0xddf8de1011cf63d2ull));
const vnx::TypeCode* const vnx_native_type_code_CrawlFrontend_fetch_return = vnx::get_type_code(vnx::Hash64(0xce1d8d53217a282eull));
const vnx::TypeCode* const vnx_native_type_code_CrawlFrontend_register_parser = vnx::get_type_code(vnx::Hash64(0x4b91d9c8a161bbfdull));
const vnx::TypeCode* const vnx_native_type_code_CrawlFrontend_register_parser_return = vnx::get_type_code(vnx::Hash64(0x8ced9de5e282cceaull));
const vnx::TypeCode* const vnx_native_type_code_CrawlProcessorBase = vnx::get_type_code(vnx::Hash64(0x508848d1f9d97d9full));
const vnx::TypeCode* const vnx_native_type_code_CrawlProcessor_get_stats = vnx::get_type_code(vnx::Hash64(0x7b8fefbbb0397a0dull));
const vnx::TypeCode* const vnx_native_type_code_CrawlProcessor_get_stats_return = vnx::get_type_code(vnx::Hash64(0xb8530c2e4e418c47ull));
const vnx::TypeCode* const vnx_native_type_code_FetchResult = vnx::get_type_code(vnx::Hash64(0x82d345528415d31aull));
const vnx::TypeCode* const vnx_native_type_code_HttpResponse = vnx::get_type_code(vnx::Hash64(0xd6552db423d70e21ull));
const vnx::TypeCode* const vnx_native_type_code_PageContent = vnx::get_type_code(vnx::Hash64(0x4cd00be0870fbe22ull));
const vnx::TypeCode* const vnx_native_type_code_PageIndex = vnx::get_type_code(vnx::Hash64(0x4c9c9cf43a382f0ull));
const vnx::TypeCode* const vnx_native_type_code_PageInfo = vnx::get_type_code(vnx::Hash64(0x547cfd9b2bb19c80ull));
const vnx::TypeCode* const vnx_native_type_code_Response = vnx::get_type_code(vnx::Hash64(0xcf371c90803a778cull));
const vnx::TypeCode* const vnx_native_type_code_SearchEngineBase = vnx::get_type_code(vnx::Hash64(0x4e0f26d3496896a1ull));
const vnx::TypeCode* const vnx_native_type_code_SearchEngine_get_domain_info = vnx::get_type_code(vnx::Hash64(0xc775a7413dab0511ull));
const vnx::TypeCode* const vnx_native_type_code_SearchEngine_get_domain_info_return = vnx::get_type_code(vnx::Hash64(0xdd84ef2259be7eull));
const vnx::TypeCode* const vnx_native_type_code_SearchEngine_get_domain_list = vnx::get_type_code(vnx::Hash64(0x6852b566cb5e7ba5ull));
const vnx::TypeCode* const vnx_native_type_code_SearchEngine_get_domain_list_return = vnx::get_type_code(vnx::Hash64(0xa06392efe70f0679ull));
const vnx::TypeCode* const vnx_native_type_code_SearchEngine_get_page_info = vnx::get_type_code(vnx::Hash64(0x8f4b73a7bd7a8effull));
const vnx::TypeCode* const vnx_native_type_code_SearchEngine_get_page_info_return = vnx::get_type_code(vnx::Hash64(0xa79acda144d4c33bull));
const vnx::TypeCode* const vnx_native_type_code_SearchEngine_query = vnx::get_type_code(vnx::Hash64(0x14a6c3ff80018ce8ull));
const vnx::TypeCode* const vnx_native_type_code_SearchEngine_query_return = vnx::get_type_code(vnx::Hash64(0xc4adbd01e9504960ull));
const vnx::TypeCode* const vnx_native_type_code_SearchEngine_reverse_domain_lookup = vnx::get_type_code(vnx::Hash64(0x70967f585e137c7dull));
const vnx::TypeCode* const vnx_native_type_code_SearchEngine_reverse_domain_lookup_return = vnx::get_type_code(vnx::Hash64(0x8f0160842cd83465ull));
const vnx::TypeCode* const vnx_native_type_code_SearchEngine_reverse_lookup = vnx::get_type_code(vnx::Hash64(0x4e3b1cbd5cbd42afull));
const vnx::TypeCode* const vnx_native_type_code_SearchEngine_reverse_lookup_return = vnx::get_type_code(vnx::Hash64(0x12b287ac93bf5253ull));
const vnx::TypeCode* const vnx_native_type_code_SearchEngine_suggest_domains = vnx::get_type_code(vnx::Hash64(0x1abdd70e75f411b3ull));
const vnx::TypeCode* const vnx_native_type_code_SearchEngine_suggest_domains_return = vnx::get_type_code(vnx::Hash64(0x385aae88fe3df753ull));
const vnx::TypeCode* const vnx_native_type_code_SearchEngine_suggest_words = vnx::get_type_code(vnx::Hash64(0x7124d7bfda1b31f2ull));
const vnx::TypeCode* const vnx_native_type_code_SearchEngine_suggest_words_return = vnx::get_type_code(vnx::Hash64(0xcf74c35d3594cc90ull));
const vnx::TypeCode* const vnx_native_type_code_SearchResult = vnx::get_type_code(vnx::Hash64(0x659ce58d97581ddull));
const vnx::TypeCode* const vnx_native_type_code_TextResponse = vnx::get_type_code(vnx::Hash64(0x7cee1cd5b88ec569ull));
const vnx::TypeCode* const vnx_native_type_code_UrlIndex = vnx::get_type_code(vnx::Hash64(0xab26319d1802d1b6ull));
const vnx::TypeCode* const vnx_native_type_code_UrlInfo = vnx::get_type_code(vnx::Hash64(0xd69101803f781acfull));
const vnx::TypeCode* const vnx_native_type_code_WordArray = vnx::get_type_code(vnx::Hash64(0xabb8eb197035a035ull));
const vnx::TypeCode* const vnx_native_type_code_WordContext = vnx::get_type_code(vnx::Hash64(0xdaa75d84f367905bull));
const vnx::TypeCode* const vnx_native_type_code_result_item_t = vnx::get_type_code(vnx::Hash64(0x25b1d557d37a1bdfull));
const vnx::TypeCode* const vnx_native_type_code_search_flags_e = vnx::get_type_code(vnx::Hash64(0xe55b90a8ffab58ffull));

} // namespace vnx
} // namespace search
