#!/usr/bin/env bash
TESTS_PATH=../build
for testfile in $(ls $TESTS_PATH/test_*); do
    echo $testfile
    ./$testfile --rng-seed 69420  --abort 
done