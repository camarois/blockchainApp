#ifndef COMMON_DATABASE_HPP
#define COMMON_DATABASE_HPP

#include <cstddef>
#include <memory>
#include <nlohmann/json.hpp>
#include <sqlite3.h>
#include <string>

#include <common/database_models.hpp>
#include <common/miner_models.hpp>
#include <common/models.hpp>
#include <common/query.hpp>
#include <common/sqlite_err.hpp>
#include <common/statement.hpp>

namespace Common {

enum Functions {
  AddUser,
  SetUserPassword,
  ContainsUser,
  ContainsAdmin,
  GetRole,
  GetSalt,
  CheckForExistingClass,
  DeleteExistingClass,
  DeleteExistingResults,
  AddNewClass,
  AddNewResult,
  GetClassResult,
  GetStudentResult,
  GetClasses,
  GetStudents
};

class Database {
 public:
  explicit Database();
  explicit Database(const std::string& dbPath);

  static void assertSqlite(int errCode, const std::string& message = "");

  Common::Models::SqlResponse get(const Common::Models::SqlRequest& sql);

  void addUser(const Common::Models::AddUserRequest& request);
  void setUserPassword(const Common::Models::SetUserPasswordRequest& request);
  bool containsUser(const Common::Models::ContainsUserRequest& request);
  bool containsAdmin(const Common::Models::ContainsAdminRequest& request);
  std::optional<bool> getRole(const Common::Models::GetRoleRequest& request);
  std::optional<std::string> getSalt(const std::string& username);

  std::vector<std::string> getIps();
  void addIp(const std::string& ip);
  bool containsIp(const std::string& ip);

  int addLogSession();
  void addLog(int logId, int severity, int provenance, const std::string& time, const std::string& log,
              int logSessionId);
  std::vector<Common::Models::Information> getLogs(int lastLogId, int provenance);

  std::optional<int> checkForExistingClass(const Common::Models::CheckForExistingClassRequest& request);
  void deleteExistingClass(int classId);
  void deleteExistingResults(int classId);
  int addNewClass(const Common::Models::TransactionRequest& transactionRequest);
  void addNewResult(const Common::Models::AddNewResultRequest& request);
  std::vector<Common::Models::Result> getClassResult(int classId);
  std::vector<Common::Models::StudentResult> getStudentResult(const Common::Models::StudentRequest& studentRequest);
  std::vector<Common::Models::ClassInfo> getClasses();
  std::vector<Common::Models::StudentInfo> getStudents();

 private:
  void close();

  std::shared_ptr<sqlite3> db_;
};

}  // namespace Common

#endif  // COMMON_DATABASE_HPP
