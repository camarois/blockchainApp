file (GLOB_RECURSE SOURCE_FILES "*.cc")
add_executable(miner ${SOURCE_FILES})
# target_include_directories(miner PRIVATE include ${CMAKE_SOURCE_DIR}/third_party/pistache/include)
target_link_libraries(miner PRIVATE common cppzmq-static)
