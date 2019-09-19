file (GLOB_RECURSE SOURCE_FILES "*.cc")
add_executable(miner ${SOURCE_FILES})
target_include_directories(miner PRIVATE include
    ${CMAKE_SOURCE_DIR}/third_party/libzmp/include ${CMAKE_SOURCE_DIR}/third_party/cppzmq)
target_link_libraries(miner PRIVATE common libzmq-static)
