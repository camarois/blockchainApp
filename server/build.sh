#!/bin/bash
#
# Usage: ./build.sh [JOB COUNT]

set -o xtrace

BUILD_DIRECTORY=$(dirname "$0")/build
mkdir -p "$BUILD_DIRECTORY"
cd "$BUILD_DIRECTORY" || exit

cmake -GNinja -DCMAKE_BUILD_TYPE=Release ..
if [[ -z ${1+x} ]]; then
	ninja
else
	ninja -j "$1"
fi
