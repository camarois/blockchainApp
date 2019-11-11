#include "common/logger.hpp"

#include <common/database.hpp>
#include <common/format_helper.hpp>
#include <magic_enum.hpp>

namespace Common {

std::shared_ptr<Logger> Logger::instance;

// NOLINTNEXTLINE
Logger::Logger(int logSessionId, const std::string& dbPath) : logSessionId_(logSessionId), dbPath_(dbPath) {
  logCount_ = 1;
}

std::shared_ptr<Logger> Logger::get() { return instance; }

void Logger::init(const std::string& dbPath) {
  if (!Logger::instance) {
    Common::Database db(dbPath);
    auto logSessionId = db.addLogSession();
    instance = std::make_shared<Logger>(logSessionId, dbPath);
    std::cout << "Logger created" << std::endl;
  }
}

void Logger::error(int provenance, const std::string& message) { log(Severity::ERROR, provenance, message, std::cerr); }

void Logger::attention(int provenance, const std::string& message) {
  log(Severity::ATTENTION, provenance, message, std::cout);
}

void Logger::info(int provenance, const std::string& message) { log(Severity::INFO, provenance, message, std::cout); }

void Logger::log(Severity severity, int provenance, const std::string& message, std::ostream& stream) {
  std::lock_guard<std::mutex> lock(mutex_);
  Common::Database db(dbPath_);
  auto nowStr = Common::FormatHelper::nowStr();
  stream << std::endl
         << logCount_ << ": " << magic_enum::enum_name(severity) << ": " << nowStr << ": " << provenance << ": " << message
         << std::endl;
  db.addLog(logCount_, severity, provenance, nowStr, message, logSessionId_);
  ++logCount_;
}

}  // namespace Common
