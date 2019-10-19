#!/bin/bash
#
# Usage: ./build.sh [JOB COUNT]

set -o xtrace

pushd "$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)" || exit 1
printf "O" | ./createDb.sh
popd || exit 1

BUILD_DIRECTORY=$(dirname "$0")/build
mkdir -p "$BUILD_DIRECTORY"
cd "$BUILD_DIRECTORY" || exit

cmake -GNinja -DCMAKE_BUILD_TYPE=Release ..
if [[ -z ${1+x} ]]; then
	ninja -j 4
else
	ninja -j "$1"
fi
