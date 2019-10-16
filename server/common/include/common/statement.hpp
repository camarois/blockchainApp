#ifndef COMMON_STATEMENT_HPP
#define COMMON_STATEMENT_HPP

#include <memory>
#include <sqlite3.h>
#include <string>

#include "query.hpp"

namespace Common {

struct Sqlite3StmtDeleter {
  void operator()(sqlite3_stmt* state) const { sqlite3_finalize(state); }
};

using sqlite3_state_ptr = std::unique_ptr<sqlite3_stmt, Sqlite3StmtDeleter>;

class Statement {
 public:
  Statement(sqlite3* db, const Query& query);

  std::string getColumnText(size_t col) const;

  bool step();

 private:
  sqlite3_state_ptr state_;
};

}  // namespace Common

#endif  // COMMON_STATEMENT_HPP