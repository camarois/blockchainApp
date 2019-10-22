#ifndef COMMON_QUERY_HPP
#define COMMON_QUERY_HPP

#include <climits>
#include <cstdint>
#include <iostream>
#include <memory>
#include <sqlite3.h>
#include <string>
#include <type_traits>
#include <utility>

namespace Common {

class Query {
 public:
  template <typename... Args>
  explicit Query(const std::string& zFormat, Args... args) {
    query_ = sqlite3_mprintf(zFormat.c_str(), args...); // NOLINT
  }

  std::string val() const;

 private:
  std::string query_;
};

}  // namespace Common

#endif  // COMMON_QUERY_HPP