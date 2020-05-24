/*
 * test_url.cpp
 *
 *  Created on: May 24, 2020
 *      Author: mad
 */

#include <vnx/search/Util.h>

#include <iostream>


int main()
{
	const Url::Url parent("http://parent.com/folder/page.html");
	std::cout << "parent = '" << parent.str() << "'" << std::endl;
	{
		std::string link = "test.com";
		std::cout << "'" << link << "' => '" << vnx::search::process_link(Url::Url(link), parent).str() << "'" << std::endl;
	}
	{
		std::string link = "test.com/";
		std::cout << "'" << link << "' => '" << vnx::search::process_link(Url::Url(link), parent).str() << "'" << std::endl;
	}
	{
		std::string link = "www.test.com";
		std::cout << "'" << link << "' => '" << vnx::search::process_link(Url::Url(link), parent).str() << "'" << std::endl;
	}
	{
		std::string link = "/test.html";
		std::cout << "'" << link << "' => '" << vnx::search::process_link(Url::Url(link), parent).str() << "'" << std::endl;
	}
	{
		std::string link = "test.html";
		std::cout << "'" << link << "' => '" << vnx::search::process_link(Url::Url(link), parent).str() << "'" << std::endl;
	}
	{
		std::string link = "./test.html";
		std::cout << "'" << link << "' => '" << vnx::search::process_link(Url::Url(link), parent).str() << "'" << std::endl;
	}
	{
		std::string link = "../test.html";
		std::cout << "'" << link << "' => '" << vnx::search::process_link(Url::Url(link), parent).str() << "'" << std::endl;
	}
	{
		std::string link = "//test.com";
		std::cout << "'" << link << "' => '" << vnx::search::process_link(Url::Url(link), parent).str() << "'" << std::endl;
	}
	{
		std::string link = "//test.com/";
		std::cout << "'" << link << "' => '" << vnx::search::process_link(Url::Url(link), parent).str() << "'" << std::endl;
	}
	{
		std::string link = "http:/test.com/";
		std::cout << "'" << link << "' => '" << vnx::search::process_link(Url::Url(link), parent).str() << "'" << std::endl;
	}
	{
		std::string link = " http:/test.com/";
		std::cout << "'" << link << "' => '" << vnx::search::process_link(Url::Url(link), parent).str() << "'" << std::endl;
	}
	{
		std::string link = "http://test.com/";
		std::cout << "'" << link << "' => '" << vnx::search::process_link(Url::Url(link), parent).str() << "'" << std::endl;
	}
	
	
}


