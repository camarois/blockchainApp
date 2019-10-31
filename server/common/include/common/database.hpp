
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

  bool containsUser(const Common::Models::LoginRequest& loginRequest);
  void addUser(const Common::Models::LoginRequest& user);

  std::vector<std::string> getIps();
  void addIp(const std::string& ip);
  bool containsIp(const std::string& ip);

  int addLogSession();
  void addLog(int logId, int severity, int provenance, const std::string& time, const std::string& log, int logSessionId);

 private:
  void close();

  std::shared_ptr<sqlite3> db_;
};

}  // namespace Common

#endif  // COMMON_DATABASE_HPP
