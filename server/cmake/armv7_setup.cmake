set(toolchain_file x-tools7h.tar.xz)
set(toolchain_url https://archlinuxarm.org/builder/xtools/${toolchain_file})

message(STATUS "Downloading ARMv7 toolchain")
file(
    DOWNLOAD ${toolchain_url} ${CMAKE_BINARY_DIR}/third_party/armv7_toolchain/${toolchain_file}
    EXPECTED_HASH MD5=856aa3e81afda734deff8fb2848d5760
    TLS_VERIFY ON
)

message(STATUS "Unpacking ARMv7 toolchain")
execute_process(
    COMMAND tar --skip-old-files -xvf ${toolchain_file}
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/third_party/armv7_toolchain
    OUTPUT_QUIET
    ERROR_QUIET
)

message(STATUS "Setting ARMv7 toolchain")
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/cmake/armv7_toolchain.cmake)
set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")
