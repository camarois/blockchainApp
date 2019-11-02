#include "common/database.hpp"

#include <common/format_helper.hpp>
#include <common/scripts_helper.hpp>
#include <gflags/gflags.h>

namespace Common {

Database::Database(const std::string& dbPath) {
  Common::ScriptsHelper::createDb(dbPath);
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
    throw SqliteErr(message + " -> Sqlite error message: " + sqlite3_errstr(errCode));
  }
}

std::optional<std::string> Database::getSalt(const std::string& username, bool isAdmin) {
  std::string table = isAdmin ? "admin" : "users";
  Query query = Query("SELECT salt FROM " + table + " WHERE username = '%q';", username.c_str());
  Statement statement = Statement(db_, query);
  if (statement.step()) {
    return statement.getColumnText(0);
  }
  return {};
}

bool Database::containsUser(const Common::Models::LoginRequest& loginRequest, const std::string& salt, bool isAdmin) {
  std::string table = isAdmin ? "admin" : "users";
  Query query = Query("SELECT username FROM " + table + " WHERE username = '%q' AND password = '%q';",
                      loginRequest.username.c_str(), Common::FormatHelper::hash(loginRequest.password + salt).c_str());
  Statement statement = Statement(db_, query);
  return statement.step();
}

void Database::addUser(const Common::Models::LoginRequest& user, bool isAdmin) {
  std::string table = isAdmin ? "admin" : "users";
  auto salt = Common::FormatHelper::randomStr();
  Query query = Query("INSERT OR REPLACE INTO " + table +
                          " (username, password, salt) "
                          "VALUES ('%q', '%q', '%q');",
                      user.username.c_str(), Common::FormatHelper::hash(user.password + salt).c_str(), salt.c_str());
  Statement statement = Statement(db_, query);
  statement.step();
}

void Database::setUserPassword(const std::string& username, const Common::Models::PasswordRequest& passwords,
                               const std::string& salt, bool isAdmin) {
  std::string table = isAdmin ? "admin" : "users";
  Query query = Query("UPDATE " + table +
                          " SET password = '%q' "
                          "WHERE username = '%q' AND password = '%q';",
                      Common::FormatHelper::hash(passwords.newPwd + salt).c_str(), username.c_str(),
                      Common::FormatHelper::hash(passwords.oldPwd + salt).c_str());
  Statement statement = Statement(db_, query);
  statement.step();
}

std::vector<std::string> Database::getIps() {
  Query query = Query("SELECT ip FROM ips;");
  Statement statement = Statement(db_, query);
  std::vector<std::string> ips;
  while (statement.step()) {
    ips.push_back(statement.getColumnText(0));
  }
  return ips;
}

void Database::addIp(const std::string& ip) {
  Query query = Query(
      "INSERT INTO ips (ip) "
      "VALUES ('%q');",
      ip.c_str());
  Statement statement = Statement(db_, query);
  statement.step();
}

bool Database::containsIp(const std::string& ip) {
  Query query = Query(
      "SELECT ip FROM ips "
      "WHERE ip = '%q';",
      ip.c_str());
  Statement statement = Statement(db_, query);
  return statement.step();
}

int Database::addLogSession() {
  Query query = Query(
      "INSERT INTO logSessions (startTime) "
      "VALUES ('%q');",
      Common::FormatHelper::nowStr().c_str());
  Statement statement = Statement(db_, query);
  statement.step();
  return sqlite3_last_insert_rowid(db_.get());
}

void Database::addLog(int logId, int severity, int provenance, const std::string& time, const std::string& log,
                      int logSessionId) {
  Query query = Query(
      "INSERT INTO logs (logId, severity, logTime, provenance, log, logSessionId) "
      "VALUES ('%q', '%q', '%q', '%q', '%q', '%q');",
      std::to_string(logId).c_str(), std::to_string(severity).c_str(), time.c_str(), std::to_string(provenance).c_str(),
      log.c_str(), std::to_string(logSessionId).c_str());
  Statement statement = Statement(db_, query);
  statement.step();
}

std::vector<Common::Models::Information> Database::getLogs(int lastLogId, int provenance) {
  Query query = lastLogId ? Query(
                                "SELECT logId, severity, logTime, log FROM logs "
                                "WHERE logId > '%q' AND provenance = '%q';",
                                std::to_string(lastLogId).c_str(), std::to_string(provenance).c_str())
                          : Query(
                                "SELECT logId, severity, logTime, log FROM logs "
                                "WHERE provenance = '%q'"
                                "ORDER BY logId DESC LIMIT 20;",
                                std::to_string(provenance).c_str());
  Statement statement = Statement(db_, query);

  std::vector<Common::Models::Information> logs;
  while (statement.step()) {
    logs.push_back({std::stoi(statement.getColumnText(0)), statement.getColumnText(1), statement.getColumnText(2),
                    statement.getColumnText(3)});
  }
  return logs;
}

}  // namespace Common
