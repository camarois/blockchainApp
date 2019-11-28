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

  db.executeRequest({Common::Functions::AddUser, Common::Models::toStr(addUserRequest)});
  db.executeRequest({Common::Functions::AddUser, Common::Models::toStr(addEditorRequest)});

  Common::Models::GetSaltRequest getSaltRequest = {.username = expectedUser.username,};
  auto userSalt = db.executeRequest({Common::Functions::GetSalt, Common::Models::toStr(getSaltRequest)});
  ASSERT_TRUE(userSalt.found);

  auto editorSalt = db.executeRequest({Common::Functions::GetSalt, Common::Models::toStr(getSaltRequest)});
  ASSERT_TRUE(editorSalt.found);

  Common::Models::ContainsUserRequest containsUserRequest = {expectedUser, userSalt.data};  
  auto receivedUser = db.executeRequest({Common::Functions::ContainsUser, Common::Models::toStr(containsUserRequest)});
  ASSERT_TRUE(receivedUser.found);

  Common::Models::ContainsUserRequest containsEditorRequest = {expectedUser, editorSalt.data};  
  auto receivedEditor = db.executeRequest({Common::Functions::ContainsUser, Common::Models::toStr(containsEditorRequest)});
  ASSERT_TRUE(receivedEditor.found);
}

TEST(User, change_password) {
  Common::Database db("test-blockchain.db");
  Common::Models::LoginRequest expectedUser = {"Anne-Sophie Provencher", "LOL1234!"};
  Common::Models::GetSaltRequest getSaltRequest = {.username = expectedUser.username,};
  auto userSalt = db.executeRequest({Common::Functions::GetSalt, Common::Models::toStr(getSaltRequest)});
  ASSERT_TRUE(userSalt.found);

  Common::Models::SetUserPasswordRequest passwordRequest = {.username = expectedUser.username, {expectedUser.password, "1234"}, userSalt.data, false, false};
  auto passwordResponse = db.executeRequest({Common::Functions::SetUserPassword, Common::Models::toStr(passwordRequest)});
  ASSERT_FALSE(passwordResponse.found);
}

TEST(User, get_role) {
  Common::Database db("test-blockchain.db");
  Common::Models::LoginRequest expectedUser = {"Anne-Sophie Provencher", "LOL1234!"};
  Common::Models::LoginRequest expectedEditor = {"Jolyne Rodrigue", "editrice"};

  Common::Models::GetSaltRequest getSaltRequest = {.username = expectedUser.username,};
  auto userSalt = db.executeRequest({Common::Functions::GetSalt, Common::Models::toStr(getSaltRequest)});
  ASSERT_TRUE(userSalt.found);

  getSaltRequest = {.username = expectedEditor.username,};
  auto editorSalt = db.executeRequest({Common::Functions::GetSalt, Common::Models::toStr(getSaltRequest)});
  ASSERT_TRUE(editorSalt.found);

  Common::Models::GetRoleRequest getRoleRequest = {expectedUser, userSalt.data};
  auto userRole = db.executeRequest({Common::Functions::GetRole, Common::Models::toStr(getRoleRequest)});
  //ASSERT_EQ(userRole.data.value(), "0");

  getRoleRequest = {expectedEditor, editorSalt.data};
  auto editorRole = db.executeRequest({Common::Functions::GetRole, Common::Models::toStr(getRoleRequest)});
  //ASSERT_EQ(editorRole.data.value(), "1");
}

TEST(User, complete_delete) {
  Common::Database db("test-blockchain.db");
  Common::Models::LoginRequest expectedUser = {"Anne-Sophie Provencher", "LOL1234!"};
  Common::Models::LoginRequest expectedEditor = {"Jolyne Rodrigue", "editrice"};

  Common::Models::DeleteAccountRequest deleteAccountRequest = {expectedUser.username};
  EXPECT_NO_THROW(db.executeRequest({Common::Functions::DeleteUser, Common::Models::toStr(deleteAccountRequest)}));

  deleteAccountRequest = {expectedEditor.username};
  EXPECT_NO_THROW(db.executeRequest({Common::Functions::DeleteUser, Common::Models::toStr(deleteAccountRequest)}));
}

