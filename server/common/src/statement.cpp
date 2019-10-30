#include "common/statement.hpp"

#include "common/database.hpp"
#include "common/sqlite_err.hpp"

namespace Common {

Statement::Statement(const std::shared_ptr<sqlite3>& db, const Query& query) {
  const auto& q = query.val();
  sqlite3_stmt* stmtPtr;
  int errCode = sqlite3_prepare_v2(db.get(), q.c_str(), q.length(), &stmtPtr, nullptr);
  Database::assertSqlite(errCode);
  statement_.reset(stmtPtr);
}

std::string Statement::getColumnText(size_t col) const {
  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
  return reinterpret_cast<const char*>(sqlite3_column_text(statement_.get(), col));
}

bool Statement::step(bool unique) {
  int errCode = sqlite3_step(statement_.get());
  Database::assertSqlite(errCode);
  return unique ? errCode == SQLITE_DONE : errCode == SQLITE_ROW;
}

}  // namespace Common
