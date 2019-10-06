# set(GENERATOR_DEFINES -GNinja)
set(INSTALL_DEFINES -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/externals)
set(BUILD_TYPE_DEFINES -DCMAKE_BUILD_TYPE=Release)
set(COMMON_DEFINES
    # ${GENERATOR_DEFINES}
    ${BUILD_TYPE_DEFINES}
    ${TOOLCHAIN_DEFINES}
    ${INSTALL_DEFINES}
)

include_directories(${CMAKE_BINARY_DIR}/externals/include)
link_directories(${CMAKE_BINARY_DIR}/externals/lib)

set(EXTERNAL_LIBS_DIR ${CMAKE_BINARY_DIR}/externals/lib)

ExternalProject_Add(libgflags
    GIT_REPOSITORY https://github.com/gflags/gflags.git
    BUILD_BYPRODUCTS ${EXTERNAL_LIBS_DIR}/libgflags.a
    CMAKE_ARGS -DBUILD_STATIC_LIBS=on
    CMAKE_ARGS ${COMMON_DEFINES}
    PREFIX libgflags
)

ExternalProject_Add(libjson
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    BUILD_BYPRODUCTS ${EXTERNAL_LIBS_DIR}/libjson.a
    CMAKE_ARGS -DBUILD_TESTING=off
    CMAKE_ARGS ${COMMON_DEFINES}
    PREFIX libjson
)

ExternalProject_Add(libgtest
    GIT_REPOSITORY https://github.com/google/googletest.git
    CMAKE_ARGS -Dgmock_build_tests=off
    CMAKE_ARGS -Dgtest_build_tests=off
    CMAKE_ARGS -Dgtest_build_samples=off
    BUILD_BYPRODUCTS ${EXTERNAL_LIBS_DIR}/libgtest.a
    CMAKE_ARGS -Dgtest_disable_pthreads=off
    CMAKE_ARGS ${COMMON_DEFINES}
    PREFIX libgtest
)

ExternalProject_Add(libpistache
    GIT_REPOSITORY https://github.com/oktal/pistache.git
    CMAKE_ARGS -DPISTACHE_INSTALL=on
    BUILD_BYPRODUCTS ${EXTERNAL_LIBS_DIR}/libpistache.a
    CMAKE_ARGS -DPISTACHE_USE_SSL=off
    CMAKE_ARGS ${COMMON_DEFINES}
    PREFIX libpistache
)

ExternalProject_Add(libzmq
    GIT_REPOSITORY https://github.com/zeromq/libzmq.git
    CMAKE_ARGS -DBUILD_TESTS=off
    CMAKE_ARGS -DENABLE_DRAFTS=off
    CMAKE_ARGS -DLIBZMQ_PEDANTIC=off
    BUILD_BYPRODUCTS ${EXTERNAL_LIBS_DIR}/libzmq.a
    CMAKE_ARGS -DWITH_DOCS=off
    CMAKE_ARGS ${COMMON_DEFINES}
    PREFIX libzmq
)

ExternalProject_Add(libcppzmq
    GIT_REPOSITORY https://github.com/zeromq/cppzmq.git
    BUILD_BYPRODUCTS ${EXTERNAL_LIBS_DIR}/libcppzmq.a
    CMAKE_ARGS -DCPPZMQ_BUILD_TESTS=off
    CMAKE_ARGS ${COMMON_DEFINES}
    PREFIX libcppzmq
)
add_dependencies(libcppzmq libzmq)

ExternalProject_Add(libcurlpp
BUILD_BYPRODUCTS ${EXTERNAL_LIBS_DIR}/libcurlpp.a
    GIT_REPOSITORY https://github.com/jpbarrette/curlpp.git
    CMAKE_ARGS ${COMMON_DEFINES}
    PREFIX libcurlpp
)
