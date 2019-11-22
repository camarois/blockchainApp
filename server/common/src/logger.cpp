#include "common/logger.hpp"

#include <common/database.hpp>
#include <common/format_helper.hpp>
#include <magic_enum.hpp>

namespace Common {

std::shared_ptr<Logger> Logger::instance;

// NOLINTNEXTLINE
Logger::Logger(int logSessionId) : logSessionId_(logSessionId) {
  logCount_ = 1;
  provenance_ = 0;
}

std::shared_ptr<Logger> Logger::get() {
  if (instance) {
    return instance;
  }
  auto logSessionId = Common::Database::get()->addLogSession();
  instance = std::make_shared<Logger>(logSessionId);
  std::cout << "Logger created" << std::endl;
  return instance;
}

void Logger::setProvenance(int provenance) { provenance_ = provenance; }

void Logger::error(const std::string& message) { log(Severity::ERROR, message, std::cerr); }

void Logger::attention(const std::string& message) { log(Severity::ATTENTION, message, std::cout); }

void Logger::info(const std::string& message) { log(Severity::INFO, message, std::cout); }

void Logger::log(Severity severity, const std::string& message, std::ostream& stream) {
  std::lock_guard<std::mutex> lock(mutex_);
  auto nowStr = Common::FormatHelper::nowStr();
  stream << std::endl
         << logCount_ << ": " << magic_enum::enum_name(severity) << ": " << nowStr << ": " << provenance_ << ": "
         << message << std::endl;
  Common::Database::get()->addLog(logCount_, severity, provenance_, nowStr, message, logSessionId_);
  ++logCount_;
}

}  // namespace Common
