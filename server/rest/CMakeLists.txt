add_executable(rest rest_server.cc)
# target_compile_options(rest PRIVATE ${WARNING_FLAGS})
target_link_libraries(rest PRIVATE common)
