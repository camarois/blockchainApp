#include <iostream>
#include <memory>
#include <sqlite3.h>
#include <stdio.h>

struct Sqlite3Deleter {
    void operator () (sqlite3* db) const { sqlite3_close(db); }
};

using sqlite3_ptr = std::unique_ptr<sqlite3, Sqlite3Deleter>;

int main(int argc, char* argv[]) {
  sqlite3_ptr db;
  int rc;

  rc = sqlite3_open("blockchain.db", reinterpret_cast<sqlite3**> (&db));

  if (rc) {
    std::cout << "Error" << std::endl;
    std::cerr << "Can't open database:" << sqlite3_errmsg(&(*db)) << std::endl;
    return (0);
  } else {
    std::cout << "Opened database successfully\n";
  }
}