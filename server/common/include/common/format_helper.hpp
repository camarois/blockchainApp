#ifndef COMMON_FORMAT_HELPER_HPP
#define COMMON_FORMAT_HELPER_HPP

#include <algorithm>
#include <chrono>
#include <iostream>
#include <picosha2.h>
#include <random>
#include <string>

namespace Common {
namespace FormatHelper {
const size_t kDefaultRandomSize = 32;
const int kAsciiNumber = 128;
static std::mt19937 rng; // NOLINT

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

inline std::string randomStr(size_t size = kDefaultRandomSize) {
  std::uniform_int_distribution<std::mt19937::result_type> dist(0, kAsciiNumber);

  std::string result;
  for (size_t i = 0; i < size; i++) {
    auto randomChar = static_cast<unsigned char>(dist(rng));
    if (std::isalnum(randomChar) != 0) {
      result += randomChar;  // Creates a random alpha num value
    }
    else {
      --i;
    }
  }
  return result;
}

}  // namespace FormatHelper
}  // namespace Common

#endif  // COMMON_FORMAT_HELPER_HPP
