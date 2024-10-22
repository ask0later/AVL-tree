cmake_minimum_required(VERSION 3.11)
project(AVL-tree)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(WITH_TESTS "tests" OFF)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=leak,address,undefined")

add_subdirectory(src)

if (WITH_TESTS)
    find_package(GTest CONFIG REQUIRED)

    message("Build binary file for UNIT tests ...")
    add_subdirectory(tests)
endif()