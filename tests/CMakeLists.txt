add_executable(tests tests.cpp)

target_link_libraries(tests GTest::gtest_main)
target_link_libraries(tests tree_lib)

include(GoogleTest)
gtest_discover_tests(tests)