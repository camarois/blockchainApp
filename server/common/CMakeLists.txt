file (GLOB COMMON_SOURCE_FILES "src/*.cc")
add_library(common ${COMMON_SOURCE_FILES})
target_include_directories(common PUBLIC include)
