
#ifndef COMMON_LOGGER_HPP
#define COMMON_LOGGER_HPP

#include <memory>
#include <mutex>
#include <string>
#include <vector>
#include <ostream>

namespace Common {

enum Severity { ERROR, ATTENTION, INFO };

class Logger {
 public:
  explicit Logger(int logSessionId, std::string dbPath);
  static std::shared_ptr<Logger> get();
  static void init(const std::string& dbPath);
  void error(int provenance, const std::string& log);
  void attention(int provenance, const std::string& log);
  void info(int provenance, const std::string& log);

 private:
  void log(int severity, int provenance, const std::string& log, std::ostream& stream);
  static std::shared_ptr<Logger> instance_;
  static bool isInitialized_;
  static std::vector<std::string> severities_;

  std::mutex mutex_;
  const int logSessionId_;
  const std::string dbPath_;
  int logCount_;
};

}  // namespace Common

#endif  // COMMON_LOGGER_HPP
