#ifndef COMMON_GFLAGS_HELPER_HPP
#define COMMON_GFLAGS_HELPER_HPP

#include <cstdio>
#include <cstring>
#include <gflags/gflags.h>

namespace Common {
namespace GflagsHelper {
// NOLINTNEXTLINE(modernize-avoid-c-arrays)
inline void init(const std::string& name, int argc, char* argv[]) {
  for (int i = 0; i < argc; i++) {
    if (strcmp("--help", argv[i]) == 0 || strcmp("-help", argv[i]) == 0) {
      argv[i] = "--helpshort";
    }
  }

  GFLAGS_NAMESPACE::SetUsageMessage(name);
  GFLAGS_NAMESPACE::ParseCommandLineFlags(&argc, &argv, false);
}

}  // namespace GflagsHelper
}  // namespace Common

#endif  // COMMON_GFLAGS_HELPER_HPP
