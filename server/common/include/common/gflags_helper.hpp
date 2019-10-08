#ifndef COMMON_GFLAGS_HELPER_HPP
#define COMMON_GFLAGS_HELPER_HPP

#include <string>
#include <gflags/gflags.h>

namespace Common {
namespace GflagsHelper {
inline void init(std::string name, int argc, char* argv[]) {
  for (int i = 0; i < argc; i++) {
    if (strcmp("--help", argv[i]) == 0) {
      argv[i] = "--helpshort";
    }
  }

  GFLAGS_NAMESPACE::SetUsageMessage("Rest service");
  GFLAGS_NAMESPACE::ParseCommandLineFlags(&argc, &argv, false);
}

}  // namespace GflagsHelper
}  // namespace Common

#endif  // COMMON_GFLAGS_HELPER_HPP
