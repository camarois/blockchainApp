#include "common/sqlite_err.hpp"

namespace Common {

SqliteErr::SqliteErr(int code)
    : runtime_error(sqlite3_errstr(code))
    , code_(code)
{ }

SqliteErr::SqliteErr(int code, const std::string& msg)
    : runtime_error(msg)
    , code_(code)
{ }

int SqliteErr::code() {
    return code_;
}

} // namespace Common