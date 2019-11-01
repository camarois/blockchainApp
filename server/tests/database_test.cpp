#include <common/database.hpp>
#include <common/models.hpp>
#include <gtest/gtest.h>

TEST(Sqlite3Tests, get_user) {
  Common::Database db("test-blockchain.db");
  Common::Models::LoginRequest expectedUser = {"Anne-Sophie Provencher", "LOL1234!"};
  std::string expectedHash = "da6a850377faa387cea7c58a6ebd5935d5502a95aa0993848f8ae4ab8efc68ad";

  db.addUser(expectedUser);

  auto salt = db.getSalt(expectedUser.username);
  ASSERT_TRUE(salt.has_value());
  auto receivedUser = db.containsUser(expectedUser, salt.value());
  ASSERT_TRUE(receivedUser);
}

TEST(Sqlite3Tests, test_transaction) {  
  Common::Database db("test-blockchain.db");
  Common::Models::Result expectedResult1 = {"Tremblay", "Michel", "12345678", "69.00"};
  Common::Models::Result expectedResult2 = {"Smith", "John", "87654321", "42.00"};
  std::vector<Common::Models::Result> expectedResults;
  expectedResults.push_back(expectedResult1);
  expectedResults.push_back(expectedResult2);

  Common::Models::TransactionRequest transaction = {"inf3995", "Projet3", 20193, expectedResults};
  std::optional<int> classId = db.checkForExistingClass(transaction.acronym, transaction.trimester);
  if (classId){
    db.DeleteExistingClass(classId.value());
    db.DeleteExistingResults(classId.value());
  }
  int newClassId = db.AddNewClass(transaction);
  db.AddNewResult(transaction, newClassId);

  Common::Models::ClassesRequest classesRequest = {"inf3995", 20193};
  classId = db.checkForExistingClass(classesRequest.acronym, classesRequest.trimester);
  std::vector<Common::Models::Result> receivedResults;
  if (classId){
    receivedResults = db.getClassResult(classId.value());
  }

  ASSERT_TRUE(!receivedResults.empty());
  ASSERT_EQ(expectedResults.size(), receivedResults.size());

  Common::Models::StudentRequest studentRequest = {"inf3995", 20193, "12345678"};
  classId = db.checkForExistingClass(studentRequest.acronym, studentRequest.trimester);
  std::optional<Common::Models::Result> receivedResult;
  if (classId){
    receivedResult = db.getStudentResult(classId.value(), studentRequest.id);
  }
  ASSERT_EQ(expectedResult1.grade, receivedResult.value().grade);

}
