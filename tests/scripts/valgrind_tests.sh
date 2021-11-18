#!/bin/bash

#
# Compile and run all the memory leak tests.
#

cd ../build || (mkdir ../build && cd ../build);

if [ ! -f Makefile ]; then
    cmake ..
fi



files=$(make help | grep valgrind | grep -v unit | cut -c 5-)

echo "TEST_FILES: $files"
echo "RUNNING make"
make "$files"
echo "RUNNING THE TEST FILES WITH valgrind"
RED='\033[0;31m'
NC='\033[0m' # No Color
for file in $files; do
    exitcode=$(valgrind -q --leak-check=full --error-exitcode=2 "./$file")
    if [ "$exitcode" -eq 2 ]; then
        echo "${RED}LEAKS DETECTED IN: $file ${NC}"
        echo "EXITING"
        exit 1;
    else
        echo "NO LEAKS DETECTED IN: $file"
    fi
done

echo "NO LEAKS DETECTED"
