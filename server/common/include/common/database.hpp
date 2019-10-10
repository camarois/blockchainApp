
#ifndef COMMON_DATABASE_HPP
#define COMMON_DATABASE_HPP

#include <sqlite3.h>
#include "sqlite_err.hpp"
#include <cstddef>
#include <string>

namespace Common {

class Database {
    public:
        static void assertSqlite(int errcode, const std::string& message = "");
    
    public:
        explicit Database();
        ~Database();
    
    private:
        sqlite3* db_;
        const std::string kDatabaseName_ = "blockchain.db";
};

}

#endif // COMMON_DATABASE_HPP