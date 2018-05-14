#!/usr/bin/env bash

mkdir build
cd build
cmake ..
make
cd ..
rm -r build
bin/main
