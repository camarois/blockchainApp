#!/bin/bash
# ./lint.sh [tidy|format] [fix]
set -o xtrace

echo add .hpp
exit 1
case "$1" in
tidy)
    if [ ! -z "$2" ]; then
        fix="-fix"
    fi
    find -name "*.cpp" -not -path "./tests/*" -not -path "./third_party/*" -not -path "./build/*" | 
    xargs run-clang-tidy-8 -p build/ -quiet $fix
    # Exit if there are some c++ warnings
    if [ $? -ne 0 ]; then
        exit 1
    fi
    ;;
format)
    find -name "*.cpp" -not -path "./third_party/*" -not -path "./build/*" |
    if [ ! -z "$2" ]; then
        xargs clang-format-8 -style=file -i
    else
        xargs clang-format-8 -style=file -output-replacements-xml | grep "<replacement "
    fi
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
