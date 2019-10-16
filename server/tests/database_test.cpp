#include <common/database.hpp>
#include <common/models.hpp>
#include <filesystem>
#include <gtest/gtest.h>

TEST(Sqlite3Tests, create_user) {
  Common::Database db(std::filesystem::current_path() / "../../test-blockchain.db");
  Common::Models::LoginRequest expectedUser = {"Anne-Sophie Provencher", "LOL1234!"};

  db.createUser(&expectedUser);

  Common::Models::LoginRequest receivedUser = db.getUser(expectedUser.username);

  ASSERT_EQ(expectedUser.username, receivedUser.username);
  ASSERT_EQ(expectedUser.password, receivedUser.password);
}
