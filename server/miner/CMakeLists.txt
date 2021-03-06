file(GLOB_RECURSE SOURCE_FILES "*.cpp")
list(FILTER SOURCE_FILES EXCLUDE REGEX ".ccls-cache")

add_custom_target(deps_file_miner
    COMMAND cp
        ${SQL_DIR}/createDb.sh
        ${SQL_DIR}/createDb.sql
        ${CMAKE_CURRENT_BINARY_DIR}
)

add_executable(miner ${SOURCE_FILES})
add_dependencies(miner common deps_file_miner libstduuid)

target_include_directories(miner PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${COMMON_SRCS_DIR}/include
)

if(BUILD_ARMV7)
    set(additional_libs
        -static-libgcc
        -static-libstdc++
    )
endif()

target_link_libraries(miner PRIVATE
    ${COMMON_LIBS_DIR}/libcommon.a
    ${EXTERNAL_LIBS_DIR}/libgflags.a
    ${EXTERNAL_LIBS_DIR}/libzmq.a
    ${EXTERNAL_LIBS_DIR}/libcurlpp.a
    curl
    pthread
    stdc++fs
    sqlite3
    uuid
    ${additional_libs}
)
