#include <common/database.hpp>
#include <common/format_helper.hpp>
#include <common/models.hpp>
#include <gtest/gtest.h>

TEST(Singleton, init_and_get) {
  EXPECT_THROW(Common::Database::get(), std::runtime_error);
  Common::Database::init("test-blockchain.db");
  EXPECT_NO_THROW(Common::Database::get());
};

TEST(Sqlite_error, get_error) {
  Common::SqliteErr sqliteError;
  ASSERT_EQ(sqliteError.message(), "");
};

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
  Common::Models::LoginRequest expectedEditor = {"Jolyne Rodrigue", "editrice"};
  EXPECT_NO_THROW(db.deleteUser({expectedUser.username}));
  EXPECT_NO_THROW(db.deleteUser({expectedEditor.username}));
}

TEST(User, get_all) {
  Common::Database db("test-blockchain.db");
  auto response = db.getAllUsers();
  Common::Models::UserResponse receivedEditor = response.users.back();
  ASSERT_EQ(receivedEditor.username, "admin");
  ASSERT_TRUE(receivedEditor.isEditor);
  ASSERT_TRUE(receivedEditor.isAdmin);
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

TEST(Ips, add_and_contains_ip) {
  Common::Database db("test-blockchain.db");
  std::string expectedIp = "192.0.0.0";
  db.addIp(expectedIp);
  auto ip = db.containsIp(expectedIp);
  ASSERT_TRUE(ip);
}

TEST(Ips, get_all) {
  Common::Database db("test-blockchain.db");
  std::string expectedIp = "192.0.0.0";
  auto ips = db.getIps();
  ASSERT_TRUE(std::find(ips.begin(), ips.end(), expectedIp) != ips.end());
}

TEST(Server, set_and_get_self_id) {
  Common::Database db("test-blockchain.db");
  int expectedId = 1;
  ASSERT_FALSE(db.getSelfId());
  db.setSelfId(expectedId);
  auto id = db.getSelfId();
  ASSERT_EQ(id, expectedId);
}

TEST(Logs, add_log_and_get_logs) {
  Common::Database db("test-blockchain.db");
  int expectedProvenance = 0;
  int expectedLogSession = 1;
  Common::Models::Information expectedLog = {
    .no = 5,
    .severity = "0",
    .time = Common::FormatHelper::nowStr(),
    .message = "TEST ERROR LOG",
  };

  db.addLogSession();
  db.addLog(expectedLog.no, std::stoi(expectedLog.severity), expectedProvenance, expectedLog.time, expectedLog.message, expectedLogSession);
  
  auto receivedLogs = db.getLogs({expectedLog.no, expectedProvenance});
  ASSERT_TRUE(receivedLogs.empty());

  auto receivedLast20Logs = db.getLogs({0, expectedProvenance});
  Common::Models::Information receivedLog = receivedLast20Logs.back();
  ASSERT_EQ(receivedLog.no, expectedLog.no);
  ASSERT_EQ(receivedLog.severity, expectedLog.severity);
  ASSERT_EQ(receivedLog.time, expectedLog.time);
  ASSERT_EQ(receivedLog.message, expectedLog.message);
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
