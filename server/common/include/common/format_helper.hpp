#ifndef COMMON_FORMAT_HELPER_HPP
#define COMMON_FORMAT_HELPER_HPP

#include <algorithm>
#include <chrono>
#include <string>

namespace Common {
namespace FormatHelper {
inline std::string nowStr() {
  auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
  std::string nowStr = std::ctime(&now);
  return nowStr.substr(0, nowStr.length() - 1);
}

inline std::string toLower(std::string str) {
  std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c) { return std::tolower(c); });
  return str;
}

}  // namespace FormatHelper
}  // namespace Common

#endif  // COMMON_FORMAT_HELPER_HPP
