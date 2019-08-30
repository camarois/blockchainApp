function(inf3995_example example_name)
    set(EXAMPLE_EXECUTABLE run_${example_name})
    set(EXAMPLE_SOURCE ${example_name}.cc)

    add_executable(${EXAMPLE_EXECUTABLE} ${EXAMPLE_SOURCE})
    target_link_libraries(${EXAMPLE_EXECUTABLE} pistache_static)
endfunction()

inf3995_example(rest_server)
