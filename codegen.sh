#!/bin/bash

export VNX_INTERFACE_DIR=${VNX_INTERFACE_DIR:-/usr/interface}

cd $(dirname "$0")

./addons/codegen.sh
./keyvalue/codegen.sh

vnxcppcodegen --cleanup generated/ vnx.search interface/ modules/ ${VNX_INTERFACE_DIR}/vnx/ keyvalue/interface/
