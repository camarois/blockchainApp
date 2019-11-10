function(download_and_unpack url md5 file directory)
    if(EXISTS ${directory}/${file})
        file(MD5 ${directory}/${file} file_md5)
        if(${md5} EQUAL ${file_md5})
            message(STATUS "Using already downloaded ${file}")
        else()
            message(STATUS "Redownloading ${file}")
            file(DOWNLOAD ${url} ${directory}/${file} EXPECTED_HASH MD5=${md5} TLS_VERIFY ON)
        endif()
    else()
        message(STATUS "Downloading ${file}")
        file(DOWNLOAD ${url} ${directory}/${file} EXPECTED_HASH MD5=${md5} TLS_VERIFY ON)
    endif()

    message(STATUS "Unpacking ${file}")
    execute_process(
        COMMAND tar --skip-old-files --no-same-owner -xvf ${directory}/${file}
        WORKING_DIRECTORY ${directory}
        OUTPUT_QUIET
        ERROR_QUIET
    )
endfunction()

set(SYSROOT_DIR ${CMAKE_BINARY_DIR}/sysroot)
set(sysroot_file ArchLinuxARM-zedboard-latest.tar.gz)
set(sysroot_md5 87e6df9003b5fb4394503b36b46f2584)
set(sysroot_url http://mirror.archlinuxarm.org/os/${sysroot_file})
download_and_unpack(
    ${sysroot_url}
    ${sysroot_md5}
    ${sysroot_file}
    ${SYSROOT_DIR}
)

set(curl_file curl-7.66.0-1-armv7h.pkg.tar.xz)
set(curl_md5 a1fe169706420d9c110a55b905bdd4c3)
set(curl_url http://tardis.tiny-vps.com/aarm/packages/c/curl/${curl_file})
download_and_unpack(
    ${curl_url}
    ${curl_md5}
    ${curl_file}
    ${SYSROOT_DIR}
)

set(sqlite_file sqlite-3.30.1-1-armv7h.pkg.tar.xz)
set(sqlite_md5 6e4f940daae448eec9b31ae62d4829ef)
set(sqlite_url http://tardis.tiny-vps.com/aarm/packages/s/sqlite/${sqlite_file})
download_and_unpack(
    ${sqlite_url}
    ${sqlite_md5}
    ${sqlite_file}
    ${SYSROOT_DIR}
)

set(TOOLCHAIN_DIR ${CMAKE_BINARY_DIR}/toolchain)
set(toolchain_file x-tools7h.tar.xz)
set(toolchain_md5 856aa3e81afda734deff8fb2848d5760)
set(toolchain_url https://archlinuxarm.org/builder/xtools/${toolchain_file})
download_and_unpack(
    ${toolchain_url}
    ${toolchain_md5}
    ${toolchain_file}
    ${TOOLCHAIN_DIR}
)

message(STATUS "Setting ARMv7 toolchain")
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/cmake/armv7_toolchain.cmake)
set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")