//TODO
TEST(User, get_all) {
  Common::Database db("test-blockchain.db");
  auto response = db.executeRequest({Common::Functions::GetAllUsers, ""});
  Common::Models::AllUsersResponse allUsers = nlohmann::json::parse(response.data);
  Common::Models::UserResponse receivedEditor = allUsers.users.back();
  ASSERT_EQ(receivedEditor.username, "admin");
  ASSERT_TRUE(receivedEditor.isEditor);
  ASSERT_TRUE(receivedEditor.isAdmin);
}

TEST(Admin, get_salt_and_contains) {
  Common::Database db("test-blockchain.db");
  Common::Models::LoginRequest expectedUser = {"admin", "equipe01"};
  Common::Models::GetSaltRequest getSaltRequest = {.username = expectedUser.username,};
  auto userSalt = db.executeRequest({Common::Functions::GetSalt, Common::Models::toStr(getSaltRequest)});
  ASSERT_TRUE(userSalt.found);
  
  Common::Models::ContainsAdminRequest containsAdminRequest = {expectedUser, userSalt.data, true};  
  auto receivedAdmin = db.executeRequest({Common::Functions::ContainsAdmin, Common::Models::toStr(containsAdminRequest)});
  ASSERT_TRUE(receivedAdmin.found);
}

