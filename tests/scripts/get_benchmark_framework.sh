#!/bin/bash
#
# Download and compile the framework for benchmarks
#
echo "This script will pull, compile and move in the right place the google benchmark framework. (It may take some minutes)"
read -p "Continue (y/n)?" choice
case "$choice" in 
  y|Y ) ;;
  n|N ) exit 1;;
  * ) echo "invalid option"; exit 1;;
esac
# Go into the benchmark folder
cd ../ext || exit 1
# Check out the library.
git clone https://github.com/google/benchmark.git
# Go to the library root directory
cd benchmark
# Make a build directory to place the build output.
cmake -E make_directory "build"
# Generate build system files with cmake, and download any dependencies.
cmake -E chdir "build" cmake -DBENCHMARK_DOWNLOAD_DEPENDENCIES=on -DCMAKE_BUILD_TYPE=Release ../
# or, starting with CMake 3.13, use a simpler form:
# cmake -DCMAKE_BUILD_TYPE=Release -S . -B "build"
# Build the library.
cmake --build "build" --config Release