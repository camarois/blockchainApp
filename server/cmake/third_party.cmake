set(INSTALL_DEFINES -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/externals)
set(BUILD_TYPE_DEFINES -DCMAKE_BUILD_TYPE=Release)
set(COMMON_DEFINES
    ${BUILD_TYPE_DEFINES}
    ${TOOLCHAIN_DEFINES}
    ${INSTALL_DEFINES}
)

include_directories(${CMAKE_BINARY_DIR}/externals/include)
link_directories(${CMAKE_BINARY_DIR}/externals/lib)

set(EXTERNAL_LIBS_DIR ${CMAKE_BINARY_DIR}/externals/lib)

ExternalProject_Add(libcppjwt
    GIT_REPOSITORY https://github.com/arun11299/cpp-jwt.git
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND mkdir -p ${CMAKE_BINARY_DIR}/externals/include
                 && cp -v -R ${CMAKE_BINARY_DIR}/libcppjwt/src/libcppjwt/include/jwt/
                          ${CMAKE_BINARY_DIR}/externals/include
    PREFIX libcppjwt
)
add_dependencies(libcppjwt libgtest)

ExternalProject_Add(libsqlite3
    URL https://www.sqlite.org/2019/sqlite-src-3300100.zip
    URL_MD5 a090a543b1d17178627f280f0fe07324
    CONFIGURE_COMMAND ../libsqlite3/configure
                        --prefix=${CMAKE_BINARY_DIR}/externals
                        --disable-tcl
    BUILD_COMMAND make
    INSTALL_COMMAND make install
    PREFIX libsqlite3
    UPDATE_DISCONNECTED ON
)

ExternalProject_Add(libgflags
    GIT_REPOSITORY https://github.com/gflags/gflags.git
    BUILD_BYPRODUCTS ${EXTERNAL_LIBS_DIR}/libgflags.a
    CMAKE_ARGS -DBUILD_STATIC_LIBS=on
    CMAKE_ARGS ${COMMON_DEFINES}
    PREFIX libgflags
    UPDATE_DISCONNECTED ON
)

ExternalProject_Add(libjson
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    BUILD_BYPRODUCTS ${EXTERNAL_LIBS_DIR}/libjson.a
    CMAKE_ARGS -DBUILD_TESTING=off
    CMAKE_ARGS ${COMMON_DEFINES}
    PREFIX libjson
    UPDATE_DISCONNECTED ON
)

ExternalProject_Add(libgtest
    GIT_REPOSITORY https://github.com/google/googletest.git
    BUILD_BYPRODUCTS ${EXTERNAL_LIBS_DIR}/libgtest.a
    CMAKE_ARGS -Dgmock_build_tests=off
    CMAKE_ARGS -Dgtest_build_tests=off
    CMAKE_ARGS -Dgtest_build_samples=off
    CMAKE_ARGS -Dgtest_disable_pthreads=on
    CMAKE_ARGS ${COMMON_DEFINES}
    PREFIX libgtest
    UPDATE_DISCONNECTED ON
)

find_package(OpenSSL REQUIRED)
ExternalProject_Add(libpistache
    GIT_REPOSITORY https://github.com/oktal/pistache.git
    BUILD_BYPRODUCTS ${EXTERNAL_LIBS_DIR}/libpistache.a
    CMAKE_ARGS -DPISTACHE_INSTALL=on
    CMAKE_ARGS -DPISTACHE_USE_SSL=on
    CMAKE_ARGS ${COMMON_DEFINES}
    PREFIX libpistache
    UPDATE_DISCONNECTED ON
)

ExternalProject_Add(libzmq
    GIT_REPOSITORY https://github.com/zeromq/libzmq.git
    BUILD_BYPRODUCTS ${EXTERNAL_LIBS_DIR}/libzmq.a
    CMAKE_ARGS -DBUILD_TESTS=off
    CMAKE_ARGS -DENABLE_DRAFTS=off
    CMAKE_ARGS -DLIBZMQ_PEDANTIC=off
    CMAKE_ARGS -DWITH_DOCS=off
    CMAKE_ARGS ${COMMON_DEFINES}
    PREFIX libzmq
    UPDATE_DISCONNECTED ON
)

ExternalProject_Add(libcppzmq
    GIT_REPOSITORY https://github.com/zeromq/cppzmq.git
    BUILD_BYPRODUCTS ${EXTERNAL_LIBS_DIR}/libcppzmq.a
    CMAKE_ARGS -DCPPZMQ_BUILD_TESTS=off
    CMAKE_ARGS ${COMMON_DEFINES}
    PREFIX libcppzmq
    UPDATE_DISCONNECTED ON
)
add_dependencies(libcppzmq libzmq)

ExternalProject_Add(libcurlpp
    GIT_REPOSITORY https://github.com/jpbarrette/curlpp.git
    BUILD_BYPRODUCTS ${EXTERNAL_LIBS_DIR}/libcurlpp.a
    CMAKE_ARGS ${COMMON_DEFINES}
    PREFIX libcurlpp
    UPDATE_DISCONNECTED ON
)

ExternalProject_Add(libpicosha2
    GIT_REPOSITORY https://github.com/okdshin/picosha2.git
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND mkdir -p ${CMAKE_BINARY_DIR}/externals/include
                 && cp -v ${CMAKE_BINARY_DIR}/libpicosha2/src/libpicosha2/picosha2.h
                          ${CMAKE_BINARY_DIR}/externals/include
    PREFIX libpicosha2
)
