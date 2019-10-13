#include "common/database.hpp"
#include <sstream>

namespace Common {

Database::Database() {
  assertSqlite(sqlite3_initialize(), "Unable to initialize SQLite");
  assertSqlite(sqlite3_enable_shared_cache(1), "Cannot enable db shared cache mode");
  try {
    assertSqlite(sqlite3_open_v2(kDatabaseName_.c_str(), &db_,
				 static_cast<unsigned>(SQLITE_OPEN_READWRITE | SQLITE_OPEN_SHAREDCACHE),
				 nullptr),
		 "Cannot connect to database");
  } catch (...) {
    sqlite3_close_v2(db_);
    throw;
  }
}

void Database::close() {
  sqlite3_close_v2(db_);
  sqlite3_shutdown();
}

void Database::assertSqlite(int errCode, const std::string& message) {
  if (errCode != SQLITE_DONE && errCode != SQLITE_OK && errCode != SQLITE_ROW) {
    if (message.length() > 0) {
      std::ostringstream err;
      err << message << "; Sqlite error message: " << sqlite3_errstr(errCode);
      throw SqliteErr(errCode, err.str());
    }
    throw SqliteErr(errCode);
  }
}

// User_t Database::getUserById(uint32_t id) const {
//     return getUserByQuery_(Query(
//         "SELECT user_id, ip, name, mac, is_blacklisted FROM user WHERE (user_id = %u);",
//         id));
// }

// void Database::createUser(const User_t* user) {
//     executeQuery_(Query(
//         "INSERT OR REPLACE INTO user (user_id, ip, mac, name) "
//         "VALUES (%u, '%q', '%q', '%q');",
//         user->userId,
//         user->ip,
//         user->mac,
//         user->name));
// }

}  // namespace Common
