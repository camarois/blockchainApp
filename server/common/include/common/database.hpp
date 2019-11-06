
#ifndef COMMON_DATABASE_HPP
#define COMMON_DATABASE_HPP

#include "common/models.hpp"
#include "common/query.hpp"
#include "common/statement.hpp"
#include "sqlite_err.hpp"
#include <cstddef>
#include <memory>
#include <sqlite3.h>
#include <string>

namespace Common {

class Database {
 public:
  explicit Database();
  explicit Database(const std::string& dbPath);

  static void assertSqlite(int errCode, const std::string& message = "");

  void addUser(const Common::Models::LoginRequest& user, bool isAdmin = false);
  void setUserPassword(const std::string& username, const Common::Models::PasswordRequest& passwordRequest,
                       const std::string& salt, bool isAdmin = false);
  bool containsUser(const Common::Models::LoginRequest& loginRequest, const std::string& salt, bool isAdmin = false);
  std::optional<std::string> getSalt(const std::string& username);

  std::vector<std::string> getIps();
  void addIp(const std::string& ip);
  bool containsIp(const std::string& ip);

  int addLogSession();
  void addLog(int logId, int severity, int provenance, const std::string& time, const std::string& log,
              int logSessionId);
  std::vector<Common::Models::Information> getLogs(int lastLogId, int provenance);

  std::optional<int> checkForExistingClass(const std::string& acronym, int trimester);
  void DeleteExistingClass(int classId);
  void DeleteExistingResults(int classId);
  int AddNewClass(const Common::Models::TransactionRequest& transactionRequest);
  void AddNewResult(const Common::Models::TransactionRequest& transactionRequest, int classId);
  std::vector<Common::Models::Result> getClassResult(int classId);
  std::vector<Common::Models::StudentResult> getStudentResult(const Common::Models::StudentRequest& studentRequest);

 private:
  void close();

  std::shared_ptr<sqlite3> db_;
};

}  // namespace Common

#endif  // COMMON_DATABASE_HPP
