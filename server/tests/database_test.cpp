#include <common/database.hpp>
#include <common/models.hpp>
#include <gtest/gtest.h>

TEST(Sqlite3Tests, get_user) {
  Common::Database db("test-blockchain.db");
  Common::Models::LoginRequest expectedUser = {"Anne-Sophie Provencher", "LOL1234!"};

  db.createUser(expectedUser);

  auto receivedUser = db.getUser(expectedUser.username);

  ASSERT_TRUE(receivedUser.has_value());
  ASSERT_EQ(expectedUser.username, receivedUser->username);
  ASSERT_EQ(expectedUser.password, receivedUser->password);
}
