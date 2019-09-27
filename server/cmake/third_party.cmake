ExternalProject_Add(libgflags
    GIT_REPOSITORY https://github.com/gflags/gflags.git
    CMAKE_ARGS -DBUILD_STATIC_LIBS=on
    CMAKE_ARGS ${TOOLCHAIN_DEFINES}
    INSTALL_COMMAND ""
    PREFIX libgflags
)
include_directories(${CMAKE_BINARY_DIR}/libgflags/src/libgflags-build/include)
link_directories(${CMAKE_BINARY_DIR}/libgflags/src/libgflags-build/lib)

ExternalProject_Add(libjson
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    CMAKE_ARGS -DBUILD_TESTING=off
    CMAKE_ARGS ${TOOLCHAIN_DEFINES}
    INSTALL_COMMAND ""
    PREFIX libjson
)
include_directories(${CMAKE_BINARY_DIR}/libjson/src/libjson/include)

ExternalProject_Add(libzmq
    GIT_REPOSITORY https://github.com/zeromq/libzmq.git
    CMAKE_ARGS -DBUILD_TESTS=off
    CMAKE_ARGS -DENABLE_DRAFTS=off
    CMAKE_ARGS -DLIBZMQ_PEDANTIC=off
    CMAKE_ARGS -DWITH_DOCS=off
    CMAKE_ARGS -DBUILD_SHARED=off
    CMAKE_ARGS ${TOOLCHAIN_DEFINES}
    INSTALL_COMMAND ""
    PREFIX libzmq
)
include_directories(${CMAKE_BINARY_DIR}/libzmq/src/libzmq/include)
link_directories(${CMAKE_BINARY_DIR}/libzmq/src/libzmq-build/lib)

ExternalProject_Add(libgtest
    GIT_REPOSITORY https://github.com/google/googletest.git
    CMAKE_ARGS -Dgmock_build_tests=off
    CMAKE_ARGS -Dgtest_build_tests=off
    CMAKE_ARGS -Dgtest_build_samples=off
    CMAKE_ARGS -Dgtest_disable_pthreads=off
    CMAKE_ARGS ${TOOLCHAIN_DEFINES}
    INSTALL_COMMAND ""
    PREFIX libgtest
)
include_directories(${CMAKE_BINARY_DIR}/libgtest/src/libgtest/googlemock/include)
include_directories(${CMAKE_BINARY_DIR}/libgtest/src/libgtest/googletest/include)
link_directories(${CMAKE_BINARY_DIR}/libgtest/src/libgtest-build/lib)

ExternalProject_Add(libpistache
    GIT_REPOSITORY https://github.com/oktal/pistache.git
    CMAKE_ARGS -DPISTACHE_INSTALL=off
    CMAKE_ARGS -DPISTACHE_USE_SSL=off
    CMAKE_ARGS ${TOOLCHAIN_DEFINES}
    INSTALL_COMMAND ""
    PREFIX libpistache
)
include_directories(${CMAKE_BINARY_DIR}/libpistache/src/libpistache/include)
link_directories(${CMAKE_BINARY_DIR}/libpistache/src/libpistache-build/src)

ExternalProject_Add(libcppzmq
    GIT_REPOSITORY https://github.com/zeromq/cppzmq.git
    CMAKE_ARGS -DCPPZMQ_BUILD_TESTS=off
    CMAKE_ARGS ${TOOLCHAIN_DEFINES}
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
    PREFIX libcppzmq
)
include_directories(${CMAKE_BINARY_DIR}/libcppzmq/src/libcppzmq)
