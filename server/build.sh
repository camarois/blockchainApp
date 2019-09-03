#!/bin/bash
#
# Usage: ./build.sh [JOB COUNT]

set -o xtrace

BUILD_DIRECTORY=$(dirname "$0")/build
mkdir -p "$BUILD_DIRECTORY"
cd "$BUILD_DIRECTORY" || exit

if [[ -z ${1+x} ]]; then
	JOB_COUNT=1
else
	JOB_COUNT=$1
fi

cmake -G "Unix Makefiles" ..
make -j "$JOB_COUNT"

