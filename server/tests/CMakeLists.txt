include_directories(tests PRIVATE
    ${CMAKE_SOURCE_DIR}/miner/include
    ${COMMON_SRCS_DIR}/include
)

link_libraries(
    common
    gmock
    gtest
    gtest_main
    pthread
    stdc++fs
)

file(COPY data DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable(block_tests
    ${CMAKE_SOURCE_DIR}/miner/src/block.cpp
    block_test.cpp
    utils.cpp
)
add_test(block_tests block_tests)

add_executable(blockchain_tests
    ${CMAKE_SOURCE_DIR}/miner/src/block.cpp
    ${CMAKE_SOURCE_DIR}/miner/src/blockchain.cpp
    blockchain_test.cpp
    utils.cpp
)
add_test(blockchain_tests blockchain_tests)
