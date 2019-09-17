cmake_minimum_required(VERSION 3.10.0)
project(inf3995)

set(CMAKE_CXX_STANDARD 17)
set(WARNING_FLAGS -Wextra -Wall -Wpedantic)

execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                RESULT_VARIABLE GIT_SUBMOD_RESULT)

add_subdirectory(third_party/pistache)
include_directories(third_party/pistache/include)

set(JSON_BuildTests OFF CACHE BOOL "Don't build JSON tests" FORCE)
add_subdirectory(third_party/nlohmann_json)
include_directories(third_party/nlohmann_json/include)

add_subdirectory(common)
include_directories(common/include)

add_subdirectory(rest)
