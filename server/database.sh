#!/usr/bin/env bash
THIS_SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null && pwd )"
SCRIPTS_DIR="${THIS_SCRIPT_DIR}"

DB_NAME="blockchain.db"
DB_NAME_TEST="test-blockchain.db"
EXISTING_DB="$DB_NAME"
STRUCT_FILE="sql/database.sql"

backup_db() {
    i=1
    while [ -f "$EXISTING_DB.auto_backup$i" ]; do
            ((i++))
    done
    mv "$EXISTING_DB" "sql/backups/$EXISTING_DB.auto_backup$i"
}

write_db() {
    if [ ! -f $STRUCT_FILE ]; then
            echo "no $STRUCT_FILE found at $SCRIPTS_DIR"
    else
            pushd $SCRIPTS_DIR > /dev/null 2>&1
            cat $STRUCT_FILE | sqlite3 $DB_NAME
            cat $STRUCT_FILE | sqlite3 $DB_NAME_TEST
            popd > /dev/null 2>&1
    fi
}

if [ -f $EXISTING_DB ]; then
    echo -ne "$DB_NAME exist, [O]verwrite, [B]ackup and create, [C]ancel : "
    read -n 1 choice
    if   [ "$choice" = "O" ] || [ "$choice" = "o" ];
    then
            rm -f $EXISTING_DB
            write_db
            echo ""
    elif [ "$choice" = "B" ] || [ "$choice" = "b" ];
    then
            backup_db
            write_db
            echo ""
    elif [ "$choice" = "C" ] || [ "$choice" = "c" ];
    then
            echo ""
            exit
    fi
else
        write_db
fi
