#include "common/database.hpp"
#include <common/format_helper.hpp>
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
    throw SqliteErr(message + " -> Sqlite error message: " + sqlite3_errstr(errCode));
  }
}

std::optional<Common::Models::LoginRequest> Database::getUser(const std::string& username) {
  Query query = Query(
      "SELECT username, password FROM users "
      "WHERE username = '%q';",
      username.c_str());
  Statement statement = Statement(db_, query);
  if (statement.step()) {
    return Common::Models::LoginRequest{statement.getColumnText(0), statement.getColumnText(1)};
  }
  return {};
}

void Database::addUser(const Common::Models::LoginRequest& user) {
  Query query = Query(
      "INSERT OR REPLACE INTO users (username, password) "
      "VALUES ('%q', '%q');",
      user.username.c_str(), user.password.c_str());
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

void Database::addLog(int logId, int severity, int provenance, const std::string& time, const std::string& log, int logSessionId) {
  Query query = Query(
      "INSERT INTO logs (logId, severity, logTime, provenance, log, logSessionId) "
      "VALUES ('%q', '%q', '%q', '%q', '%q', '%q');",
      std::to_string(logId).c_str(), std::to_string(severity).c_str(), std::to_string(provenance).c_str(), log.c_str(),
      time.c_str(), std::to_string(logSessionId).c_str());
  Statement statement = Statement(db_, query);
  statement.step();
}

void Database::addTransactionRequest(const Common::Models::TransactionRequest& transactionRequest) {
  Query checkForExistingClassQuery = Query(
      "SELECT TOP 1 classId FROM classes "
      "WHERE symbol = '%q' AND trimester = '%q';", 
      transactionRequest.acronym.c_str(), std::to_string(transactionRequest.trimester)
      );
  Statement statementCheck = Statement(db_, checkForExistingClassQuery);
  std::string classId;
  if (statementCheck.step()) {
    classId = statementCheck.getColumnText(0);
    Query deleteClassQuery = Query(
        "DELETE FROM classes WHERE classId = '%q'",
        classId.c_str()
    );
    Statement statementDeleteClass = Statement(db_, deleteClassQuery);
    statementDeleteClass.step();
    Query deleteResultQuery = Query(
        "DELETE FROM results WHERE classId = '%q'",
        classId.c_str()
    );
    Statement statementDeleteResults = Statement(db_, deleteResultQuery);
    statementDeleteResults.step();
  }
  Query newClassQuery = Query(
      "INSERT INTO classes (acronym, name, trimester) "
      "VALUES ('%q', '%q', '%q');",
      transactionRequest.acronym.c_str(), transactionRequest.name.c_str(), std::to_string(transactionRequest.trimester)
  );
  Statement statementNewClass = Statement(db_, newClassQuery);
  statementNewClass.step();
  std::string resultsToAdd = "";
  for (Common::Models::Result result : transactionRequest.results ) {
    resultsToAdd += "INSERT INTO results (lastName, firstName, id, grade, classId) "
                    "VALUES ('%q', '%q', '%q', '%q', last_insert_rowid())",
                    result.lastName.c_str(), result.firstName.c_str(), result.id.c_str(), result.grade.c_str();
  }
Query resultsToAddQuery = Query(resultsToAdd);
  Statement statementNewResults = Statement(db_, resultsToAddQuery);
  statementNewResults.step();
}
}  // namespace Common
