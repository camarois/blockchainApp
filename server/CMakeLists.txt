cmake_minimum_required(VERSION 3.10.0)
project(inf3995)

set(CMAKE_CXX_STANDARD 17)
add_compile_options(-Wextra -Wall -Wpedantic)

execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                RESULT_VARIABLE GIT_SUBMOD_RESULT)

add_subdirectory(third_party/pistache)
set(JSON_BuildTests OFF CACHE BOOL "Don't build JSON tests" FORCE)
add_subdirectory(third_party/nlohmann_json)
add_subdirectory(third_party/libzmq)
add_subdirectory(third_party/cppzmq)
add_subdirectory(third_party/googletest)

add_subdirectory(common)
add_subdirectory(rest)
add_subdirectory(miner)
add_subdirectory(tests)
