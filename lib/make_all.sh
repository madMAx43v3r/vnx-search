#!/bin/bash

set -e

cd libxmlplusplus
./autogen.sh
make -j8
cd ..

cd robotstxt
mkdir build && cd build
cmake ..
make -j8
cd ..

