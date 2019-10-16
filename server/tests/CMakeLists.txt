include_directories(tests PRIVATE
    ${COMMON_SRCS_DIR}/include
)

link_libraries(
    sqlite3
    common
    gmock
    gtest
    gtest_main
    pthread
)

add_executable(sqlite3_tests
    ${CMAKE_CURRENT_SOURCE_DIR}/database_test.cpp
)

add_test(sqlite3_tests sqlite3_tests)
