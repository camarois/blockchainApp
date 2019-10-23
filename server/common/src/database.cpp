#include "common/database.hpp"

#include <gflags/gflags.h>

namespace Common {

Database::Database(const std::string& dbPath) {
  assertSqlite(sqlite3_initialize(), "Unable to initialize SQLite");
  assertSqlite(sqlite3_enable_shared_cache(1), "Cannot enable db shared cache mode");
  try {
    sqlite3* dbPtr;
    assertSqlite(
        sqlite3_open_v2(dbPath.c_str(), &dbPtr,
                        static_cast<unsigned>(SQLITE_OPEN_READWRITE) | static_cast<unsigned>(SQLITE_OPEN_SHAREDCACHE),
                        nullptr),
        "Cannot connect to database");
    db_.reset(dbPtr, sqlite3_close_v2);
  } catch (...) {
    close();
    throw;
  }
}

void Database::close() {
  sqlite3_close_v2(db_.get());
  sqlite3_shutdown();
}

void Database::assertSqlite(int errCode, const std::string& message) {
  if (errCode != SQLITE_DONE && errCode != SQLITE_OK && errCode != SQLITE_ROW) {
    throw SqliteErr(message + "; Sqlite error message: " + sqlite3_errstr(errCode));
  }
}

// TODO(frank): change all return types to auto!!
std::optional<Common::Models::LoginRequest> Database::getUser(const std::string& username) {
  Query query = Query("SELECT username, password FROM users WHERE (username = '%q');", username.c_str());
  Statement statement(db_, query);
  if (statement.step()) {
    return Common::Models::LoginRequest{statement.getColumnText(0), statement.getColumnText(1)};
  }
  return {};
}

bool Database::createUser(const Common::Models::LoginRequest& user) {
  Query query = Query(
      "INSERT OR REPLACE INTO users (username, password) "
      "VALUES ('%q', '%q');",
      (user.username).c_str(), (user.password).c_str());
  Statement statement(db_, query);
  return statement.step();
}

}  // namespace Common
