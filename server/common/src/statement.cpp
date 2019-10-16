#include <iostream>
#include <sstream>

#include "common/database.hpp"
#include "common/sqlite_err.hpp"
#include "common/statement.hpp"

namespace Common {

Statement::Statement(sqlite3* db, const Query& query) {
  const auto& q = query.val();
  int errCode = sqlite3_prepare_v2(db, q.c_str(), q.length(), reinterpret_cast<sqlite3_stmt**>(&state_), NULL);
  Database::assertSqlite(errCode);
}

std::string Statement::getColumnText(size_t col) const { return reinterpret_cast<const char*>(sqlite3_column_text(&(*state_), col)); }

bool Statement::step() {
  int errCode = sqlite3_step(&(*state_));
  Database::assertSqlite(errCode);
  return errCode == SQLITE_ROW;
}

}  // namespace Common
