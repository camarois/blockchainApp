#!/bin/bash
# ./lint.sh [--tidy] [--format] [--fix] [--include-tests] [FILES]

function get_source_files() {
    local without_tests=$1
    local without_build_files=('-not' '-path' './build/*' '-not' '-path' './third_party/*')

    if [[ $without_tests -eq 1 ]]; then
        without_tests=('-not' '-path' './tests/*')
    else
        without_tests=()
    fi

    find . -name '*.cpp' "${without_tests[@]}" "${without_build_files[@]}"
}

function array_as_lines() {
    local array=("$@")

    for element in "${array[@]}"; do
        echo "$element"
    done 
}

function lint_files() {
    local fix_lint=0
    local build_flags=('-p' './build')
    local only_headers=('-header-filter' 'common/|rest/|miner/')

    if [[ "$fix_lint" -eq 1 ]]; then
        fix_lint=('-fix')
    else
        fix_lint=()
    fi

    xargs run-clang-tidy-8 -quiet "${build_flags[@]}" "${only_headers[@]}" "${fix_lint[@]}"
}

function format_files() {
    local fix_format=$1
    local cmd

    if [[ "$fix_format" -eq 1 ]]; then
        fix_format=('-i')
        cmd=('false')
    else
        fix_format=('-output-replacements-xml')
        cmd=('grep' '<replacement ')
    fi

    ! xargs clang-format-8 -style=file "${fix_format[@]}" | "${cmd[@]}"
}

DO_TIDY=0
DO_FORMAT=0
DO_FIX=0
DO_EXCLUDE_TESTS=1
FILES=()

while [[ $# -gt 0 ]]; do
    case "$1" in
        -t|--tidy)
            DO_TIDY=1
            shift
            ;;
        --format)
            DO_FORMAT=1
            shift
            ;;
        --fix)
            DO_FIX=1
            shift
            ;;
        --include-tests)
            DO_EXCLUDE_TESTS=0
            shift
            ;;
        *)
            FILES+=("$1")
            shift
            ;;
    esac
done

if [[ $DO_TIDY -eq 0 && $DO_FORMAT -eq 0 ]]; then
    DO_TIDY=1
    DO_FORMAT=1
fi

if [[ ${#FILES[@]} -eq 0 ]]; then
    FILENAMES=$(get_source_files $DO_EXCLUDE_TESTS)
else
    FILENAMES=$(array_as_lines "${FILES[@]}")
fi

if [[ $DO_TIDY -eq 1 ]]; then
    printf "%s" "$FILENAMES" | lint_files $DO_FIX
fi

if [[ $DO_FORMAT -eq 1 ]]; then
    printf "%s" "$FILENAMES" | format_files $DO_FIX
fi
