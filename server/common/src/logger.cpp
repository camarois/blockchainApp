#include "common/logger.hpp"
#include <common/database.hpp>
#include <common/format_helper.hpp>

namespace Common {

std::vector<std::string> Logger::severities = {"ERREUR", "ATTENTION", "INFO"};
bool Logger::isInitialized = false;
std::shared_ptr<Logger> Logger::instance;

Logger::Logger(int logSessionId, const std::string& dbPath) : logSessionId_(logSessionId), dbPath_(dbPath) {
  logCount_ = 1;
}

std::shared_ptr<Logger> Logger::get() { return instance; }

void Logger::init(const std::string& dbPath) {
  if (!isInitialized) {
    Common::Database db(dbPath);
    auto logSessionId = db.addLogSession();
    instance = std::make_shared<Logger>(logSessionId, dbPath);
    isInitialized = true;
    std::cout << "Logger created" << std::endl;
  }
}

void Logger::error(int provenance, const std::string& message) {
  log(static_cast<int>(Severity::ERROR), provenance, message, std::cerr);
}

void Logger::attention(int provenance, const std::string& message) {
  log(static_cast<int>(Severity::ATTENTION), provenance, message, std::cout);
}

void Logger::info(int provenance, const std::string& message) {
  log(static_cast<int>(Severity::INFO), provenance, message, std::cout);
}

void Logger::log(int severity, int provenance, const std::string& message, std::ostream& stream) {
  std::lock_guard<std::mutex> lock(mutex_);
  Common::Database db(dbPath_);
  auto nowStr = Common::FormatHelper::nowStr();
  stream << std::endl
         << logCount_ << ": " << severities.at(severity) << ": " << nowStr << ": " << provenance << ": " << message
         << std::endl;
  db.addLog(logCount_, severity, provenance, nowStr, message, logSessionId_);
  ++logCount_;
}

}  // namespace Common
