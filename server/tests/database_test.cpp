#include <common/database.hpp>
#include <common/models.hpp>
#include <gtest/gtest.h>

TEST(Sqlite3Tests, get_user) {
  Common::Database db("test-blockchain.db");
  Common::Models::LoginRequest expectedUser = {"Anne-Sophie Provencher", "LOL1234!"};
  std::string expectedHash = "da6a850377faa387cea7c58a6ebd5935d5502a95aa0993848f8ae4ab8efc68ad";

  db.addUser(expectedUser);

  auto salt = db.getSalt(expectedUser.username);
  std::cout << salt.value() << std::endl;
  ASSERT_TRUE(salt.has_value());
  auto receivedUser = db.containsUser(expectedUser, salt.value());
  ASSERT_TRUE(receivedUser);
}
