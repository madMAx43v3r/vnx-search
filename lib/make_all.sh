#!/bin/bash

set -e

cd libxmlplusplus
./autogen.sh
make clean
make -j8
cd ..

cd robotstxt
rm -rf build
mkdir -p build && cd build
cmake ..
make -j8
cd ..

