set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR arm)
set(CMAKE_SYSROOT ${SYSROOT_DIR})

set(toolchain ${TOOLCHAIN_DIR}/x-tools7h/arm-unknown-linux-gnueabihf)
set(ENV{CC}  ${toolchain}/bin/arm-unknown-linux-gnueabihf-gcc)
set(ENV{CPP} ${toolchain}/bin/arm-unknown-linux-gnueabihf-g++)
set(ENV{CFLAGS}   "-I${SYSROOT_DIR}/usr/include -L${SYSROOT_DIR}/usr/lib --sysroot=${SYSROOT_DIR}")
set(ENV{CPPFLAGS} "-I${SYSROOT_DIR}/usr/include -L${SYSROOT_DIR}/usr/lib --sysroot=${SYSROOT_DIR} -static-libstdc++")

set(CMAKE_C_COMPILER   ${toolchain}/bin/arm-unknown-linux-gnueabihf-gcc)
set(CMAKE_CXX_COMPILER ${toolchain}/bin/arm-unknown-linux-gnueabihf-g++)
set(CMAKE_TOOLCHAIN_DEFINES
    -DTOOLCHAIN_DIR=${TOOLCHAIN_DIR}
    -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}
    -DCMAKE_C_FLAGS=$ENV{CFLAGS}
    -DCMAKE_CXX_FLAGS=$ENV{CPPFLAGS}
)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
