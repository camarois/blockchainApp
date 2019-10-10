#include "common/sqlite_err.hpp"

namespace Common {

sqlite_err::sqlite_err(int code)
    : runtime_error(sqlite3_errstr(code))
    , code_(code)
{ }

sqlite_err::sqlite_err(int code, const std::string& msg)
    : runtime_error(msg)
    , code_(code)
{ }

sqlite_err::~sqlite_err()
{ }

int sqlite_err::code() {
    return code_;
}

}