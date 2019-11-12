#include "common/database.hpp"

#include "common/miner_models.hpp"
#include <common/format_helper.hpp>
#include <common/scripts_helper.hpp>
#include <gflags/gflags.h>
#include <iostream>

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

    addUser({{"admin", "equipe01"}, true});
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

Common::Models::SqlResponse Database::get(const Common::Models::SqlRequest& sql) {
  switch (sql.function) {
    case Functions::AddUser: {
      addUser(nlohmann::json::parse(sql.params));
      return {false, ""};
    }
    case Functions::SetUserPassword: {
      setUserPassword(nlohmann::json::parse(sql.params));
      return {false, ""};
    }
    case Functions::ContainsUser: {
      return {containsUser(nlohmann::json::parse(sql.params)), ""};
    }
    case Functions::ContainsAdmin: {
      return {containsAdmin(nlohmann::json::parse(sql.params)), ""};
    }
    case Functions::GetRole: {
      auto edition = getRole(nlohmann::json::parse(sql.params));
      return {edition.has_value(), edition.has_value() ? std::to_string(static_cast<int>(edition.value())) : ""};
    }
    case Functions::GetSalt: {
      auto salt = getSalt(sql.params);
      return {salt.has_value(), salt.has_value() ? salt.value() : ""};
    }
    case Functions::CheckForExistingClass: {
      auto classId = checkForExistingClass(nlohmann::json::parse(sql.params));
      return {classId.has_value(), classId.has_value() ? std::to_string(classId.value()) : ""};
    }
    case Functions::DeleteExistingClass: {
      deleteExistingClass(nlohmann::json::parse(sql.params));
      return {false, ""};
    }
    case Functions::DeleteExistingResults: {
      deleteExistingResults(nlohmann::json::parse(sql.params));
      return {false, ""};
    }
    case Functions::AddNewClass: {
      return {true, std::to_string(addNewClass(nlohmann::json::parse(sql.params)))};
    }
    case Functions::AddNewResult: {
      addNewResult(nlohmann::json::parse(sql.params));
      return {false, ""};
    }
    case Functions::GetClassResult: {
      return {true, Common::Models::toStr(getClassResult(nlohmann::json::parse(sql.params)))};
    }
    case Functions::GetStudentResult: {
      return {true, Common::Models::toStr(getStudentResult(nlohmann::json::parse(sql.params)))};
    }
    case Functions::GetClasses: {
      return {true, Common::Models::toStr(getClasses())};
    }
    case Functions::GetStudents: {
      return {true, Common::Models::toStr(getStudents())};
    }

    default:
      throw std::runtime_error("Function not allowed:" + std::to_string(sql.function));
  }
  return {};
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

bool Database::containsUser(const Common::Models::ContainsUserRequest& request) {
  Query query = Query(
      "SELECT username FROM users "
      "WHERE username = '%q' AND password = '%q';",
      request.loginRequest.username.c_str(),
      Common::FormatHelper::hash(request.loginRequest.password + request.salt).c_str());
  Statement statement = Statement(db_, query);
  return statement.step();
}

bool Database::containsAdmin(const Common::Models::ContainsAdminRequest& request) {
  Query query = Query(
      "SELECT username FROM users "
      "WHERE username = '%q' AND password = '%q' AND isAdmin = '%q';",
      request.loginRequest.username.c_str(),
      Common::FormatHelper::hash(request.loginRequest.password + request.salt).c_str(),
      std::to_string(int(request.isAdmin)).c_str());
  Statement statement = Statement(db_, query);
  return statement.step();
}

std::optional<bool> Database::getRole(const Common::Models::GetRoleRequest& request) {
  Query query = Query(
      "SELECT isAdmin FROM users "
      "WHERE username = '%q' AND password = '%q';",
      request.loginRequest.username.c_str(),
      Common::FormatHelper::hash(request.loginRequest.password + request.salt).c_str());
  Statement statement = Statement(db_, query);
  if (statement.step()) {
    return std::stoi(statement.getColumnText(0));
  }

  return {};
}

void Database::addUser(const Common::Models::AddUserRequest& request) {
  auto salt = Common::FormatHelper::randomStr();
  Query query = Query(
      "INSERT OR REPLACE INTO users "
      "(username, password, salt, isAdmin) "
      "VALUES ('%q', '%q', '%q', '%q');",
      request.loginRequest.username.c_str(), Common::FormatHelper::hash(request.loginRequest.password + salt).c_str(),
      salt.c_str(), std::to_string(int(request.isAdmin)).c_str());
  Statement statement = Statement(db_, query);
  statement.step();
}

void Database::setUserPassword(const Common::Models::SetUserPasswordRequest& request) {
  Query query = Query(
      "UPDATE users "
      "SET password = '%q' "
      "WHERE username = '%q' AND password = '%q' AND isAdmin = '%q';",
      Common::FormatHelper::hash(request.passwordRequest.newPwd + request.salt).c_str(), request.username.c_str(),
      Common::FormatHelper::hash(request.passwordRequest.oldPwd + request.salt).c_str(),
      std::to_string(int(request.isAdmin)).c_str());
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
                                     "WHERE provenance = '%q' "
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

std::optional<int> Database::checkForExistingClass(const Common::Models::CheckForExistingClassRequest& request) {
  Query checkForExistingClassQuery = Query(
      "SELECT classId FROM classes "
      "WHERE acronym = '%q' AND trimester = '%q' "
      "LIMIT 1;",
      request.acronym.c_str(), std::to_string(request.trimester).c_str());
  Statement statementCheck = Statement(db_, checkForExistingClassQuery);
  if (statementCheck.step()) {
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

void Database::addNewResult(const Common::Models::AddNewResultRequest& request) {
  std::string resultsToAdd = "INSERT INTO results (lastName, firstName, id, grade, classId) VALUES";
  for (const Common::Models::Result& result : request.transactionRequest.results) {
    resultsToAdd += " ('" + result.lastName + "', '" + result.firstName + "', '" + result.id + "', '" + result.grade +
                    "', " + std::to_string(request.classId) + "),";
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

std::vector<Common::Models::ClassInfo> Database::getClasses() {
  Query getClassesQuery = Query(
      "SELECT DISTINCT acronym, name, trimester "
      "FROM classes;");
  Statement getClassesStatement = Statement(db_, getClassesQuery);
  std::vector<Common::Models::ClassInfo> result;
  while (getClassesStatement.step()) {
    result.push_back({.acronym = getClassesStatement.getColumnText(0),
                      .name = getClassesStatement.getColumnText(1),
                      .trimester = std::stoi(getClassesStatement.getColumnText(2))});
  }
  return result;
}

std::vector<Common::Models::StudentInfo> Database::getStudents() {
  Query getStudentQuery = Query(
      "SELECT DISTINCT firstName, lastName, id "
      "FROM results;");
  Statement getStudentStatement = Statement(db_, getStudentQuery);
  std::vector<Common::Models::StudentInfo> result;
  while (getStudentStatement.step()) {
    result.push_back({.lastName = getStudentStatement.getColumnText(0),
                      .firstName = getStudentStatement.getColumnText(1),
                      .id = getStudentStatement.getColumnText(2)});
  }

  return result;
}

}  // namespace Common
