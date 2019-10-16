set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR arm)

set(toolchain ${TOOLCHAIN_DIR}/armv7_toolchain/x-tools7h/arm-unknown-linux-gnueabihf)
set(CMAKE_C_COMPILER   ${toolchain}/bin/arm-unknown-linux-gnueabihf-gcc)
set(CMAKE_CXX_COMPILER ${toolchain}/bin/arm-unknown-linux-gnueabihf-g++)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
