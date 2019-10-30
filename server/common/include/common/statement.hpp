#ifndef COMMON_STATEMENT_HPP
#define COMMON_STATEMENT_HPP

#include "query.hpp"
#include "sqlite_err.hpp"
#include <memory>
#include <sqlite3.h>
#include <string>

namespace Common {

struct Sqlite3StmtDeleter {
  void operator()(sqlite3_stmt* statement) const { sqlite3_finalize(statement); }
};

using sqlite3_statement_ptr = std::unique_ptr<sqlite3_stmt, Sqlite3StmtDeleter>;

class Statement {
 public:
  Statement(const std::shared_ptr<sqlite3>& db, const Query& query);

  std::string getColumnText(size_t col) const;

  bool step();

 private:
  sqlite3_statement_ptr statement_;
};

}  // namespace Common

#endif  // COMMON_STATEMENT_HPP
