set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)

add_library(tree_lib INTERFACE)
target_compile_features(tree_lib INTERFACE cxx_std_20)
target_include_directories(tree_lib INTERFACE ${INCLUDE_DIR})

add_executable(main main.cpp)
target_compile_features(main PUBLIC cxx_std_20)
target_link_libraries(main tree_lib)

add_executable(main_set main_set.cpp)
target_compile_features(main_set PUBLIC cxx_std_20)