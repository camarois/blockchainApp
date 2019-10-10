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

add_executable(block_tests
    ${CMAKE_SOURCE_DIR}/miner/src/block.cpp
    block_test.cpp
)
add_test(block_tests block_tests)
