#ifndef COMMON_DATABASE_MODELS_HPP
#define COMMON_DATABASE_MODELS_HPP

#include <common/models.hpp>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

namespace Common {
namespace Models {

const std::string kLoginRequest = "loginRequest";
const std::string kIsAdmin = "estAdmin";
const std::string kIsEditor = "estEditeur";
const std::string kPasswordRequest = "passwordRequest";
const std::string kSalt = "salt";
const std::string kTransactionRequest = "transactionRequest";
const std::string kClassId = "classId";
const std::string kUsers = "utilisateurs";

struct UserResponse {
  std::string username;
  bool isEditor = false;
  bool isAdmin = false;
};

// NOLINTNEXTLINE(readability-identifier-naming, google-runtime-references)
inline void to_json(nlohmann::json& j, const UserResponse& obj) {
  j = {{kUsername, obj.username}, {kIsEditor, obj.isEditor}, {kIsAdmin, obj.isAdmin}};
}

// NOLINTNEXTLINE(readability-identifier-naming, google-runtime-references)
inline void from_json(const nlohmann::json& j, UserResponse& obj) {
  j.at(kUsername).get_to(obj.username);
  j.at(kIsAdmin).get_to(obj.isAdmin);
  j.at(kIsEditor).get_to(obj.isEditor);
}

struct AllUsersResponse {
  std::vector<UserResponse> users;
};

// NOLINTNEXTLINE(readability-identifier-naming, google-runtime-references)
inline void to_json(nlohmann::json& j, const AllUsersResponse& obj) { j = {{kUsers, obj.users}}; }

// NOLINTNEXTLINE(readability-identifier-naming, google-runtime-references)
inline void from_json(const nlohmann::json& j, AllUsersResponse& obj) { j.at(kUsers).get_to(obj.users); }


struct AddUserRequest {
  Common::Models::LoginRequest loginRequest;
  bool isAdmin = false;
  bool isEditor = false;
};

// NOLINTNEXTLINE(readability-identifier-naming, google-runtime-references)
inline void to_json(nlohmann::json& j, const AddUserRequest& obj) {
  j = {{kLoginRequest, obj.loginRequest}, {kIsAdmin, obj.isAdmin}, {kIsEditor, obj.isEditor}};
}

// NOLINTNEXTLINE(readability-identifier-naming, google-runtime-references)
inline void from_json(const nlohmann::json& j, AddUserRequest& obj) {
  j.at(kLoginRequest).get_to(obj.loginRequest);
  j.at(kIsAdmin).get_to(obj.isAdmin);
  j.at(kIsEditor).get_to(obj.isEditor);
}

struct SetUserPasswordRequest {
  std::string username;
  Common::Models::PasswordRequest passwordRequest;
  std::string salt;
  bool isEditor = false;
  bool isAdmin = false;
};

// NOLINTNEXTLINE(readability-identifier-naming, google-runtime-references)
inline void to_json(nlohmann::json& j, const SetUserPasswordRequest& obj) {
  j = {{kUsername, obj.username}, {kPasswordRequest, obj.passwordRequest}, {kSalt, obj.salt}, {kIsEditor, obj.isEditor}, {kIsAdmin, obj.isAdmin}};
}

// NOLINTNEXTLINE(readability-identifier-naming, google-runtime-references)
inline void from_json(const nlohmann::json& j, SetUserPasswordRequest& obj) {
  j.at(kUsername).get_to(obj.username);
  j.at(kPasswordRequest).get_to(obj.passwordRequest);
  j.at(kSalt).get_to(obj.salt);
  j.at(kIsEditor).get_to(obj.isEditor);
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
  int trimester = 0;
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
  int classId = 0;
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
  bool isAdmin = false;
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

struct GetSaltRequest {
  std::string username;
};

// NOLINTNEXTLINE(readability-identifier-naming, google-runtime-references)
inline void to_json(nlohmann::json& j, const GetSaltRequest& obj) {
  j = {{kUsername, obj.username}};
}

// NOLINTNEXTLINE(readability-identifier-naming, google-runtime-references)
inline void from_json(const nlohmann::json& j, GetSaltRequest& obj) {
  j.at(kUsername).get_to(obj.username);
}

}  // namespace Models
}  // namespace Common

#endif  // COMMON_DATABASE_MODELS_HPP
