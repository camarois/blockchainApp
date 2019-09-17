#!/bin/bash
#
# Usage: ./build.sh [JOB COUNT]

set -o xtrace

BUILD_DIRECTORY=$(dirname "$0")/build
mkdir -p "$BUILD_DIRECTORY"
cd "$BUILD_DIRECTORY" || exit

cmake -G "Unix Makefiles" ..
if [[ -z ${1+x} ]]; then
	make -j
else
	make -j $1
fi


