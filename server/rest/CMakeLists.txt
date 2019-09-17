file (GLOB_RECURSE REST_SOURCE_FILES "*.cc")
add_executable(rest ${REST_SOURCE_FILES})
target_include_directories(rest PRIVATE include)
target_link_libraries(rest PRIVATE common pistache_static)
