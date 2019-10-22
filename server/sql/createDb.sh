#!/usr/bin/env bash
#
# Usage: ./createDB.sh FILENAME
#
# The file already exists, then one of the following choices will be asked:
#   [O]verwrite project and test database
#   [B]ackup project database and recreate new project and test database
#   [C]ancel

set -o xtrace

SCRIPT_DIR=$(realpath "$(dirname "${BASH_SOURCE[0]}")")
STRUCT_FILE="$SCRIPT_DIR/createDb.sql"

function backup_db() {
    local file=$1

    i=1
    while [ -f "$file.auto_backup$i" ]; do
            ((i++))
    done
    mv "$file" "$file.auto_backup$i"
}

function ask_overwrite() {
    local file=$1

    echo -ne "$file already exists, [O]verwrite, [B]ackup and create, [C]ancel : "
    read -r -n 1 choice
    if   [ "$choice" = "O" ] || [ "$choice" = "o" ]; then
            rm -f "$file"
            return 1
    elif [ "$choice" = "B" ] || [ "$choice" = "b" ]; then
            backup_db "$file"
            return 1
    elif [ "$choice" = "C" ] || [ "$choice" = "c" ]; then
            return 0
    fi
}

function write_db() {
    local struct_file=$1
    local output_file=$2

    if [[ -f "$output_file" ]]; then
        if ask_overwrite "$output_file"; then
            return
        fi
    fi

    if [[ ! -f "$struct_file" ]]; then
        echo "file $struct_file couldn't be found"
    else
        sqlite3 "$output_file" < "$struct_file"
    fi
}

write_db "$STRUCT_FILE" "$1"
