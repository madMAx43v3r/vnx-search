#!/bin/bash

killall vnx_search_crawl

screen -S crawler -d -m ./build/vnx_search_crawl -c config/local/ -f :5555 -i

