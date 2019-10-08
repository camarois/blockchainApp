#!/bin/bash
# ./lint.sh [tidy|format] [fix]
set -o xtrace

function get_source_files() {
    local without_tests=$1
    local without_build_files=('-not' '-path' './build/*' '-not' '-path' './third_party/*')

    # TODO: test header files
    if [[ $without_tests = true ]]; then
        without_tests=('-not' '-path' './tests/*')
    else
        without_tests=()
    fi

    find . -name '*.cpp' "${without_tests[@]}" "${without_build_files[@]}"
}

function lint_files() {
    local fix_lint=$1
    local build_flags=('-p' './build')
    local only_headers=('-header-filter' 'common/|rest/|miner/')

    if [ -n "$fix_lint" ]; then
        fix_lint=('-fix')
    else
        fix_lint=()
    fi

    xargs run-clang-tidy-8 -quiet "${build_flags[@]}" "${only_headers[@]}" "${fix_lint[@]}"
}

function format_files() {
    local fix_format=$1
    local cmd

    if [ -n "$fix_format" ]; then
        fix_format=('-i')
        cmd=('false')
    else
        fix_format=('-output-replacements-xml')
        cmd=('grep' '<replacement ')
    fi

    ! xargs clang-format-8 -style=file "${fix_format[@]}" | "${cmd[@]}"
}

case "$1" in
tidy)
    get_source_files true | lint_files "$2"
    ;;
format)
    get_source_files false | format_files "$2"
    ;;
*)
    ./lint.sh tidy
    ./lint.sh format
    ;;
esac
