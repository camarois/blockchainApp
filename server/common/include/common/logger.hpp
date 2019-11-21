
#ifndef COMMON_LOGGER_HPP
#define COMMON_LOGGER_HPP

#include <atomic>
#include <memory>
#include <mutex>
#include <ostream>
#include <string>
#include <vector>

namespace Common {

enum Severity { ERROR, ATTENTION, INFO };

class Logger {
 public:
  explicit Logger(int logSessionId);
  static std::shared_ptr<Logger> get();
  void setProvenance(int provenance);
  void error(const std::string& message);
  void attention(const std::string& message);
  void info(const std::string& message);

 private:
  void log(Severity severity, const std::string& message, std::ostream& stream);

  static std::shared_ptr<Logger> instance;

  std::mutex mutex_;
  const int logSessionId_;
  std::atomic<int> logCount_;
  int provenance_;
};

}  // namespace Common

#endif  // COMMON_LOGGER_HPP
