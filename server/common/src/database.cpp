#include "common/database.hpp"
#include <iostream>
#include <sstream>

namespace Common {

Database::Database() {
  assertSqlite(sqlite3_initialize(), "Unable to initialize SQLite");
  assertSqlite(sqlite3_enable_shared_cache(1), "Cannot enable db shared cache mode");
  try {
    assertSqlite(
        sqlite3_open_v2(kDatabaseName_.c_str(), reinterpret_cast<sqlite3**>(&db_),
                        static_cast<unsigned>(SQLITE_OPEN_READWRITE) | static_cast<unsigned>(SQLITE_OPEN_SHAREDCACHE),
                        nullptr),
        "Cannot connect to database");
  } catch (...) {
    close();
    throw;
  }
}

Database::Database(const std::filesystem::path& filePath) {
  assertSqlite(sqlite3_initialize(), "Unable to initialize SQLite");
  assertSqlite(sqlite3_enable_shared_cache(1), "Cannot enable db shared cache mode");
  try {
    assertSqlite(
        sqlite3_open_v2(filePath.c_str(), reinterpret_cast<sqlite3**>(&db_),
                        static_cast<unsigned>(SQLITE_OPEN_READWRITE) | static_cast<unsigned>(SQLITE_OPEN_SHAREDCACHE),
                        nullptr),
        "Cannot connect to database");
  } catch (...) {
    close();
    throw;
  }
}

void Database::close() {
  sqlite3_close_v2(&(*db_));
  sqlite3_shutdown();
}

void Database::assertSqlite(int errCode, const std::string& message) {
  if (errCode != SQLITE_DONE && errCode != SQLITE_OK && errCode != SQLITE_ROW) {
    throw SqliteErr(message + "; Sqlite error message: " + sqlite3_errstr(errCode));
  }
}

Common::Models::LoginRequest Database::getUserFromStatement(const Statement& state) const {
  Common::Models::LoginRequest user = {state.getColumnText(0), state.getColumnText(1)};
  return user;
}

Common::Models::LoginRequest Database::getUser(const std::string& username) {
  Common::Models::LoginRequest user = {};
  Query query = Query("SELECT username, password FROM users WHERE (username = '%q');", username.c_str());
  Statement state = Statement(&(*db_), query);

  return state.step() ? getUserFromStatement(state) : user;
}

void Database::createUser(const Common::Models::LoginRequest* user) {
  Query query = Query(
      "INSERT INTO users (username, password) "
      "VALUES ('%q', '%q');",
      (user->username).c_str(), (user->password).c_str());
  Statement state = Statement(&(*db_), query);
  state.step();
}

}  // namespace Common
