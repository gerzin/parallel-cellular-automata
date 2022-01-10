cmake_minimum_required(VERSION 3.10)

# set the project name and version
project(PCATests    VERSION 0.1
                    DESCRIPTION "tests of the framework for building Cellular Automata"
                    LANGUAGES CXX)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_BUILD_TYPE Release)


find_package(OpenMP REQUIRED)

if (${OPENMP_FOUND})
    include_directories(${INCLUDE_DIRS})
endif()

include_directories(../../include ../ext)
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
    add_compile_options(-Wall -Wextra -pedantic -Werror -std=c++17)
    if (NOT CMAKE_CXX_COMPILER_ID MATCHES "GNU")
        add_compile_options(-Wno-error=unused-command-line-argument)
    endif()
    # optimizations and debug informations
    add_compile_options(-g -O3 -lpthread)
    # if the compiler supports OpenMP, use the right flags
    if (${OPENMP_FOUND})
        add_compile_options(${OpenMP_CXX_FLAGS})
        link_libraries(${OpenMP_CXX_FLAGS})
    endif()

endif()

set(unit_test_targets
    test_all
    test_library_imports
    test_sequential_automaton
    test_omp_automaton
    test_barrier
    test_queues_and_threadpool
    test_parallel_automaton
    test_grid
    test_ff_automaton
    test_same_computation
)

foreach(TARGET ${unit_test_targets})
    add_executable(${TARGET} ${TARGET}.cpp)
    target_link_libraries(${TARGET} parallelcellularautomata)
endforeach()
