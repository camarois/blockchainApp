file (GLOB_RECURSE SOURCE_FILES "*.cpp")
add_executable(miner ${SOURCE_FILES})
target_include_directories(miner PRIVATE include)
target_link_libraries(miner PRIVATE common)
