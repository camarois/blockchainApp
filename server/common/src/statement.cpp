#include "common/statement.hpp"

#include "common/database.hpp"
#include "common/sqlite_err.hpp"
#include <iostream>
#include <sstream>

namespace Common {

Statement::Statement(std::shared_ptr<sqlite3> db, const Query& query) {
  const auto& q = query.val();
  int errCode = sqlite3_prepare_v2(db.get(), q.c_str(), q.length(), reinterpret_cast<sqlite3_stmt**>(&statement_),
                                   nullptr);  // NOLINT
  Database::assertSqlite(errCode);
}

std::string Statement::getColumnText(size_t col) const {
  return reinterpret_cast<const char*>(sqlite3_column_text(&(*statement_), col));  // NOLINT
}

bool Statement::step() {
  int errCode = sqlite3_step(&(*statement_));
  Database::assertSqlite(errCode);
  return errCode == SQLITE_ROW;
}

}  // namespace Common
