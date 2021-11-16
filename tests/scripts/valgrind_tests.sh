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

for file in $files; do
    exitcode=$(valgrind --leak-check=full --error-exitcode=2 "./$file")
    if [ "$exitcode" -eq 2 ]; then
        echo "LEAKS DETECTED IN: $file"
        echo "EXITING"
        exit 1;
    fi
done

echo "NO LEAKS DETECTED"