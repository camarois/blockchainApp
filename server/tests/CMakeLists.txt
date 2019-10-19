file(GLOB_RECURSE TEST_SRC *.cpp)

add_executable(tests ${TEST_SRC})

target_include_directories(tests PRIVATE
    ${COMMON_SRCS_DIR}/include
)

target_link_libraries(tests PRIVATE
    common
    gmock
    gtest
    gtest_main
    pthread
)

add_test(tests COMMAND tests)
