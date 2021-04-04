#!/bin/bash

screen -S engine_tunnel -d -m ./scripts/run_engine_tunnel.sh

screen -S backend -d -m ./build/vnx_search_backend -c config/local/
screen -S engine -d -m ./build/vnx_search_engine -c config/local/

