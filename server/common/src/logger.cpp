#include "common/logger.hpp"
#include <common/database.hpp>
#include <common/format_helper.hpp>

namespace Common {

std::mutex Logger::mutex_;
bool Logger::isInitialized_;
bool Logger::logSessionId_;
std::string Logger::dbPath_;
int Logger::logCount_;

int Logger::init(const std::string& dbPath) {
  if (!Logger::isInitialized_) {
    Logger::dbPath_ = dbPath;
    Logger::logCount_ = 1;
    Common::Database db(dbPath);
    Logger::logSessionId_ = db.addLogSession();
    Logger::isInitialized_ = true;
  }
  return Logger::logSessionId_;
}

void Logger::log(int severity, int provenance, const std::string& log, int logSessionId) {
  std::lock_guard<std::mutex> lock(mutex_);
  Common::Database db(Logger::dbPath_);
  auto nowStr = Common::FormatHelper::nowStr();
  std::cout << Logger::logCount_ << ": " << severity << ": " << nowStr << ": " << provenance << ": " << log
            << std::endl;
  db.addLog(Logger::logCount_, severity, provenance, nowStr, log, logSessionId);
  ++Logger::logCount_;
}

}  // namespace Common
