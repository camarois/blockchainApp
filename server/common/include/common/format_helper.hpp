#ifndef COMMON_FORMAT_HELPER_HPP
#define COMMON_FORMAT_HELPER_HPP

#include <algorithm>
#include <chrono>
#include <picosha2.h>
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

inline std::string hash(const std::string& password) {
  std::vector<unsigned char> hash(picosha2::k_digest_size);
  picosha2::hash256(password.begin(), password.end(), hash.begin(), hash.end());
  return picosha2::bytes_to_hex_string(hash.begin(), hash.end());
}

inline std::string randomStr(size_t size = 32) {
  const int kAsciiNumber = 128;
  std::string result = "";
  for (size_t i = 0; i < size; i++) {
    result += static_cast<char>(std::rand() % kAsciiNumber); // Creates a random ASCII value
  }
  return result;
}

}  // namespace FormatHelper
}  // namespace Common

#endif  // COMMON_FORMAT_HELPER_HPP
