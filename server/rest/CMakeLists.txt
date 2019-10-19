file(GLOB_RECURSE SOURCE_FILES "*.cpp")

add_executable(rest ${SOURCE_FILES})
add_dependencies(rest common libpistache)

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
)
