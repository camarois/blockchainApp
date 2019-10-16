#include <common/database.hpp>
#include <common/models.hpp>
#include <filesystem>
#include <gtest/gtest.h>

TEST(Sqlite3Tests, create_user) {
  Common::Database db(std::filesystem::current_path() / "../../test-blockchain.db");
  Common::Models::LoginRequest expected_user = {
    "Anne-Sophie Provencher",
    "LOL1234!"
  };

  db.createUser(&expected_user);

  Common::Models::LoginRequest received_user = db.getUser(expected_user.username);

  ASSERT_EQ(expected_user.username, received_user.username);
  ASSERT_EQ(expected_user.password, received_user.password);
}
