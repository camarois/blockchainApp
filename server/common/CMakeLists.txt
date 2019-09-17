add_library(common example.cc)
target_include_directories(common PUBLIC include)
target_link_libraries(common PUBLIC pistache_static)
