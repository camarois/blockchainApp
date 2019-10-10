#include "common/database.hpp"
#include <sstream> 

using namespace Common;

Database::Database() {
    assertSqlite(!sqlite3_threadsafe());
    assertSqlite(sqlite3_initialize(), "Unable to initialize SQLite");
    assertSqlite(sqlite3_enable_shared_cache(true), "Cannot enable db shared cache mode");
    try {
        assertSqlite(
            sqlite3_open_v2(kDatabaseName_.c_str(), &db_, SQLITE_OPEN_READWRITE | SQLITE_OPEN_SHAREDCACHE, NULL),
            "Cannot connect to database");
    } catch (...) {
        sqlite3_close_v2(db_);
        throw;
    }
}

Database::~Database() {
    sqlite3_close_v2(db_);
    sqlite3_shutdown();
}

void Database::assertSqlite(int errcode, const std::string& message) {
    if (errcode != SQLITE_DONE
        && errcode != SQLITE_OK
        && errcode != SQLITE_ROW) {
        if (message.length() > 0) {
            std::ostringstream err;
            err << message << "; Sqlite error message: " << sqlite3_errstr(errcode);
            throw sqlite_err(errcode, err.str());
        }
        else {
            throw sqlite_err(errcode);
        }
    }
}