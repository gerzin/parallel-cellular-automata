# Scripts
All the scripts in this folder are meant to be run from this working directory otherwise the relative paths won't work.

#### `get_benchmark_framework.sh`
This script downloads and compile the [google/benchmark](https://github.com/google/benchmark) framework.

If when running the script you get an error like
    ```
    error: use of the 'maybe_unused' attribute is a C++17 extension [-Werror,-Wc++17-extensions] [[maybe_unused]] bool first_iter = true;
    ```
simply add `set(CMAKE_CXX_STANDARD 17)` on line 17 of the `CMakeLists.txt` in the 
[benchmark folder](../ext/benchmark) that has already been downloaded by this script
and run it again.

#### `run_benchmarks.sh`
This script compiles and runs the benchmarks. You can edit it to run only some specific benchmarks. Now it is set to run `bench_all`. It will also write the output of the benchmark in `benchmarks/outputs` in json format.