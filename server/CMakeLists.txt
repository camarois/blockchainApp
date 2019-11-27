cmake_minimum_required(VERSION 3.10.0)


option(BUILD_ARMV7 "Build the project using ARMv7 toolchain" OFF)
if(BUILD_ARMV7)
    include(${CMAKE_SOURCE_DIR}/cmake/armv7_setup.cmake)
endif()

project(inf3995)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS true)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-write-strings")
set(SQL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/sql)
set(CERT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/cert)

include(ExternalProject)
include(cmake/third_party.cmake)

add_subdirectory(common)

set(COMMON_LIBS_DIR ${CMAKE_BINARY_DIR}/common)
set(COMMON_SRCS_DIR ${CMAKE_SOURCE_DIR}/common)

add_subdirectory(rest)
add_subdirectory(miner)

if(NOT BUILD_ARMV7)
    enable_testing()
    add_subdirectory(tests)
endif()

install(
    FILES ${CMAKE_BINARY_DIR}/rest/rest
          ${CMAKE_BINARY_DIR}/miner/miner
    DESTINATION usr/bin
    PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE
                GROUP_READ GROUP_EXECUTE
                WORLD_READ WORLD_EXECUTE
)

install(
    FILES ${CMAKE_BINARY_DIR}/rest/createCert.sh
          ${CMAKE_BINARY_DIR}/rest/createDb.sh
    DESTINATION usr/share/inf3995
    PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE
                GROUP_READ GROUP_EXECUTE
                WORLD_READ WORLD_EXECUTE
)

install(
    FILES ${CMAKE_BINARY_DIR}/rest/createDb.sql
    DESTINATION usr/share/inf3995
)

install(FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/systemd/inf3995-miner.service
    ${CMAKE_CURRENT_SOURCE_DIR}/systemd/inf3995-rest.service
    DESTINATION usr/lib/systemd/system
)
