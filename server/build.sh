#!/bin/bash
set -o xtrace

mkdir -p build
pushd build
cmake -G "Unix Makefiles" ..
make -j $1 && src/run_rest_server
popd
