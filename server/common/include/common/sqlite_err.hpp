#ifndef COMMON_SQLITE_ERR_HPP
#define COMMON_SQLITE_ERR_HPP

#include <sqlite3.h>
#include <stdexcept>

namespace Common {

class SqliteErr : public std::runtime_error {
 public:
  explicit SqliteErr(const std::string& msg = "Default");

  std::string message();

 private:
  std::string msg_;
};

}  // namespace Common

#endif  // COMMON_SQLITE_ERR_HPP
