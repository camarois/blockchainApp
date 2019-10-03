file (GLOB_RECURSE COMMON_SOURCE_FILES "*.cpp")
add_library(common ${COMMON_SOURCE_FILES})
target_include_directories(common PUBLIC include
    ${CMAKE_SOURCE_DIR}/third_party/libzmp/include ${CMAKE_SOURCE_DIR}/third_party/cppzmq
    ${CMAKE_SOURCE_DIR}/third_party/curlpp/include)
target_link_libraries(common PUBLIC nlohmann_json libzmq-static curlpp)
