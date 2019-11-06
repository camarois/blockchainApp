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

std::optional<std::string> Database::getSalt(const std::string& username) {
  Query query = Query(
      "SELECT salt FROM users "
      "WHERE username = '%q';",
      username.c_str());
  Statement statement = Statement(db_, query);
  if (statement.step()) {
    return statement.getColumnText(0);
  }
  return {};
}

bool Database::containsUser(const Common::Models::LoginRequest& loginRequest, const std::string& salt, bool isAdmin) {
  Query query = Query(
      "SELECT username FROM users "
      "WHERE username = '%q' AND password = '%q' AND isAdmin = '%q';",
      loginRequest.username.c_str(), Common::FormatHelper::hash(loginRequest.password + salt).c_str(),
      std::to_string(int(isAdmin)).c_str());
  Statement statement = Statement(db_, query);
  return statement.step();
}

void Database::addUser(const Common::Models::LoginRequest& user, bool isAdmin) {
  auto salt = Common::FormatHelper::randomStr();
  Query query = Query(
      "INSERT OR REPLACE INTO users "
      "(username, password, salt, isAdmin) "
      "VALUES ('%q', '%q', '%q', '%q');",
      user.username.c_str(), Common::FormatHelper::hash(user.password + salt).c_str(), salt.c_str(),
      std::to_string(int(isAdmin)).c_str());
  Statement statement = Statement(db_, query);
  statement.step();
}

void Database::setUserPassword(const std::string& username, const Common::Models::PasswordRequest& passwordRequest,
                               const std::string& salt, bool isAdmin) {
  Query query = Query(
      "UPDATE users "
      "SET password = '%q' "
      "WHERE username = '%q' AND password = '%q' AND isAdmin = '%q';",
      Common::FormatHelper::hash(passwordRequest.newPwd + salt).c_str(), username.c_str(),
      Common::FormatHelper::hash(passwordRequest.oldPwd + salt).c_str(), std::to_string(int(isAdmin)).c_str());
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
  Query query = lastLogId != 0 ? Query(
                                "SELECT logId, severity, logTime, log FROM logs "
                                "WHERE logId > '%q' AND provenance = '%q'"
                                "ORDER BY logTime ASC;",
                                std::to_string(lastLogId).c_str(), std::to_string(provenance).c_str())
                          : Query(
                                "SELECT * FROM ("
                                "SELECT logId, severity, logTime, log FROM logs "
                                "WHERE provenance = '%q'"
                                "ORDER BY logId DESC LIMIT 20) "
                                "ORDER BY logTime ASC;",
                                std::to_string(provenance).c_str());
  Statement statement = Statement(db_, query);

  std::vector<Common::Models::Information> logs;
  while (statement.step()) {
    logs.push_back({std::stoi(statement.getColumnText(0)), statement.getColumnText(1), statement.getColumnText(2),
                    statement.getColumnText(3)});
  }
  return logs;
}

std::optional<int> Database::checkForExistingClass(const std::string& acronym, int trimester) {
  Query checkForExistingClassQuery = Query(
      "SELECT classId FROM classes "
      "WHERE acronym = '%q' AND trimester = '%q' "
      "LIMIT 1;",
      acronym.c_str(), std::to_string(trimester).c_str());
  Statement statementCheck = Statement(db_, checkForExistingClassQuery);
  if (statementCheck.step()){
    return std::stoi(statementCheck.getColumnText(0));    
  }

  return {};
}

void Database::deleteExistingClass(int classId) {
  Query deleteClassQuery = Query("DELETE FROM classes WHERE classId = '%q'", std::to_string(classId).c_str());
  Statement statementDeleteClass = Statement(db_, deleteClassQuery);
  statementDeleteClass.step();
}

void Database::deleteExistingResults(int classId) {
  Query deleteResultQuery = Query("DELETE FROM results WHERE classId = '%q'", std::to_string(classId).c_str());
  Statement statementDeleteResults = Statement(db_, deleteResultQuery);
  statementDeleteResults.step();
}

int Database::addNewClass(const Common::Models::TransactionRequest& transactionRequest) {
  Query newClassQuery = Query(
      "INSERT INTO classes (acronym, name, trimester) "
      "VALUES ('%q', '%q', '%q');",
      transactionRequest.acronym.c_str(), transactionRequest.name.c_str(),
      std::to_string(transactionRequest.trimester).c_str());
  Statement statementNewClass = Statement(db_, newClassQuery);
  statementNewClass.step();
  return sqlite3_last_insert_rowid(db_.get());
}

void Database::addNewResult(const Common::Models::TransactionRequest& transactionRequest, int classId) {
  std::string resultsToAdd = "INSERT INTO results (lastName, firstName, id, grade, classId) VALUES";
  for (const Common::Models::Result& result : transactionRequest.results) {
    resultsToAdd += " ('" + result.lastName + "', '" + result.firstName + "', '" + result.id + "', '" + result.grade +
                    "', " + std::to_string(classId) + "),";
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
    results.push_back({.lastName = getResultsStatement.getColumnText(1),
                       .firstName = getResultsStatement.getColumnText(0),
                       .id = getResultsStatement.getColumnText(2),
                       .grade = getResultsStatement.getColumnText(3)});
  }

  return results;
}

std::vector<Common::Models::StudentResult> Database::getStudentResult(
    const Common::Models::StudentRequest& studentRequest) {
  std::string acronym = studentRequest.acronym;
  std::replace(acronym.begin(), acronym.end(), '*', '%');
  std::string trimester = studentRequest.trimester;
  std::replace(trimester.begin(), trimester.end(), '*', '%');
  std::string studentId = studentRequest.id;
  Query getClassResultsQuery = Query(
      "SELECT c.acronym, c.trimester, r.grade "
      "FROM classes c "
      "JOIN results r ON c.classId = r.classId "
      "WHERE c.acronym LIKE '%q' AND r.id = '%q' AND c.trimester LIKE '%q';",
      acronym.c_str(), studentId.c_str(), trimester.c_str());
  Statement getResultsStatement = Statement(db_, getClassResultsQuery);
  std::vector<Common::Models::StudentResult> studentResult;

  while (getResultsStatement.step()) {
    studentResult.push_back({
        .acronym = getResultsStatement.getColumnText(0),
        .trimester = std::stoi(getResultsStatement.getColumnText(1)),
        .grade = getResultsStatement.getColumnText(2),
    });
  }

  return studentResult;
}

  std::vector<Common::Models::CLassInfo> Database::getClasses() {
    Query getClassesQuery = Query(
      "SELECT DISTINC acronym, name, trimester"
      "FROM classes;"
    );
    Statement getClassesStatement = Statement(db_, getClassesQuery);
    std::vector<Common::Models::ClassInfo> result;
    while (getClassesStatement.step()){
      result.push_back({
        .acronym = getClassesStatement.getColumnText(0),
        .name = getClassesStatement.getColumnText(1),
        .trimester = stoi(getResultStatement.getColumnText(2))
      });
    }
    return result;
  }

  std::vector<Common::Models::StudentInfo> Database::getStudents() {
    Query getStudentQuery = Query(
      "SELECT DISTINC firstName, lastName, id"
      "FROM result;"
    );
    Statement getStudentStatement = Statement(db_, getStudentQuery);
    std::vector<Common::Models::StudentInfo> result;
    while (getStudentStatement.step()){
      result.push_back({
        .lastName = getClassesStatement.getColumnText(0),
        .firstName = getClassesStatement.getColumnText(1),
        .id = stoi(getResultStatement.getColumnText(2))
      });
    }
    return result;
  }


}  // namespace Common
