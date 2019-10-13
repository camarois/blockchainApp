#ifndef COMMON_SQLITE_ERR_HPP
#define COMMON_SQLITE_ERR_HPP

#include <sqlite3.h>
#include <stdexcept>

namespace Common {

class SqliteErr : public std::runtime_error {
 public:
  explicit SqliteErr(int code = SQLITE_OK);
  SqliteErr(int code, const std::string& msg);

  int code();

 private:
  int code_;
};

}  // namespace Common

#endif  // COMMON_SQLITE_ERR_HPP
