file(GLOB_RECURSE COMMON_SOURCE_FILES "*.cpp")
list(FILTER COMMON_SOURCE_FILES EXCLUDE REGEX ".ccls-cache")

add_library(common STATIC ${COMMON_SOURCE_FILES})
add_dependencies(common libcppzmq libzmq libjson libgflags libcurlpp libsqlite3)

target_include_directories(common PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(common PRIVATE
    ${EXTERNAL_LIBS_DIR}/libzmq.a
    ${EXTERNAL_LIBS_DIR}/libgflags.a
)
