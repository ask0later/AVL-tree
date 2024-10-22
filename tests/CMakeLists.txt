add_executable(tests tests.cpp)

target_link_libraries(tests GTest::gtest_main)
target_link_libraries(tests tree_lib)

target_include_directories(tests PRIVATE ${GMOCK_INCLUDE_DIRS} ${GTEST_INCLUDE_DIRS})