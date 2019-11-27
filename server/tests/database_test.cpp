#include <common/database.hpp>
#include <common/models.hpp>
#include <gtest/gtest.h>

TEST(Sqlite3Tests, get_salt) {
  Common::Database db("test-blockchain.db");

  Common::Models::LoginRequest expectedUser = {"Anne-Sophie Provencher", "LOL1234!"};
  Common::Models::AddUserRequest addUserRequest = {
      .loginRequest = expectedUser,
      .isAdmin = false,
      .isEditor = false,
  };
  db.addUser(addUserRequest);

  Common::Models::GetSaltRequest getSaltRequest = {
      .username = expectedUser.username,
  };

  auto salt = db.getSalt(getSaltRequest);
  ASSERT_TRUE(salt.has_value());
}

// TEST(Sqlite3Tests, contains_user) {
//   Common::Database db("test-blockchain.db");

//   Common::Models::LoginRequest expectedUser = {"Anne-Sophie Provencher", "LOL1234!"};
//   Common::Models::AddUserRequest addUserRequest = {
//       .loginRequest = expectedUser,
//       .isAdmin = false,
//       .isEditor = false,
//   };
//   db.addUser(addUserRequest);

//   Common::Models::ContainsUserRequest containsUserRequest = {
//   .loginRequest = expectedUser,
//   .salt =
// };
//   auto receivedUser = db.containsUser(expectedUser, salt.value());
//   ASSERT_TRUE(receivedUser);
// }

// TEST(Sqlite3Tests, test_transaction) {
//   Common::Database db("test-blockchain.db");
//   Common::Models::Result expectedResult1 = {"Tremblay", "Michel", "12345678", "69.00"};
//   Common::Models::Result expectedResult2 = {"Smith", "John", "87654321", "42.00"};
//   std::vector<Common::Models::Result> expectedResults;
//   expectedResults.push_back(expectedResult1);
//   expectedResults.push_back(expectedResult2);

//   Common::Models::TransactionRequest transaction = {"inf3995", "Projet3", 20193, expectedResults};
//   std::optional<int> classId = db.checkForExistingClass(transaction.acronym, transaction.trimester);
//   if (classId){
//     db.deleteExistingClass(classId.value());
//     db.deleteExistingResults(classId.value());
//   }
//   int newClassId = db.addNewClass(transaction);
//   db.addNewResult(transaction, newClassId);

//   Common::Models::ClassesRequest classesRequest = {"inf3995", 20193};
//   classId = db.checkForExistingClass(classesRequest.acronym, classesRequest.trimester);
//   std::vector<Common::Models::Result> receivedResults;
//   if (classId){
//     receivedResults = db.getClassResult(classId.value());
//   }

//   ASSERT_TRUE(!receivedResults.empty());
//   ASSERT_EQ(expectedResults.size(), receivedResults.size());

//   Common::Models::StudentRequest studentRequest = {"inf3995", "20193", "12345678"};
//   std::vector<Common::Models::StudentResult> receivedResult  = db.getStudentResult(studentRequest);
//   ASSERT_EQ(expectedResult1.grade, receivedResult[0].grade);

// }
