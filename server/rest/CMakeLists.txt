file (GLOB_RECURSE SOURCE_FILES "*.cpp")
add_executable(rest ${SOURCE_FILES})
target_include_directories(rest PRIVATE include ${CMAKE_SOURCE_DIR}/third_party/pistache/include)
target_link_libraries(rest PRIVATE common pistache_static)
