cmake_minimum_required(VERSION 3.10)

# set the project name and version
project(PCATests    VERSION 0.1
                    DESCRIPTION "benchmarks of the framework for building Cellular Automata"
                    LANGUAGES CXX)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

#add_subdirectory(../ext/benchmark)

find_package(OpenMP REQUIRED)
find_package(benchmark PATHS ../ext/benchmark/build REQUIRED)

if (${OPENMP_FOUND})
    include_directories(${INCLUDE_DIRS})
endif()

include_directories(../../include ../ext ../ext/benchmark/include ./include)
link_directories(../../build)

# compile options
if (MSVC)
    # warning level 4 and all warnings as errors
    add_compile_options(/W4 /WX)
    # if the compiler supports OpenMP, use the right flags
    if (${OPENMP_FOUND})
        add_compile_options(${OpenMP_CXX_FLAGS})
    endif()
else()
    # lots of warnings and all warnings as errors
    add_compile_options(-Wall -Wextra -pedantic -Werror)
    if (NOT CMAKE_CXX_COMPILER_ID MATCHES "GNU")
        add_compile_options(-Wno-error=unused-command-line-argument)
    endif()
    # optimizations and debug informations
    add_compile_options(-g -O3)
    # if the compiler supports OpenMP, use the right flags
    if (${OPENMP_FOUND})
        add_compile_options(${OpenMP_CXX_FLAGS})
    endif()

endif()

set(
    BENCHMARK_TARGETS
    bench_sequential_automaton
    bench_omp_automaton
)

foreach(TARGET ${BENCHMARK_TARGETS})
    add_executable(${TARGET} ${TARGET}.cpp)
    target_link_libraries(${TARGET} parallelcellularautomata benchmark::benchmark)
endforeach()
