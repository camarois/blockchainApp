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
      "INSERT INTO users (username, password) "
      "VALUES ('%q', '%q');",
      user.username.c_str(), Common::FormatHelper::hash(user.password).c_str());
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
      std::to_string(logId).c_str(), std::to_string(severity).c_str(), std::to_string(provenance).c_str(), log.c_str(),
      time.c_str(), std::to_string(logSessionId).c_str());
  Statement statement = Statement(db_, query);
  statement.step();
}

int Database::checkForExistingClass(const std::string& acronym, int trimester) {
  Query checkForExistingClassQuery = Query(
      "SELECT classId FROM classes "
      "WHERE acronym = '%q' AND trimester = '%q' "
      "LIMIT 1;",
      acronym.c_str(), std::to_string(trimester).c_str());
  Statement statementCheck = Statement(db_, checkForExistingClassQuery);
  if (statementCheck.step())
    return std::stoi(statementCheck.getColumnText(0));
  else
    return -1;
}

void Database::DeleteExistingClass(int classId) {
  Query deleteClassQuery = Query("DELETE FROM classes WHERE classId = '%q'", std::to_string(classId).c_str());
  Statement statementDeleteClass = Statement(db_, deleteClassQuery);
  statementDeleteClass.step();
}

void Database::DeleteExistingResults(int classId) {
  Query deleteResultQuery = Query("DELETE FROM results WHERE classId = '%q'", std::to_string(classId).c_str());
  Statement statementDeleteResults = Statement(db_, deleteResultQuery);
  statementDeleteResults.step();
}

int Database::AddNewClass(const Common::Models::TransactionRequest& transactionRequest) {
  Query newClassQuery = Query(
      "INSERT INTO classes (acronym, name, trimester) "
      "VALUES ('%q', '%q', '%q');",
      transactionRequest.acronym.c_str(), transactionRequest.name.c_str(),
      std::to_string(transactionRequest.trimester).c_str());
  Statement statementNewClass = Statement(db_, newClassQuery);
  statementNewClass.step();
  return sqlite3_last_insert_rowid(db_.get());
}

void Database::AddNewResult(const Common::Models::TransactionRequest& transactionRequest, int classId) {
  std::string resultsToAdd = "INSERT INTO results (lastName, firstName, id, grade, classId) VALUES";
  for (Common::Models::Result result : transactionRequest.results) {
    resultsToAdd += " ('" + result.lastName + "', '" + result.firstName + "', '" + result.id + "', '" + result.grade +
                    "', " + std::to_string(classId).c_str() + "),";
  }
  resultsToAdd.replace(resultsToAdd.length() - 1, 1, ";");
  Query resultsToAddQuery = Query(resultsToAdd);
  Statement statementNewResults = Statement(db_, resultsToAddQuery);
  statementNewResults.step();
}

std::vector<Common::Models::Result> Database::getClassResult(int classId) {
  Query getClassResultsQuery = Query(
      "SELECT firstName, lastname, id, grade FROM results "
      "WHERE classId = '%q';",
      std::to_string(classId).c_str());
  Statement getResultsStatement = Statement(db_, getClassResultsQuery);

  std::vector<Common::Models::Result> results;
  while (getResultsStatement.step()) {
    Common::Models::Result result;
    result.firstName = getResultsStatement.getColumnText(0);
    result.lastName = getResultsStatement.getColumnText(1);
    result.id = getResultsStatement.getColumnText(2);
    result.grade = getResultsStatement.getColumnText(3);
    results.push_back(result);
  }

  return results;
}

Common::Models::Result Database::getStudentResult(int classId, const std::string studentId) {
  Query getClassResultsQuery = Query(
      "SELECT firstName, lastname, id, grade FROM results "
      "WHERE classId = '%q' AND id = '%q';",
      std::to_string(classId).c_str(), studentId.c_str());
  Statement getResultsStatement = Statement(db_, getClassResultsQuery);
  Common::Models::Result result;

  if (getResultsStatement.step()) {
    result.firstName = getResultsStatement.getColumnText(0);
    result.lastName = getResultsStatement.getColumnText(1);
    result.id = getResultsStatement.getColumnText(2);
    result.grade = getResultsStatement.getColumnText(3);
  }

  return result;
}
}  // namespace Common
