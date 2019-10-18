include_directories(tests PRIVATE
    ${COMMON_SRCS_DIR}/include
)

link_libraries(
    common
    sqlite3
    gmock
    gtest
    gtest_main
    pthread
    dl
)

add_executable(sqlite3_tests
    ${CMAKE_CURRENT_SOURCE_DIR}/database_test.cpp
)

add_test(sqlite3_tests sqlite3_tests)
