cmake_minimum_required(VERSION 3.8)
project(vnx-search)

add_subdirectory(addons)
add_subdirectory(keyvalue)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -pthread -Wno-deprecated-declarations")

find_package(PkgConfig)
find_package(LibXml2)

find_library(JEMALLOC_LIBRARY jemalloc REQUIRED)

pkg_check_modules(PC_GLIBMM glibmm-2.4)

file(GLOB GENERATED_SOURCE_FILES generated/src/*.cpp)

add_library(xmlpp SHARED IMPORTED GLOBAL)
set_target_properties(xmlpp PROPERTIES
	IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/lib/libxmlplusplus/libxml++/.libs/libxml++-2.6.so)

add_library(robotstxt SHARED IMPORTED GLOBAL)
set_target_properties(robotstxt PROPERTIES
	IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/lib/robotstxt/build/librobots.so)

add_library(vnx_search SHARED
	${GENERATED_SOURCE_FILES}
	src/CrawlFrontend.cpp
	src/HtmlParser.cpp
	src/CrawlProcessor.cpp
	src/SearchEngine.cpp
	src/PlainTextParser.cpp
	src/PdfParser.cpp
	src/ArchiveProxy.cpp
	src/ArchiveServer.cpp
	src/SearchFrontend.cpp
	src/QueryEngine.cpp
	src/WordContext.cpp
	src/Util.cpp
	src/page_link_t.cpp
	src/image_link_t.cpp
	src/reverse_link_t.cpp
)

target_include_directories(vnx_search
	PUBLIC
		include
		generated/include
		lib/libstx/include
	PRIVATE
		lib/utf8.h
		lib/libxmlplusplus
		lib/robotstxt
		lib/robotstxt/build/libs/abseil-cpp-src
		${LIBXML2_INCLUDE_DIRS}
		${PC_GLIBMM_INCLUDE_DIRS}
)
target_link_libraries(vnx_search
	vnx_keyvalue vnx_addons vnx_base
	url_cpp xmlpp robotstxt curl
	${LIBXML2_LIBRARIES}
	${PC_GLIBMM_LIBRARIES}
	${JEMALLOC_LIBRARY}
)

add_executable(vnx_search_frontend src/vnx_search_frontend.cpp)
target_link_libraries(vnx_search_frontend vnx_search)

add_executable(vnx_search_html_parser src/vnx_search_html_parser.cpp)
target_link_libraries(vnx_search_html_parser vnx_search)

add_executable(vnx_search_backend src/vnx_search_backend.cpp)
target_link_libraries(vnx_search_backend vnx_keyvalue vnx_base)

add_executable(vnx_search_crawl src/vnx_search_crawl.cpp)
target_link_libraries(vnx_search_crawl vnx_search)

add_executable(vnx_search_engine src/vnx_search_engine.cpp)
target_link_libraries(vnx_search_engine vnx_search)

add_executable(vnx_search_query_engine src/vnx_search_query_engine.cpp)
target_link_libraries(vnx_search_query_engine vnx_search)

add_executable(vnx_search_query src/vnx_search_query.cpp)
target_link_libraries(vnx_search_query vnx_search)

add_executable(vnx_search_browse src/vnx_search_browse.cpp)
target_link_libraries(vnx_search_browse vnx_search)

add_executable(vnx_search_discover src/vnx_search_discover.cpp)
target_link_libraries(vnx_search_discover vnx_search)

add_executable(vnx_search_archive src/vnx_search_archive.cpp)
target_link_libraries(vnx_search_archive vnx_search)

add_executable(vnx_search_httpd src/vnx_search_httpd.cpp)
target_link_libraries(vnx_search_httpd vnx_search)

add_executable(test_url test/test_url.cpp)
target_link_libraries(test_url vnx_search)

add_executable(test_text_parse test/test_text_parse.cpp)
target_link_libraries(test_text_parse vnx_search)

install(DIRECTORY include/ DESTINATION include)
install(DIRECTORY generated/include/ DESTINATION include)
install(DIRECTORY interface/ DESTINATION interface/vnx/keyvalue)

install(TARGETS vnx_search DESTINATION lib)
