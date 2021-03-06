file(GLOB_RECURSE SOURCE_FILES "*.cpp")
list(FILTER SOURCE_FILES EXCLUDE REGEX ".ccls-cache")

add_custom_target(deps_file_rest
    COMMAND cp
        ${SQL_DIR}/createDb.sh
        ${SQL_DIR}/createDb.sql
        ${CMAKE_CURRENT_BINARY_DIR}

    COMMAND cp
        ${CERT_DIR}/createCert.sh
        ${CERT_DIR}/rootCA.crt
        ${CERT_DIR}/rootCA.key
        ${CMAKE_CURRENT_BINARY_DIR}
)

add_executable(rest ${SOURCE_FILES})
add_dependencies(rest common libpistache libstduuid deps_file_rest)

target_include_directories(rest PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${COMMON_SRCS_DIR}/include
)

if(BUILD_ARMV7)
    set(additional_libs
        -static-libgcc
        -static-libstdc++
    )
endif()

target_link_libraries(rest PRIVATE
    ${COMMON_LIBS_DIR}/libcommon.a
    ${EXTERNAL_LIBS_DIR}/libpistache.a
    ${EXTERNAL_LIBS_DIR}/libgflags.a
    ${EXTERNAL_LIBS_DIR}/libzmq.a
    ${EXTERNAL_LIBS_DIR}/libcurlpp.a
    curl
    uuid
    ${OPENSSL_LIBRARIES}
    pthread
    sqlite3
    stdc++fs
    ${additional_libs}
)
