#!/bin/bash
echo "This script will compile, run and save all the benchmarks"
read -p "Continue (y/n)?" choice
case "$choice" in 
  y|Y ) ;;
  n|N ) exit 1;;
  * ) echo "invalid"; exit 1;;
esac

echo "Moving into benchmark/build"
cd ../benchmarks
if [ ! -d build ]; then
    mkdir build
    cd build
else
    cd build
    rm -rf *
fi
echo "Compiling the benchmarks"
cmake ..
make -j

#BENCHMARKS=$(ls | grep bench)
BENCHMARKS=$(ls | grep bench_all)

for item in $BENCHMARKS; do
    ./$item --benchmark_out_format=json --benchmark_out=../outputs/$item.$USERNAME.$HOSTNAME.json
done