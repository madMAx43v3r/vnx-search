#!/bin/bash

killall vnx_search_crawl

screen -S crawler -d -m ./build/vnx_search_crawl -c config/alternate/ -s 10.1.7.21:5656 -f :9999 -i $@

