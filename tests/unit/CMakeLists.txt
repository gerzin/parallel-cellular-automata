cmake_minimum_required(VERSION 3.10)

# set the project name and version
project(PCATests    VERSION 0.1
                                    DESCRIPTION "tests of the framework for building Cellular Automata"
                                    LANGUAGES CXX)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(OpenMP REQUIRED)

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
    add_compile_options(-Wall -Wextra -pedantic -Werror)
    # optimizations and debug informations
    add_compile_options(-g -O3)
    # if the compiler supports OpenMP, use the right flags
    if (${OPENMP_FOUND})
        add_compile_options(${OpenMP_CXX_FLAGS})
    endif()

endif()

set(unit_test_targets
    test_sequential_all
    test_operators
    test_library_imports
    test_sequential_automaton
    test_utilities
    test_sequential_leaks_valgrind
)

foreach(TARGET ${unit_test_targets})
    add_executable(${TARGET} ${TARGET}.cpp)
    target_link_libraries(${TARGET} parallelcellularautomata)
endforeach()