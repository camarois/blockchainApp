file(GLOB_RECURSE TEST_SRC *.cpp)
add_executable(tests ${TEST_SRC})
target_link_libraries(tests gtest gmock gtest_main common)
add_test(tests COMMAND tests)
