cmake_minimum_required(VERSION 3.11)
project(AVL-tree)

option(WITH_TESTS "tests" OFF)
option(WITH_BENCHMARKS "benchmarks" OFF)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=leak,address,undefined")

enable_testing()

add_subdirectory(src)

if (WITH_TESTS)
    find_package(GTest CONFIG REQUIRED)

    message("Build binary file for UNIT and End To End tests ...")
    add_subdirectory(tests)
endif()

if (WITH_BENCHMARKS)
    add_subdirectory(benchs)
endif()