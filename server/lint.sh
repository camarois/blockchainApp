#!/bin/bash
set -o xtrace

case "$1" in
tidy)
    clang-tidy-8 src/*.cc -p build/src/ --quiet
    # Exit if there are some c++ warnings
    if [ $? -ne 0 ]; then 
        exit 1
    fi
    ;;
format)
    clang-format-8 -style=file src/*.cc -output-replacements-xml | grep "<replacement "
    # Exit if grep did find replacements.
    if [ $? -ne 1 ]; then 
        exit 1
    fi
    ;;
*)
    ./lint.sh tidy
    ./lint.sh format
    ;;
esac
