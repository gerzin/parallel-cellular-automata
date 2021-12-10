# Scripts
All the scripts in this folder are meant to be run from this working directory.

# Note
#### `get_benchmark_framework.sh`
This script downloads and compile the [google/benchmark](https://github.com/google/benchmark) framework.

If when running the script you get an error like
    ```
    error: use of the 'maybe_unused' attribute is a C++17 extension [-Werror,-Wc++17-extensions] [[maybe_unused]] bool first_iter = true;
    ```
simply add `set(CMAKE_CXX_STANDARD 17)` on line 17 of the `CMakeLists.txt` in the 
[benchmark folder](../ext/benchmark) that has already been downloaded by this script
and run it again.