add_executable(tests ${CMAKE_CURRENT_SOURCE_DIR}/example_test.cpp)
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

add_executable(tests_sqlite3 ${CMAKE_CURRENT_SOURCE_DIR}/database_test.cpp)
target_include_directories(tests_sqlite3 PRIVATE
    ${SQLite3_INCLUDE_DIRS}
)

target_link_libraries(tests_sqlite3 PRIVATE
    ${SQLite3_LIBRARIES}
)

add_test(tests COMMAND tests)
add_test(tests_sqlite3 COMMAND tests)

