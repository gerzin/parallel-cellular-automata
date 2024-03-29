cmake_minimum_required(VERSION 3.10)

# set the project name and version
project(PCA     VERSION 0.1
                DESCRIPTION "framework for building Cellular Automata"
                LANGUAGES CXX)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_BUILD_TYPE Release)

#optimizing fastflow
execute_process(
    COMMAND yes
    COMMAND bash mapping_string.sh
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/include/fastflow/ff
    RESULT_VARIABLE FF_MS_RES
    ERROR_QUIET
)

find_package(OpenMP REQUIRED)


# compile options
if (MSVC)
    # warning level 4 and all warnings as errors
    add_compile_options(/W4 /WX)
    # speed optimization
    add_compile_options(/Ox)
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
    add_compile_options(-g -O3)
    # if the compiler supports OpenMP, use the right flags
    if (${OPENMP_FOUND})
        add_compile_options(${OpenMP_CXX_FLAGS})
        link_libraries(${OpenMP_CXX_FLAGS})
    endif()
endif()

add_library(parallelcellularautomata STATIC include/utimer.hpp src/utimer.cpp include/grid.hpp include/cellular_automata.hpp include/sequential_automaton.hpp include/omp_automaton.hpp include/parallel_automaton.hpp include/ff_automaton.hpp include/barrier.hpp src/barrier.cpp include/queues.hpp include/threadpool.hpp src/threadpool.cpp)
target_include_directories(parallelcellularautomata PUBLIC include)

