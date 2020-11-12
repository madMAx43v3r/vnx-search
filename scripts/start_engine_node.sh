#!/bin/bash

screen -S frontend_tunnel -d -m ./scripts/run_frontend_tunnel.sh
screen -S engine_tunnel -d -m ./scripts/run_engine_tunnel.sh

screen -S backend -d -m ./build/vnx_search_backend -c config/local/
screen -S engine -d -m ./build/vnx_search_engine -c config/local/
screen -S crawler -d -m ./build/vnx_search_crawl -c config/local/ -f :5555 -i

