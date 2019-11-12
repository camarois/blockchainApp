
#ifndef COMMON_LOGGER_HPP
#define COMMON_LOGGER_HPP

#include <memory>
#include <mutex>
#include <ostream>
#include <string>
#include <vector>
#include <atomic>

namespace Common {

enum Severity { ERROR, ATTENTION, INFO };

class Logger {
 public:
  explicit Logger(int logSessionId);
  static std::shared_ptr<Logger> get();
  void error(int provenance, const std::string& message);
  void attention(int provenance, const std::string& message);
  void info(int provenance, const std::string& message);

 private:
  void log(Severity severity, int provenance, const std::string& message, std::ostream& stream);

  static std::shared_ptr<Logger> instance;

  std::mutex mutex_;
  const int logSessionId_;
  std::atomic<int> logCount_;
};

}  // namespace Common

#endif  // COMMON_LOGGER_HPP
