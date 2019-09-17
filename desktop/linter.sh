#!/bin/sh

find . -type f -iname "*.java" -print0 | xargs -0 java -jar config/checkstyle/checkstyle-8.24-all.jar -c config/checkstyle/checkstyle.xml
