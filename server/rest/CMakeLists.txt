file(GLOB_RECURSE SOURCE_FILES "*.cpp")
list(FILTER SOURCE_FILES EXCLUDE REGEX ".ccls-cache")

add_custom_target(deps_file
    # This will create db if it doesn't exist
    COMMAND printf "C" | ${SQL_DIR}/createDb.sh blockchain.db
    BYPRODUCTS blockchain.db

    COMMAND cp
        ${CERT_DIR}/createCert.sh
        ${CERT_DIR}/rootCA.crt
        ${CERT_DIR}/rootCA.key
        ${CERT_DIR}/rootCA.srl
        ${CMAKE_CURRENT_BINARY_DIR}
)

add_executable(rest ${SOURCE_FILES})
add_dependencies(rest common libpistache deps_file)

target_include_directories(rest PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${COMMON_SRCS_DIR}/include
)

target_link_libraries(rest PRIVATE
    ${COMMON_LIBS_DIR}/libcommon.a
    ${EXTERNAL_LIBS_DIR}/libpistache.a
    ${EXTERNAL_LIBS_DIR}/libgflags.a
    ${EXTERNAL_LIBS_DIR}/libzmq.a
    ${EXTERNAL_LIBS_DIR}/libcurlpp.a
    curl
    ${OPENSSL_LIBRARIES}
    pthread
    sqlite3
)
