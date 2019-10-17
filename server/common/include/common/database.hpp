
#ifndef COMMON_DATABASE_HPP
#define COMMON_DATABASE_HPP

#include "common/models.hpp"
#include "common/query.hpp"
#include "common/statement.hpp"
#include "sqlite_err.hpp"
#include <cstddef>
#include <filesystem>
#include <memory>
#include <sqlite3.h>
#include <string>

namespace Common {

struct Sqlite3Deleter {
  void operator()(sqlite3* db) const { sqlite3_close(db); }
};

using sqlite3_ptr = std::unique_ptr<sqlite3, Sqlite3Deleter>;

class Database {
 public:
  Database();
  explicit Database(const std::filesystem::path& filePath);

  static void assertSqlite(int errCode, const std::string& message = "");

  Common::Models::LoginRequest getUser(const std::string& username) const;
  void createUser(const Common::Models::LoginRequest* user);

 private:
  void close();
  Common::Models::LoginRequest getUserFromStatement(const Statement& statement) const;

  const std::string kDatabaseName_ = "../blockchain.db";
  sqlite3_ptr db_;
};

}  // namespace Common

#endif  // COMMON_DATABASE_HPP
