file (GLOB_RECURSE COMMON_SOURCE_FILES "*.cc")
add_library(common ${COMMON_SOURCE_FILES})
target_link_libraries(common PUBLIC nlohmann_json)
target_include_directories(common PUBLIC include)
