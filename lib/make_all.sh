#!/bin/bash

set -e

cd libxmlplusplus
./autogen.sh
make -j8
cd ..

cd robotstxt
mkdir c-build && cd c-build
cmake ..
make -j8
cd ..

