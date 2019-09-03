#!/bin/bash
set -o xtrace

mkdir -p build
pushd build
cmake -G "Unix Makefiles" ..
make -j $1
popd
