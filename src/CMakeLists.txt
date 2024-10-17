set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)

add_library(tree_lib tree.cpp)
target_include_directories(tree_lib PUBLIC ${INCLUDE_DIR})