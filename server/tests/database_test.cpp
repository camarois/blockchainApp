#include <cstdio.h>
#include <iostream>
#include <sqlite3.h>

int main(int argc, char* argv[]) {
  sqlite3* db;
  char* zErrMsg = nullptr;
  int rc;

  rc = sqlite3_open("test.db", &db);

  if (rc) {
    std::cerr << "Can't open database:" << sqlite3_errmsg(db) << std::endl;
    return (0);
  } else {
    std::cout << "Opened database successfully\n";
  }
  sqlite3_close(db);
}