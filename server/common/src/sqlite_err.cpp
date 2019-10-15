#include "common/sqlite_err.hpp"

namespace Common {
    
SqliteErr::SqliteErr(const std::string& msg) : runtime_error(msg){}

std::string SqliteErr::message() { return msg_; }

}  // namespace Common
