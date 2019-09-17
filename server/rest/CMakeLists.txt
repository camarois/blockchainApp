add_executable(rest rest.cc)
target_include_directories(rest PRIVATE include)
target_link_libraries(rest PRIVATE common pistache_static)
