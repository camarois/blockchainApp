#include <common/database.hpp>
#include <common/models.hpp>
#include <gtest/gtest.h>

TEST(User, get_salt_and_contains) {
  Common::Database db("test-blockchain.db");

  Common::Models::LoginRequest expectedUser = {"Anne-Sophie Provencher", "LOL1234!"};
  Common::Models::AddUserRequest addUserRequest = {
      .loginRequest = expectedUser,
      .isAdmin = false,
      .isEditor = false,
  };

  Common::Models::LoginRequest expectedEditor = {"Jolyne Rodrigue", "editrice"};
  Common::Models::AddUserRequest addEditorRequest = {
      .loginRequest = expectedEditor,
      .isAdmin = false,
      .isEditor = true,
  };

  Common::Models::LoginRequest unexpectedUser = {"Joker", "muhaha"};

  db.addUser(addUserRequest);
  db.addUser(addEditorRequest);

  auto userSalt = db.getSalt({.username = expectedUser.username,});
  ASSERT_TRUE(userSalt.has_value());

  auto editorSalt = db.getSalt({.username = expectedEditor.username,});
  ASSERT_TRUE(editorSalt.has_value());

  auto receivedUser = db.containsUser({expectedUser, userSalt.value()});
  ASSERT_TRUE(receivedUser);

  auto receivedEditor = db.containsUser({expectedEditor, editorSalt.value()});
  ASSERT_TRUE(receivedEditor);
}

TEST(User, change_password) {
  Common::Database db("test-blockchain.db");
  Common::Models::LoginRequest expectedUser = {"Anne-Sophie Provencher", "LOL1234!"};
  auto salt = db.getSalt({expectedUser.username});
  ASSERT_TRUE(salt.has_value());
  EXPECT_NO_THROW(db.setUserPassword({expectedUser.username, {expectedUser.password, "1234"}, salt.value(), false, false}));
}

TEST(User, get_role) {
  Common::Database db("test-blockchain.db");
  Common::Models::LoginRequest expectedUser = {"Anne-Sophie Provencher", "LOL1234!"};
  Common::Models::LoginRequest expectedEditor = {"Jolyne Rodrigue", "editrice"};

  auto userSalt = db.getSalt({expectedUser.username});
  ASSERT_TRUE(userSalt.has_value());

  auto editorSalt = db.getSalt({expectedEditor.username});
  ASSERT_TRUE(editorSalt.has_value());

  auto userRole = db.getRole({expectedUser, userSalt.value()});
  ASSERT_FALSE(userRole.has_value());

  auto editorRole = db.getRole({expectedEditor, editorSalt.value()});
  ASSERT_TRUE(editorRole.has_value());
}

TEST(User, complete_delete) {
  Common::Database db("test-blockchain.db");
  Common::Models::LoginRequest expectedUser = {"Anne-Sophie Provencher", "LOL1234!"};
  
  EXPECT_NO_THROW(db.deleteUser({expectedUser.username}));
}

TEST(Admin, get_salt_and_contains) {
  Common::Database db("test-blockchain.db");
  Common::Models::LoginRequest expectedUser = {"admin", "equipe01"};
  auto salt = db.getSalt({expectedUser.username});
  ASSERT_TRUE(salt.has_value());
  
  auto receivedAdmin = db.containsAdmin({expectedUser, salt.value(), true});
  ASSERT_TRUE(receivedAdmin);
}

TEST(Admin, change_password) {
  Common::Database db("test-blockchain.db");
  Common::Models::LoginRequest expectedUser = {"admin", "equipe01"};
  auto salt = db.getSalt({expectedUser.username});
  ASSERT_TRUE(salt.has_value());
  EXPECT_NO_THROW(db.setUserPassword({expectedUser.username, {expectedUser.password, "1234"}, salt.value(), true, true}));
}

TEST(Transaction, test_transaction) {  
  Common::Database db("test-blockchain.db");
  Common::Models::Result expectedResult1 = {"Tremblay", "Michel", "12345678", "69.00"};
  Common::Models::Result expectedResult2 = {"Smith", "John", "87654321", "42.00"};
  std::vector<Common::Models::Result> expectedResults;
  expectedResults.push_back(expectedResult1);
  expectedResults.push_back(expectedResult2);

  Common::Models::TransactionRequest transaction = {"inf3995", "Projet3", 20193, expectedResults};
  std::optional<int> classId = db.checkForExistingClass({transaction.acronym, transaction.trimester});
  if (classId){
    db.deleteExistingClass(classId.value());
    db.deleteExistingResults(classId.value());
  }
  int newClassId = db.addNewClass(transaction);
  db.addNewResult({transaction, newClassId});

  Common::Models::ClassesRequest classesRequest = {"inf3995", 20193};
  classId = db.checkForExistingClass({classesRequest.acronym, classesRequest.trimester});
  std::vector<Common::Models::Result> receivedResults;
  if (classId){
    receivedResults = db.getClassResult(classId.value());
  }
}
