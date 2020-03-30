cmake_minimum_required(VERSION 2.6)
project(vnx-search)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fPIC -pthread")

file(GLOB GENERATED_SOURCE_FILES generated/src/*.cpp)

add_library(vnx_search SHARED
	${GENERATED_SOURCE_FILES}
	src/CrawlFrontend.cpp
)

target_include_directories(vnx_search
	PUBLIC include generated/include
	PRIVATE lib/cpp-httplib
)
target_link_libraries(vnx_search vnx_base ssl crypto z)

add_executable(vnx_search_frontend src/vnx_search_frontend.cpp)
target_link_libraries(vnx_search_frontend vnx_search)

install(DIRECTORY include/ DESTINATION include)
install(DIRECTORY generated/include/ DESTINATION include)
install(DIRECTORY interface/ DESTINATION interface/vnx/keyvalue)

install(TARGETS vnx_search DESTINATION lib)
