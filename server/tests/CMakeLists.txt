function(create_test test_name test_file)
    add_executable(${test_name} ${CMAKE_CURRENT_SOURCE_DIR}/${test_file})
    target_include_directories(${test_name} PRIVATE
        ${COMMON_SRCS_DIR}/include
    )
    target_link_libraries(${test_name}
        common
        sqlite3
        gmock
        gtest
        gtest_main
        pthread
        dl
        stdc++fs
    )
    add_test(${test_name} ${test_name})
endfunction()

create_test(sqlite3_tests database_test.cpp)
add_custom_target(sqlite3_tests_database
    # This will overrite the db
    COMMAND printf "O" | ${SQL_DIR}/createDb.sh test-blockchain.db
    BYPRODUCTS test-blockchain.db
)
add_dependencies(sqlite3_tests sqlite3_tests_database)

create_test(example_tests example_test.cpp)
