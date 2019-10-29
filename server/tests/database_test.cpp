#include <common/database.hpp>
#include <common/models.hpp>
#include <filesystem>
#include <gtest/gtest.h>

TEST(Sqlite3Tests, get_user) {
  Common::Database db("test-blockchain.db");
  Common::Models::LoginRequest expectedUser = {"Anne-Sophie Provencher", "LOL1234!"};

  db.addUser(expectedUser);

  auto receivedUser = db.getUser(expectedUser.username);

  ASSERT_TRUE(receivedUser.has_value());
  ASSERT_EQ(expectedUser.username, receivedUser->username);
  ASSERT_EQ(expectedUser.password, receivedUser->password);
}

TEST(Sqlite3Tests, get_transaction) {  
  Common::Database db("test-blockchain.db");
  Common::Models::Result expectedResult1 = {"Tremblay", "Michel", "12345678", "69.00"};
  Common::Models::Result expectedResult2 = {"Smith", "John", "87654321", "42.00"};
  std::vector<Common::Models::Result> results;
  results.push_back(expectedResult1);
  results.push_back(expectedResult2);

  Common::Models::TransactionRequest transaction = {"inf3995", "Projet3", 20193, results};
  db.addTransactionRequest(transaction);

  Common::Models::ClassesRequest classesRequest = {"inf3995", 20193};
  auto receivedClass = db.getClassesRequest(classesRequest);
  ASSERT_TRUE(receivedClass.has_value());
  ASSERT_EQ(transaction.name, receivedClass->name);
  ASSERT_EQ(results.size(), receivedClass->results.size());

  Common::Models::StudentRequest studentRequest = {"inf3995", 20193, "12345678"};
  auto receivedStudent = db.getStudentRequest(studentRequest);
  ASSERT_TRUE(receivedStudent.has_value());
  ASSERT_EQ(expectedResult1.grade, receivedStudent->grade);

}
