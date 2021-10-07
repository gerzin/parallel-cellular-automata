cmake_minimum_required(VERSION 3.10)

# set the project name and version
project(PCATests    VERSION 0.1
                                    DESCRIPTION "testing of the framework for building Cellular Automata"
                                    LANGUAGES CXX)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# compile options
if (MSVC)
    # warning level 4 and all warnings as errors
    add_compile_options(/W4 /WX)
else()
    # lots of warnings and all warnings as errors
    add_compile_options(-Wall -Wextra -pedantic -Werror)
    # optimizations and debug informations
    add_compile_options(-g -O3)
endif()
