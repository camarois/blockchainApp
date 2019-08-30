cmake_minimum_required (VERSION 3.10.0)
project (inf3995)

find_package(Git QUIET)
if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/../.git")
    message(STATUS "Submodule update")
    execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
                    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                    RESULT_VARIABLE GIT_SUBMOD_RESULT)
    message(FATAL_ERROR ${CMAKE_CURRENT_SOURCE_DIR})
    if(NOT GIT_SUBMOD_RESULT EQUAL "0")
        message(FATAL_ERROR "git submodule update --init failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
    endif()
endif()

if(NOT EXISTS "${PROJECT_SOURCE_DIR}/third_party/pistache/CMakeLists.txt")
    message(FATAL_ERROR "The submodules were not downloaded! GIT_SUBMODULE was turned off or failed. Please update submodules and try again.")
endif()

add_subdirectory(${PROJECT_SOURCE_DIR}/third_party/pistache)

# include_directories(${PROJECT_SOURCE_DIR}/third_party/pistache)

function(inf3995_example example_name)
    set(EXAMPLE_EXECUTABLE run_${example_name})
    set(EXAMPLE_SOURCE ${example_name}.cc)

    add_executable(${EXAMPLE_EXECUTABLE} ${EXAMPLE_SOURCE})
    target_link_libraries(${EXAMPLE_EXECUTABLE} inf3995_static)
endfunction()

# inf3995_example(hello_server)
# inf3995_example(http_server)
# inf3995_example(http_client)
inf3995_example(rest_server)
# inf3995_example(custom_header)
# inf3995_example(http_server_shutdown)

# set(RAPIDJSON_ROOT_DIR PATH "${PROJECT_SOURCE_DIR}/third-party/rapidjson")

# find_package(RapidJSON)
# if (RAPIDJSON_FOUND)
#     include_directories(${RAPIDJSON_INCLUDES})
#     inf3995_example(rest_description)
# else()
#     message(FATAL_ERROR "RapidJSON not found. Could not build rest_description example.")
# endif()
