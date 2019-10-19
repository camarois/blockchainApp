function(create_test test_name test_file)
    add_executable(${test_name} ${CMAKE_CURRENT_SOURCE_DIR}/${test_file})
    add_test(${test_name} ${test_name})
endfunction()

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
    stdc++fs
)

create_test(sqlite3_tests database_test.cpp)
create_test(example_tests example_test.cpp)
