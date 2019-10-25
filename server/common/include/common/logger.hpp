
#ifndef COMMON_LOGGER_HPP
#define COMMON_LOGGER_HPP

#include <atomic>
#include <mutex>
#include <string>

namespace Common {

class Logger {
 public:
  static int init(const std::string& dbPath);
  static void log(int severity, int provenance, const std::string& log, int logSessionId);

 private:
  Logger() {}  // really?

  static std::mutex mutex_;
  static bool isInitialized_;
  static bool logSessionId_;
  static std::string dbPath_;
  static int logCount_;
};

}  // namespace Common

#endif  // COMMON_LOGGER_HPP
