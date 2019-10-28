
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
  explicit Database(const std::string& dbPath);

  static void assertSqlite(int errCode, const std::string& message = "");

  std::optional<Common::Models::LoginRequest> getUser(const std::string& username);
  void addUser(const Common::Models::LoginRequest& user);

  std::vector<std::string> getIps();
  void addIp(const std::string& ip);
  bool containsIp(const std::string& ip);

  int addLogSession();
  void addLog(int logId, int severity, int provenance, const std::string& time, const std::string& log, int logSessionId);

  void addTransactionRequest(const Common::Models::TransactionRequest& transactionRequest);
  std::vector<Common::Models::Result> addClassesRequest(const Common::Models::ClassesRequest& classesRequest);
  Common::Models::Result addStudentRequest(const Common::Models::StudentRequest& studentRequest);

 private:
  void close();

  std::shared_ptr<sqlite3> db_;
};

}  // namespace Common

#endif  // COMMON_DATABASE_HPP
