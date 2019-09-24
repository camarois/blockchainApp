#!/bin/bash
#
# Usage: ./build.sh [JOB COUNT]

set -o xtrace

BUILD_DIRECTORY=$(dirname "$0")/build
mkdir -p "$BUILD_DIRECTORY"
pushd "$BUILD_DIRECTORY" || exit

cmake -G "Unix Makefiles" ..
if [[ -z ${1+x} ]]; then
	make -j5
else
	make -j$1
fi


