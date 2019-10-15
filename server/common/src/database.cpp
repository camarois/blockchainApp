#include "common/database.hpp"
#include <sstream>

namespace Common {

Database::Database() {
  assertSqlite(sqlite3_initialize(), "Unable to initialize SQLite");
  assertSqlite(sqlite3_enable_shared_cache(1), "Cannot enable db shared cache mode");
  try {
    assertSqlite(sqlite3_open_v2(kDatabaseName_.c_str(), (sqlite3**)&db_,
				 static_cast<unsigned>(SQLITE_OPEN_READWRITE | SQLITE_OPEN_SHAREDCACHE),
				 nullptr),
		 "Cannot connect to database");
  } catch (...) {
    close();
    throw;
  }
}

void Database::close() {
  sqlite3_close_v2((sqlite3*)&db_);
  sqlite3_shutdown();
}

void Database::assertSqlite(int errCode, const std::string& message) {
  if (errCode != SQLITE_DONE && errCode != SQLITE_OK && errCode != SQLITE_ROW) {
    throw SqliteErr(message + "; Sqlite error message: " + sqlite3_errstr(errCode));
  }
}

// Common::Models::LoginRequest Database::getUser(std::string username) const {
//     return getUserByQuery_(Query(
//         "SELECT username, password FROM users WHERE (username = %q);",
//         username.c_str()));
// }

// void Database::createUser(const Common::Models::LoginRequest user) {
//     executeQuery_(Query(
//         "INSERT OR REPLACE INTO users (username, password) "
//         "VALUES ('%q', '%q');",
//         user.username,
//         user.password));
// }

}  // namespace Common
