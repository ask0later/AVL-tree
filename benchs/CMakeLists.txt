set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/libhayai/src)

add_executable(benchs benchs.cpp)
target_compile_features(benchs PUBLIC cxx_std_20)
target_link_libraries(benchs tree_lib)

target_include_directories(benchs PUBLIC ${INCLUDE_DIR})