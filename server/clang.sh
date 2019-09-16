#!/bin/bash
set -o xtrace

case "$1" in
tidy)
    clang-tidy src/*.cc -p build/src/
    # Exit if there are some c++ warnings
    if [ $? -ne 0 ]; then 
        exit 1
    fi
    ;;
format)
    clang-format -style=file src/*.cc -output-replacements-xml | grep -c "<replacement " > /dev/null
    # Exit if grep did find replacements.
    if [ $? -ne 1 ]; then 
        exit 1
    fi
    ;;
*)
    ./clang.sh tidy
    ./clang.sh format
    ;;
esac
