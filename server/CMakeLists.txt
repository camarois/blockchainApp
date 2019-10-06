cmake_minimum_required(VERSION 3.10.0)

option(BUILD_ARMV7 "Build the project using ARMv7 toolchain" OFF)
if(BUILD_ARMV7)
    include(${CMAKE_SOURCE_DIR}/cmake/armv7_setup.cmake)
    set(TOOLCHAIN_DEFINES
        -DTOOLCHAIN_DIR=${TOOLCHAIN_DIR}
        -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}
    )
else()
    set(TOOLCHAIN_DEFINES
        -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}
    )
endif()

project(inf3995)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS true)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-write-strings")

include(ExternalProject)
include(cmake/third_party.cmake)

add_subdirectory(common)
set(COMMON_LIBS_DIR ${CMAKE_BINARY_DIR}/common)
set(COMMON_SRCS_DIR ${CMAKE_SOURCE_DIR}/common)

add_subdirectory(rest)
add_subdirectory(miner)

if(NOT BUILD_ARMV7)
    add_subdirectory(tests)
endif()
