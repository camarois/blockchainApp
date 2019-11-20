#ifndef COMMON_DATABASE_HPP
#define COMMON_DATABASE_HPP

#include <common/database_models.hpp>
#include <common/miner_models.hpp>
#include <common/models.hpp>
#include <common/query.hpp>
#include <common/sqlite_err.hpp>
#include <common/statement.hpp>
#include <cstddef>
#include <functional>
#include <memory>
#include <mutex>
#include <nlohmann/json.hpp>
#include <sqlite3.h>
#include <string>

namespace Common {

class Database {
 public:
  explicit Database(const std::string& dbPath);

  static void assertSqlite(int errCode, const std::string& message = "");
  static std::shared_ptr<Database> get();
  static void init(const std::string& dbPath);

  Common::Models::SqlResponse executeRequest(const Common::Models::SqlRequest& sql);

  void addUser(const Common::Models::AddUserRequest& request);
  void setUserPassword(const Common::Models::SetUserPasswordRequest& request);
  bool containsUser(const Common::Models::ContainsUserRequest& request);
  bool containsAdmin(const Common::Models::ContainsAdminRequest& request);
  std::optional<bool> getRole(const Common::Models::GetRoleRequest& request);
  std::optional<std::string> getSalt(const Common::Models::GetSaltRequest& request);

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
  std::vector<Common::Models::User> getAllUsers();
  

 private:
  void close();
  void initFunctions();

  static std::shared_ptr<Database> instance;

  std::shared_ptr<sqlite3> db_;
  std::unordered_map<Common::Functions, std::function<Common::Models::SqlResponse(nlohmann::json)>> functions_;
};

}  // namespace Common

#endif  // COMMON_DATABASE_HPP
