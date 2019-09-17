file (GLOB_RECURSE COMMON_SOURCE_FILES "*.cc")
add_library(common ${COMMON_SOURCE_FILES})
target_include_directories(common PUBLIC include)
