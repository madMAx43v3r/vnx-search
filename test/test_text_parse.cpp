/*
 * test_text_parse.cpp
 *
 *  Created on: Jul 19, 2020
 *      Author: mad
 */

#include <vnx/search/Util.h>
#include <vnx/vnx.h>

#include <iostream>


int main()
{
	const std::string content = "This is a test, of some sort, 1337. More text is to follow.\nSome more text here.\n ";
	
	std::vector<uint32_t> positions;
	const auto words = vnx::search::parse_text(content, &positions);
	
	std::cout << vnx::to_string(words) << std::endl;
	
	std::vector<std::string> test;
	for(auto pos : positions) {
		test.push_back(content.substr(pos, 1));
	}
	std::cout << vnx::to_string(test) << std::endl;
	
	return 0;
}


