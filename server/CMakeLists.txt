cmake_minimum_required (VERSION 3.10.0)

option(BUILD_ARMV7 "Build the project using ARMv7 toolchain" OFF)
if(BUILD_ARMV7)
    include(${CMAKE_SOURCE_DIR}/cmake/armv7_setup.cmake)
    setup_armv7_toolchain()
endif()

project(inf3995)

find_package(Git QUIET)
if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/../.git")
    message(STATUS "Submodule update")
    execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
                    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                    RESULT_VARIABLE GIT_SUBMOD_RESULT)
    if(NOT GIT_SUBMOD_RESULT EQUAL "0")
        message(FATAL_ERROR "git submodule update --init failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
    endif()
endif()

if(NOT EXISTS "${PROJECT_SOURCE_DIR}/third_party/pistache/CMakeLists.txt")
    message(FATAL_ERROR "The submodules were not downloaded! GIT_SUBMODULE was turned off or failed. Please update submodules and try again.")
endif()

add_subdirectory(${PROJECT_SOURCE_DIR}/third_party/pistache)
include_directories(${pistache_SOURCE_DIR}/include ${pistache_SOURCE_DIR})

set(JSON_BuildTests OFF CACHE BOOL "Don't build JSON tests" FORCE)
add_subdirectory(${PROJECT_SOURCE_DIR}/third_party/nlohmann_json)
include_directories(${nlohmann_json_SOURCE_DIR}/include ${nlohmann_json_SOURCE_DIR})

add_subdirectory(${PROJECT_SOURCE_DIR}/src)