TEST(Admin, change_password) {
  Common::Database db("test-blockchain.db");
  Common::Models::LoginRequest expectedUser = {"admin", "equipe01"};
  Common::Models::GetSaltRequest getSaltRequest = {.username = expectedUser.username,};
  auto userSalt = db.executeRequest({Common::Functions::GetSalt, Common::Models::toStr(getSaltRequest)});
  ASSERT_TRUE(userSalt.found);

  Common::Models::SetUserPasswordRequest passwordRequest = {.username = expectedUser.username, {expectedUser.password, "1234"}, userSalt.data, false, false};
  auto passwordResponse = db.executeRequest({Common::Functions::SetUserPassword, Common::Models::toStr(passwordRequest)});
  ASSERT_FALSE(passwordResponse.found);
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

TEST(Server, set_last_block_id_and_get_last_block_id) {
  Common::Database db("test-blockchain.db");
  int expectedId = 1;
  ASSERT_EQ(db.getLastBlockId(), -1);
  db.setLastBlockId(expectedId);
  auto id = db.getLastBlockId();
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

  Common::Models::GetLogsRequest logsRequest = {0, expectedProvenance};
  auto response = db.executeRequest({Common::Functions::GetLogs, Common::Models::toStr(logsRequest)});
  auto receivedLast20Logs = nlohmann::json::parse(response.data);
  Common::Models::Information receivedLog = receivedLast20Logs.back();
  
  ASSERT_EQ(receivedLog.no, expectedLog.no);
  ASSERT_EQ(receivedLog.severity, expectedLog.severity);
  ASSERT_EQ(receivedLog.time, expectedLog.time);
  ASSERT_EQ(receivedLog.message, expectedLog.message);
}

TEST(Transaction, add_transaction) {  
  Common::Database db("test-blockchain.db");
  Common::Models::Result expectedResult1 = {"Michel", "Tremblay", "12345678", "69.00"};
  Common::Models::Result expectedResult2 = {"John", "Smith", "87654321", "42.00"};
  std::vector<Common::Models::Result> expectedResults;
  expectedResults.push_back(expectedResult1);
  expectedResults.push_back(expectedResult2);

  Common::Models::TransactionRequest transaction = {"inf3995", "Projet3", 20193, expectedResults};
  Common::Models::CheckForExistingClassRequest checkForExistingClass = {transaction.acronym, transaction.trimester};
  auto classIndex = db.executeRequest({Common::Functions::CheckForExistingClass, Common::Models::toStr(checkForExistingClass)});
  if (classIndex.found){
    db.executeRequest({Common::Functions::DeleteExistingClass, classIndex.data});
    db.executeRequest({Common::Functions::DeleteExistingResults, classIndex.data});
  }

  auto newClassId = db.executeRequest({Common::Functions::AddNewClass, Common::Models::toStr(transaction)});
  Common::Models::AddNewResultRequest newResult = {transaction, std::stoi(newClassId.data)};
  db.executeRequest({Common::Functions::AddNewResult, Common::Models::toStr(newResult)});

  classIndex = db.executeRequest({Common::Functions::CheckForExistingClass, Common::Models::toStr(checkForExistingClass)});
  std::vector<Common::Models::Result> receivedResults;
  if (classIndex.found){
    auto response = db.executeRequest({Common::Functions::GetClassResult,  classIndex.data});
    ASSERT_TRUE(response.found);
  }
}

TEST(Transaction, get_classes) {
  Common::Database db("test-blockchain.db");
  Common::Models::ClassInfo expectedClass = {"inf3995", "Projet3", 20193};
  auto response = db.executeRequest({Common::Functions::GetClasses, ""});
  auto classes = nlohmann::json::parse(response.data);
  Common::Models::ClassInfo receivedClass = classes.back();
  ASSERT_EQ(expectedClass.acronym, receivedClass.acronym);
  ASSERT_EQ(expectedClass.trimester, receivedClass.trimester);
  ASSERT_EQ(expectedClass.name, receivedClass.name);
}

TEST(Transaction, get_student) {
  Common::Database db("test-blockchain.db");
  Common::Models::StudentRequest expectedStudentResult = {"inf3995", "20193", "12345678"};
  auto response = db.executeRequest({Common::Functions::GetStudentResult,  Common::Models::toStr(expectedStudentResult)});
  std::vector<Common::Models::StudentResult> studentResults = nlohmann::json::parse(response.data);
  ASSERT_EQ(expectedStudentResult.acronym, studentResults.back().acronym);
  ASSERT_EQ(std::stoi(expectedStudentResult.trimester), studentResults.back().trimester);
  ASSERT_EQ("69.00", studentResults.back().grade);
}

TEST(Transaction, get_students) {
  Common::Database db("test-blockchain.db");
  Common::Models::Result expectedStudent1 = {"Tremblay", "Michel", "12345678"};
  Common::Models::Result expectedStudent2 = {"Smith", "John", "87654321"};
  auto response = db.executeRequest({Common::Functions::GetStudents, ""});
  std::vector<Common::Models::StudentInfo> students = nlohmann::json::parse(response.data);
  auto student = students.back();
  students.pop_back();
  ASSERT_EQ(expectedStudent2.firstName, student.firstName);
  ASSERT_EQ(expectedStudent2.lastName, student.lastName);
  ASSERT_EQ(expectedStudent2.id, student.id);

  student = students.back();
  ASSERT_EQ(expectedStudent1.firstName, student.firstName);
  ASSERT_EQ(expectedStudent1.lastName, student.lastName);
  ASSERT_EQ(expectedStudent1.id, student.id);
}

TEST(Transaction, delete_transaction) {
  Common::Database db("test-blockchain.db");
  Common::Models::TransactionRequest transaction = {"inf3995", "Projet3", 20193,};
  Common::Models::CheckForExistingClassRequest checkForExistingClass = {"inf3995", 20193};
  auto classIndex = db.executeRequest({Common::Functions::CheckForExistingClass, Common::Models::toStr(checkForExistingClass)});
  if (classIndex.found){
    db.executeRequest({Common::Functions::DeleteExistingClass, classIndex.data});
    db.executeRequest({Common::Functions::DeleteExistingResults, classIndex.data});
  }
}
