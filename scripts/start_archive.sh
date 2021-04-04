#!/bin/bash

screen -S archive -d -m ./build/vnx_search_archive -c config/local/ config/archive/ -f :9999

