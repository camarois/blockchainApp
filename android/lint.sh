#!/bin/bash
echo "Running git pre-commit hook"

# check if on wsl or actual linux
if grep -qE "(Microsoft|WSL)" /proc/version &> /dev/null ; then
    if [ -z "$ANDROID_HOME" ]
    then
        echo "Please set ANDROID_HOME env variable"
        exit 1
    elif [ -z "$ANDROID_NDK_HOME" ]
    then
        echo "Please set ANDROID_NDK_HOME env variable"
        exit 1
    else
        # if in windows, "remove" local.properties
        mv local.properties local.properties.bak
        ./gradlew ktlint
        mv local.properties.bak local.properties
    fi
else
    ./gradlew ktlint
fi

# return 1 exit code if running checks fails
[ $? -ne 0 ] && exit 1
exit 0
