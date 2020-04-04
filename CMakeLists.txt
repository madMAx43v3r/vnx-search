cmake_minimum_required(VERSION 2.6)
project(vnx-search)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fPIC -pthread")

find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBXMLPP REQUIRED libxml++-2.6)

file(GLOB GENERATED_SOURCE_FILES generated/src/*.cpp)
file(GLOB URL_CPP_SOURCE_FILES lib/url-cpp/src/*.cpp)

add_library(vnx_search SHARED
	${GENERATED_SOURCE_FILES}
	${URL_CPP_SOURCE_FILES}
	src/CrawlFrontend.cpp
	src/HtmlParser.cpp
	src/PageProcessor.cpp
)

target_include_directories(vnx_search
	PUBLIC
		include
		generated/include
	PRIVATE
		lib/cpp-httplib
		lib/url-cpp/include
		${LIBXMLPP_INCLUDE_DIRS}
)
target_link_libraries(vnx_search vnx_keyvalue vnx_base ${LIBXMLPP_LIBRARIES} ssl crypto z)

add_executable(vnx_search_frontend src/vnx_search_frontend.cpp)
target_link_libraries(vnx_search_frontend vnx_search)

add_executable(vnx_search_html_parser src/vnx_search_html_parser.cpp)
target_link_libraries(vnx_search_html_parser vnx_search)

add_executable(vnx_search_backend src/vnx_search_backend.cpp)
target_link_libraries(vnx_search_backend vnx_search)

install(DIRECTORY include/ DESTINATION include)
install(DIRECTORY generated/include/ DESTINATION include)
install(DIRECTORY interface/ DESTINATION interface/vnx/keyvalue)

install(TARGETS vnx_search DESTINATION lib)
