set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)

add_library(tree_lib INTERFACE)
target_include_directories(tree_lib INTERFACE ${INCLUDE_DIR})

add_executable(main main.cpp)
target_link_libraries(main tree_lib)

add_executable(main_set main_set.cpp)