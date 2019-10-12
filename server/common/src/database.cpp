#include "common/database.hpp"
#include <sstream>

namespace Common {

Database::Database() {
  assertSqlite(sqlite3_initialize(), "Unable to initialize SQLite");
  assertSqlite(sqlite3_enable_shared_cache(1), "Cannot enable db shared cache mode");
  try {
    assertSqlite(sqlite3_open_v2(kDatabaseName_.c_str(), &db_,
				 static_cast<unsigned>(SQLITE_OPEN_READWRITE) |
				     static_cast<unsigned>(SQLITE_OPEN_SHAREDCACHE),
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

void Database::assertSqlite(int errcode, const std::string& message) {
  if (errcode != SQLITE_DONE && errcode != SQLITE_OK && errcode != SQLITE_ROW) {
    if (message.length() > 0) {
      std::ostringstream err;
      err << message << "; Sqlite error message: " << sqlite3_errstr(errcode);
      throw SqliteErr(errcode, err.str());
    }
    throw SqliteErr(errcode);
  }
}

}  // namespace Common