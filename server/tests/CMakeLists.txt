add_code_coverage_all_targets(EXCLUDE tests/* build/*)

function(create_test test_name)
    add_executable(${test_name} ${ARGN})
    target_code_coverage(${test_name} ALL EXCLUDE tests/* build/*)
    add_test(${test_name} ${test_name})
    add_custom_target(${test_name}_database
        # This will overrite the db
        COMMAND printf "O" | ${SQL_DIR}/createDb.sh ${test_name}.db
        BYPRODUCTS ${test_name}.db
    )
    add_dependencies(${test_name} ${test_name}_database)
endfunction()

include_directories(tests PRIVATE
    ${CMAKE_SOURCE_DIR}/rest/include
    ${CMAKE_SOURCE_DIR}/miner/include
    ${COMMON_SRCS_DIR}/include
)

link_libraries(
    ${EXTERNAL_LIBS_DIR}/libpistache.a
    ${EXTERNAL_LIBS_DIR}/libgflags.a
    ${EXTERNAL_LIBS_DIR}/libzmq.a
    ${EXTERNAL_LIBS_DIR}/libcurlpp.a
    ${OPENSSL_LIBRARIES}
    curl
    common
    sqlite3
    gmock
    gtest
    gtest_main
    pthread
    dl
    uuid
    stdc++fs
)

file(COPY data DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
SET(controller_files
    ${CMAKE_SOURCE_DIR}/rest/src/main_controller.cpp
    ${CMAKE_SOURCE_DIR}/rest/src/user_controller.cpp
    ${CMAKE_SOURCE_DIR}/rest/src/ping_controller.cpp
    ${CMAKE_SOURCE_DIR}/rest/src/transaction_controller.cpp
    ${CMAKE_SOURCE_DIR}/rest/src/info_controller.cpp
    ${CMAKE_SOURCE_DIR}/rest/src/file_controller.cpp
    ${CMAKE_SOURCE_DIR}/rest/src/admin_controller.cpp
    ${CMAKE_SOURCE_DIR}/rest/src/zmq.cpp
    ${CMAKE_SOURCE_DIR}/rest/src/custom_router.cpp
)

add_custom_target(deps_file_tests
    COMMAND cp
        ${SQL_DIR}/createDb.sh
        ${SQL_DIR}/createDb.sql
        ${CMAKE_CURRENT_BINARY_DIR}
)

create_test(database_test database_test.cpp)

create_test(main_controller_test
    main_controller_test.cpp
    ${controller_files}
)

create_test(info_controller_test
    info_controller_test.cpp
    ${controller_files}
)
add_dependencies(info_controller_test deps_file_tests)

create_test(file_controller_test
    file_controller_test.cpp
    ${controller_files}
)
add_dependencies(file_controller_test deps_file_tests)

create_test(admin_controller_test
    admin_controller_test.cpp
    ${controller_files}
)
add_dependencies(admin_controller_test deps_file_tests)

create_test(transaction_controller_test
    transaction_controller_test.cpp
    ${controller_files}
)
add_dependencies(transaction_controller_test deps_file_tests)

create_test(user_controller_test
    user_controller_test.cpp
    ${controller_files}
)
add_dependencies(user_controller_test deps_file_tests)

create_test(ping_controller_test
    ping_controller_test.cpp
    ${controller_files}
)
add_dependencies(ping_controller_test deps_file_tests)

# create_test(block_tests 
#     ${CMAKE_SOURCE_DIR}/miner/src/block.cpp
#     block_test.cpp
#     utils.cpp
# )

# create_test(blockchain_tests 
#     ${CMAKE_SOURCE_DIR}/miner/src/block.cpp
#     ${CMAKE_SOURCE_DIR}/miner/src/blockchain.cpp
#     blockchain_test.cpp
#     utils.cpp
# )

# create_test(zmq_tests 
#     ${CMAKE_SOURCE_DIR}/miner/src/block.cpp
#     ${CMAKE_SOURCE_DIR}/miner/src/blockchain.cpp
#     ${CMAKE_SOURCE_DIR}/miner/src/blockchain-controller.cpp
#     ${CMAKE_SOURCE_DIR}/miner/src/zmq.cpp
#     ${CMAKE_SOURCE_DIR}/rest/src/zmq.cpp
#     zmq_test.cpp
#     utils.cpp
# )
