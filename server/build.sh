#!/bin/bash
#
# Usage: ./build.sh [JOB COUNT]

set -o xtrace

pushd "$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
printf "O" | ./createDb.sh
popd

BUILD_DIRECTORY=$(dirname "$0")/build
mkdir -p "$BUILD_DIRECTORY"
cd "$BUILD_DIRECTORY" || exit

cmake -GNinja -DCMAKE_BUILD_TYPE=Release ..
if [[ -z ${1+x} ]]; then
	ninja -j 4
else
	ninja -j $1
fi
