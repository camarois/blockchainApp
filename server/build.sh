#!/bin/bash
#
# Usage: ./build.sh [JOB COUNT]

set -o xtrace

SCRIPT_DIR=$(realpath "$(dirname "${BASH_SOURCE[0]}")")

# printf "C" | "$SCRIPT_DIR/sql/createDb.sh" "$SCRIPT_DIR/sql/blockchain.db"

BUILD_DIRECTORY="$SCRIPT_DIR/build"
mkdir -p "$BUILD_DIRECTORY"
pushd "$BUILD_DIRECTORY" || exit 1
cmake -GNinja -DCMAKE_BUILD_TYPE=Debug .. # Debug RelWithDebInfo Release
if [[ -z ${1+x} ]]; then
	ninja -j 4
else
	ninja -j "$1"
fi
popd || exit 1
