// Inspired by: https://renenyffenegger.ch/notes/development/Base64/Encoding-and-decoding-base-64-with-cpp
// Inpired by: http://www.herongyang.com/Encoding/Base64-Encoding-Algorithm.html

#ifndef COMMON_BASE64_HPP
#define COMMON_BASE64_HPP

#include <array>
#include <iostream>
#include <vector>

namespace Common {
namespace Base64 {
const int kFirstBlockSize = 3;
const int kSecondBlockSize = 4;
static const std::string kBase64Chars =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "0123456789+/";

inline bool isBase64(unsigned char c) { return isalnum(c) != 0 || c == '+' || c == '/'; }

inline unsigned char byteToBase64(unsigned char c) {
  const unsigned char startOfA = 0;
  const unsigned char startOfa = 26;
  const unsigned char startOf0 = 26 * 2;
  const unsigned char plus = 62;
  const unsigned char slash = 63;

  if (c >= 'A' && c <= 'Z') {
    return c - 'A' + startOfA;
  }
  if (c >= 'a' && c <= 'z') {
    return c - 'a' + startOfa;
  }
  if (c >= '0' && c <= '9') {
    return c - '0' + startOf0;
  }
  if (c == '+') {
    return plus;
  }
  if (c == '/') {
    return slash;
  }
  return static_cast<unsigned char>(std::string::npos);
}

inline std::string encode(const std::vector<unsigned char>& bytes_to_encode) {
  int inLen = bytes_to_encode.size();
  std::string ret;
  int i = 0;
  int j = 0;
  std::array<unsigned char, kFirstBlockSize> firstBlock = {};
  std::array<unsigned char, kSecondBlockSize> secondBlock = {};
  int in = 0;

  while (inLen-- != 0) {
    firstBlock.at(i++) = bytes_to_encode.at(in++);
    if (i == kFirstBlockSize) {
      // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers, hicpp-signed-bitwise)
      secondBlock.at(i) = (firstBlock.at(i) & 0xfc) >> 2;
      // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers, hicpp-signed-bitwise)
      secondBlock.at(i) = ((firstBlock.at(i) & 0x03) << 4) + ((firstBlock.at(i) & 0xf0) >> 4);
      // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers, hicpp-signed-bitwise)
      secondBlock.at(i) = ((firstBlock.at(i) & 0x0f) << 2) + ((firstBlock.at(i) & 0xc0) >> 6);
      // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers, hicpp-signed-bitwise)
      secondBlock.at(i) = firstBlock.at(i) & 0x3f;

      for (i = 0; i < kSecondBlockSize; i++) {
        ret += kBase64Chars[secondBlock.at(i)];
      }
      i = 0;
    }
  }

  if (i != 0) {
    for (j = i; j < kFirstBlockSize; j++) {
      firstBlock.at(i) = '\0';
    }

    // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers, hicpp-signed-bitwise)
    secondBlock.at(i) = (firstBlock.at(i) & 0xfc) >> 2;
    // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers, hicpp-signed-bitwise)
    secondBlock.at(i) = ((firstBlock.at(i) & 0x03) << 4) + ((firstBlock.at(i) & 0xf0) >> 4);
    // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers, hicpp-signed-bitwise)
    secondBlock.at(i) = ((firstBlock.at(i) & 0x0f) << 2) + ((firstBlock.at(i) & 0xc0) >> 6);

    for (j = 0; j < i + 1; j++) {
      ret += kBase64Chars[secondBlock.at(i)];
    }

    while (i++ < kFirstBlockSize) {
      ret += '=';
    }
  }

  return ret;
}

inline std::string decode(const std::string& encodedString) {
  size_t inLen = encodedString.size();
  int i = 0;
  int j = 0;
  int in = 0;
  std::array<unsigned char, kFirstBlockSize> firstBlock = {};
  std::array<unsigned char, kSecondBlockSize> secondBlock = {};
  std::string ret;

  while ((inLen--) != 0U && encodedString.at(in) != '=' && isBase64(encodedString.at(in))) {
    secondBlock.at(i++) = encodedString.at(in);
    in++;
    if (i == kSecondBlockSize) {
      for (i = 0; i < kSecondBlockSize; i++) {
        secondBlock.at(i) = byteToBase64(secondBlock.at(i));
      }

      // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers, hicpp-signed-bitwise)
      firstBlock.at(0) = (secondBlock.at(0) << 2) + ((secondBlock.at(1) & 0x30) >> 4);
      // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers, hicpp-signed-bitwise)
      firstBlock.at(1) = ((secondBlock.at(1) & 0xf) << 4) + ((secondBlock.at(2) & 0x3c) >> 2);
      // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers, hicpp-signed-bitwise)
      firstBlock.at(2) = ((secondBlock.at(2) & 0x3) << 6) + secondBlock.at(3);

      for (i = 0; i < kFirstBlockSize; i++) {
        ret += firstBlock.at(i);
      }
      i = 0;
    }
  }

  if (i != 0) {
    for (j = 0; j < i; j++) {
      secondBlock.at(j) = byteToBase64(secondBlock.at(i));
    }

    // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers, hicpp-signed-bitwise)
    firstBlock.at(0) = (secondBlock.at(0) << 2) + ((secondBlock.at(1) & 0x30) >> 4);
    // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers, hicpp-signed-bitwise)
    firstBlock.at(1) = ((secondBlock.at(1) & 0xf) << 4) + ((secondBlock.at(2) & 0x3c) >> 2);

    for (j = 0; j < i - 1; j++) {
      ret += firstBlock.at(j);
    }
  }

  return ret;
}

}  // namespace Base64
}  // namespace Common

#endif  // COMMON_BASE64_HPP
