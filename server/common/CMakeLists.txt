file(GLOB_RECURSE COMMON_SOURCE_FILES "*.cpp")

add_library(common STATIC ${COMMON_SOURCE_FILES})
add_dependencies(common libcppzmq libzmq libjson libgflags libcurlpp)

target_include_directories(common PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${SQLite3_INCLUDE_DIRS}
)

target_link_libraries(common PRIVATE
    ${EXTERNAL_LIBS_DIR}/libzmq.a
    ${EXTERNAL_LIBS_DIR}/libgflags.a
    ${SQLite3_LIBRARIES}
)
