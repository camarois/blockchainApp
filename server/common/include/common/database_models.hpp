#ifndef DATABASE_MODELS_HPP
#define DATABASE_MODELS_HPP

#include <common/models.hpp>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

namespace Common {
namespace Models {

const std::string kLoginRequest = "loginRequest";
const std::string kIsAdmin = "isAdmin";
const std::string kPasswordRequest = "passwordRequest";
const std::string kSalt = "salt";
const std::string kTransactionRequest = "transactionRequest";
const std::string kClassId = "classId";

struct AddUserRequest {
  Common::Models::LoginRequest loginRequest;
  bool isAdmin;
};

// NOLINTNEXTLINE(readability-identifier-naming, google-runtime-references)
inline void to_json(nlohmann::json& j, const AddUserRequest& obj) {
  j = {{kLoginRequest, obj.loginRequest}, {kIsAdmin, obj.isAdmin}};
}

// NOLINTNEXTLINE(readability-identifier-naming, google-runtime-references)
inline void from_json(const nlohmann::json& j, AddUserRequest& obj) {
  j.at(kLoginRequest).get_to(obj.loginRequest);
  j.at(kIsAdmin).get_to(obj.isAdmin);
}

struct SetUserPasswordRequest {
  std::string username;
  Common::Models::PasswordRequest passwordRequest;
  std::string salt;
  bool isAdmin;
};

// NOLINTNEXTLINE(readability-identifier-naming, google-runtime-references)
inline void to_json(nlohmann::json& j, const SetUserPasswordRequest& obj) {
  j = {{kUsername, obj.username}, {kPasswordRequest, obj.passwordRequest}, {kSalt, obj.salt}, {kIsAdmin, obj.isAdmin}};
}

// NOLINTNEXTLINE(readability-identifier-naming, google-runtime-references)
inline void from_json(const nlohmann::json& j, SetUserPasswordRequest& obj) {
  j.at(kUsername).get_to(obj.username);
  j.at(kPasswordRequest).get_to(obj.passwordRequest);
  j.at(kSalt).get_to(obj.salt);
  j.at(kIsAdmin).get_to(obj.isAdmin);
}

struct ContainsUserRequest {
  Common::Models::LoginRequest loginRequest;
  std::string salt;
};

// NOLINTNEXTLINE(readability-identifier-naming, google-runtime-references)
inline void to_json(nlohmann::json& j, const ContainsUserRequest& obj) {
  j = {{kLoginRequest, obj.loginRequest}, {kSalt, obj.salt}};
}

// NOLINTNEXTLINE(readability-identifier-naming, google-runtime-references)
inline void from_json(const nlohmann::json& j, ContainsUserRequest& obj) {
  j.at(kLoginRequest).get_to(obj.loginRequest);
  j.at(kSalt).get_to(obj.salt);
}

struct CheckForExistingClassRequest {
  std::string acronym;
  int trimester;
};

// NOLINTNEXTLINE(readability-identifier-naming, google-runtime-references)
inline void to_json(nlohmann::json& j, const CheckForExistingClassRequest& obj) {
  j = {{kAcronym, obj.acronym}, {kTrimester, obj.trimester}};
}

// NOLINTNEXTLINE(readability-identifier-naming, google-runtime-references)
inline void from_json(const nlohmann::json& j, CheckForExistingClassRequest& obj) {
  j.at(kAcronym).get_to(obj.acronym);
  j.at(kTrimester).get_to(obj.trimester);
}

struct AddNewResultRequest {
  Common::Models::TransactionRequest transactionRequest;
  int classId;
};

// NOLINTNEXTLINE(readability-identifier-naming, google-runtime-references)
inline void to_json(nlohmann::json& j, const AddNewResultRequest& obj) {
  j = {{kTransactionRequest, obj.transactionRequest}, {kClassId, obj.classId}};
}

// NOLINTNEXTLINE(readability-identifier-naming, google-runtime-references)
inline void from_json(const nlohmann::json& j, AddNewResultRequest& obj) {
  j.at(kTransactionRequest).get_to(obj.transactionRequest);
  j.at(kClassId).get_to(obj.classId);
}

struct ContainsAdminRequest {
  Common::Models::LoginRequest loginRequest;
  std::string salt;
  bool isAdmin;
};

// NOLINTNEXTLINE(readability-identifier-naming, google-runtime-references)
inline void to_json(nlohmann::json& j, const ContainsAdminRequest& obj) {
  j = {{kLoginRequest, obj.loginRequest}, {kSalt, obj.salt}, {kIsAdmin, obj.isAdmin}};
}

// NOLINTNEXTLINE(readability-identifier-naming, google-runtime-references)
inline void from_json(const nlohmann::json& j, ContainsAdminRequest& obj) {
  j.at(kLoginRequest).get_to(obj.loginRequest);
  j.at(kSalt).get_to(obj.salt);
  j.at(kIsAdmin).get_to(obj.isAdmin);
}

struct GetRoleRequest {
  Common::Models::LoginRequest loginRequest;
  std::string salt;
};

// NOLINTNEXTLINE(readability-identifier-naming, google-runtime-references)
inline void to_json(nlohmann::json& j, const GetRoleRequest& obj) {
  j = {{kLoginRequest, obj.loginRequest}, {kSalt, obj.salt}};
}

// NOLINTNEXTLINE(readability-identifier-naming, google-runtime-references)
inline void from_json(const nlohmann::json& j, GetRoleRequest& obj) {
  j.at(kLoginRequest).get_to(obj.loginRequest);
  j.at(kSalt).get_to(obj.salt);
}

}  // namespace Models
}  // namespace Common

#endif  // DATABASE_MODELS_HPP
