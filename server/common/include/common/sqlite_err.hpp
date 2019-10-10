#ifndef COMMON_SQLITE_ERR_HPP
#define COMMON_SQLITE_ERR_HPP

#include <stdexcept>
#include <sqlite3.h>

namespace Common {

class sqlite_err : public std::runtime_error {
public:
    sqlite_err(int code = SQLITE_OK);
    sqlite_err(int, const std::string&);
    virtual ~sqlite_err();

    int code();

private:
    int code_;
};

}

#endif // COMMON_SQLITE_ERR_HPP