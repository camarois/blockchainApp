function(create_test test_name)
    add_executable(${test_name} ${ARGN})
    add_test(${test_name} ${test_name})
endfunction()

include_directories(tests PRIVATE
    ${CMAKE_SOURCE_DIR}/miner/include
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

file(COPY data DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

create_test(sqlite3_tests database_test.cpp)
add_custom_target(sqlite3_tests_database
    COMMAND rm -fv test-blockchain.db
    COMMAND ${SQL_DIR}/createDb.sh test-blockchain.db
    BYPRODUCTS test-blockchain.db
)
add_dependencies(sqlite3_tests sqlite3_tests_database)

create_test(block_tests 
    ${CMAKE_SOURCE_DIR}/miner/src/block.cpp
    block_test.cpp
    utils.cpp
)

create_test(blockchain_tests 
    ${CMAKE_SOURCE_DIR}/miner/src/block.cpp
    ${CMAKE_SOURCE_DIR}/miner/src/blockchain.cpp
    blockchain_test.cpp
    utils.cpp
)
