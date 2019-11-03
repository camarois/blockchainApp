file(GLOB_RECURSE SOURCE_FILES "*.cpp")
list(FILTER SOURCE_FILES EXCLUDE REGEX ".ccls-cache")

add_executable(miner ${SOURCE_FILES})
add_dependencies(miner common)

target_include_directories(miner PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${COMMON_SRCS_DIR}/include
)

target_link_libraries(miner PRIVATE
    ${COMMON_LIBS_DIR}/libcommon.a
    ${EXTERNAL_LIBS_DIR}/libgflags.a
    ${EXTERNAL_LIBS_DIR}/libzmq.a
    ${EXTERNAL_LIBS_DIR}/libcurlpp.a
    curl
    pthread
    stdc++fs
)
