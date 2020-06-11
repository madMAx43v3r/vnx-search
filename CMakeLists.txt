cmake_minimum_required(VERSION 3.8)
project(vnx-search)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -fopenmp -pthread")

find_package(PkgConfig)
find_package(LibXml2)

pkg_check_modules(PC_GLIBMM glibmm-2.4)

file(GLOB GENERATED_SOURCE_FILES generated/src/*.cpp)
file(GLOB URL_CPP_SOURCE_FILES lib/url-cpp/src/*.cpp)

add_library(xmlpp SHARED IMPORTED GLOBAL)
set_target_properties(xmlpp PROPERTIES
	IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/lib/libxmlplusplus/libxml++/.libs/libxml++-2.6.so)

add_library(robotstxt SHARED IMPORTED GLOBAL)
set_target_properties(robotstxt PROPERTIES
	IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/lib/robotstxt/build/librobots.so)

add_library(vnx_search SHARED
	${GENERATED_SOURCE_FILES}
	${URL_CPP_SOURCE_FILES}
	src/CrawlFrontend.cpp
	src/HtmlParser.cpp
	src/CrawlProcessor.cpp
	src/SearchEngine.cpp
	src/PlainTextParser.cpp
)

target_include_directories(vnx_search
	PUBLIC
		include
		generated/include
		lib/url-cpp/include
		lib/libstx/include
	PRIVATE
		lib/libxmlplusplus
		lib/robotstxt
		lib/robotstxt/build/libs/abseil-cpp-src
		${LIBXML2_INCLUDE_DIRS}
		${PC_GLIBMM_INCLUDE_DIRS}
)
target_link_libraries(vnx_search
	vnx_keyvalue vnx_base
	xmlpp robotstxt curl
	${LIBXML2_LIBRARIES}
	${PC_GLIBMM_LIBRARIES}
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

add_executable(vnx_search_query src/vnx_search_query.cpp)
target_link_libraries(vnx_search_query vnx_search)

add_executable(vnx_search_browse src/vnx_search_browse.cpp)
target_link_libraries(vnx_search_browse vnx_search)

add_executable(vnx_search_discover src/vnx_search_discover.cpp)
target_link_libraries(vnx_search_discover vnx_search)

add_executable(test_url test/test_url.cpp)
target_link_libraries(test_url vnx_search)

install(DIRECTORY include/ DESTINATION include)
install(DIRECTORY generated/include/ DESTINATION include)
install(DIRECTORY interface/ DESTINATION interface/vnx/keyvalue)

install(TARGETS vnx_search DESTINATION lib)
