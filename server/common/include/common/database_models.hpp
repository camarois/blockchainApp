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
const std::string kUsername = "username";
const std::string kPasswordRequest = "passwordRequest";
const std::string kSalt = "salt";

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

}  // namespace Models
}  // namespace Common

#endif  // DATABASE_MODELS_HPP
