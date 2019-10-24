#ifndef COMMON_OPTIONAL_HPP
#define COMMON_OPTIONAL_HPP

#include <functional>
#include <optional>

// NOLINTNEXTLINE(readability-identifier-naming, cert-dcl58-cpp)
namespace std {

// TODO(gabriel): We should probably subclass std::optional<T> and overload at
//                least the deferencing operator rather than alias it.
//
//                This is because deferencing std::optional_ref<T> will return
//                an std::reference_wrapper<T> and not T. Thus, when using
//                std::optional_ref<T>, we need to replace
//
//                    opt->method()
//
//                by
//
//                    opt->get().method()

template<class T>
using optional_ref = std::optional<std::reference_wrapper<T>>;

} // namespace std

#endif  // COMMON_OPTIONAL_HPP
