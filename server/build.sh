#!/bin/bash
#
# Usage: ./build.sh [JOB COUNT]

set -o xtrace

SCRIPT_DIR=$(realpath "$(dirname "${BASH_SOURCE[0]}")")

BUILD_DIRECTORY="$SCRIPT_DIR/build"
mkdir -p "$BUILD_DIRECTORY"
pushd "$BUILD_DIRECTORY" || exit 1
cmake -GNinja -DCMAKE_BUILD_TYPE=Debug -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ .. # Debug RelWithDebInfo Release
if [[ -z ${1+x} ]]; then
	ninja -j 4 || exit 1
else
	ninja -j "$1" || exit 1
fi
popd || exit 1
