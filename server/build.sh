#!/bin/bash

mkdir -p build
cd build
cmake -G "Unix Makefiles" ..
make -j && src/run_rest_server
