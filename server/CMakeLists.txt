cmake_minimum_required(VERSION 3.10.0)

option(BUILD_ARMV7 "Build the project using ARMv7 toolchain" OFF)
if(BUILD_ARMV7)
    include(${CMAKE_SOURCE_DIR}/cmake/armv7_setup.cmake)
    setup_armv7_toolchain()
endif()

project(inf3995)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS true)
# TODO add compile options only for our projects, exlude third_party
# add_compile_options(-Wextra -Wall -Wpedantic)

find_package(Git)
execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                RESULT_VARIABLE GIT_SUBMOD_RESULT)

add_subdirectory(third_party/curlpp)
add_subdirectory(third_party/pistache)

set(JSON_BuildTests OFF CACHE BOOL "Don't build tests" FORCE)
add_subdirectory(third_party/nlohmann_json)

set(BUILD_TESTS OFF CACHE BOOL "Don't build tests" FORCE)
set(ENABLE_DRAFTS OFF CACHE BOOL "Don't build tests" FORCE)
set(LIBZMQ_PEDANTIC OFF CACHE BOOL "Don't build tests" FORCE)
set(WITH_DOC OFF CACHE BOOL "Don't build tests" FORCE)
set(BUILD_SHARED OFF CACHE BOOL "Don't build tests" FORCE)
add_subdirectory(third_party/libzmq)

set(gmock_build_tests OFF CACHE BOOL "Don't build tests" FORCE)
set(gtest_build_tests OFF CACHE BOOL "Don't build tests" FORCE)
set(gtest_build_samples OFF CACHE BOOL "Don't build tests" FORCE)
set(gtest_disable_pthreads OFF CACHE BOOL "Don't build tests" FORCE)
add_subdirectory(third_party/googletest)

add_subdirectory(common)
add_subdirectory(rest)
add_subdirectory(miner)

if(NOT BUILD_ARMV7)
    add_subdirectory(tests)
endif()
