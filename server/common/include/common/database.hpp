
#ifndef COMMON_DATABASE_HPP
#define COMMON_DATABASE_HPP

#include "sqlite_err.hpp"
#include "common/models.hpp"
#include <cstddef>
#include <memory>
#include <sqlite3.h>
#include <string>

namespace Common {

struct Sqlite3Deleter {
  void operator () (sqlite3* db) const { sqlite3_close(db); }
};

using sqlite3_ptr = std::unique_ptr<sqlite3, Sqlite3Deleter>;

class Database {
 public:
  explicit Database();

 private:
  void close();
  // void createUser(const Common::Models::LoginRequest user);
  // Common::Models::LoginRequest getUser(std::string username) const;
  static void assertSqlite(int errCode, const std::string& message = "");

  sqlite3_ptr db_ = nullptr;
  const std::string kDatabaseName_ = "blockchain.db";
};

}  // namespace Common

#endif  // COMMON_DATABASE_HPP
