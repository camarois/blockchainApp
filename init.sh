#!/bin/bash

echo \
"#!/bin/bash

pushd server/
./clang.sh
if [ $? -ne 0 ]; then 
    exit 1
fi
popd
" | tee .git/hooks/pre-push
