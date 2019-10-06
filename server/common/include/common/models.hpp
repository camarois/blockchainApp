#ifndef COMMON_MODELS_HPP
#define COMMON_MODELS_HPP

#include <string>
#include <nlohmann/json.hpp>

namespace Common {
namespace Models {

const std::string kUsername = "usager";
const std::string kPassword = "mot_de_passe";
const std::string kEdtion = "edition";
const std::string kOld = "ancien";
const std::string kNew = "nouveau";

template <typename T>
inline T from_str(const std::string& str) {
    return nlohmann::json::parse(str);
}

template <typename T>
inline std::string to_str(const T& obj) {
    return ((nlohmann::json)obj).dump(4);
}

struct LoginRequest {
  std::string username;
  std::string password;
};

inline void to_json(nlohmann::json& j, const LoginRequest& obj) {
  j = nlohmann::json{{kUsername, obj.username}, {kPassword, obj.password}};
}

inline void from_json(const nlohmann::json& j, LoginRequest& obj) {
  j.at(kUsername).get_to(obj.username);
  j.at(kPassword).get_to(obj.password);
}

struct LoginResponse {
  bool edition;
};

inline void to_json(nlohmann::json& j, const LoginResponse& obj) {
  j = nlohmann::json{{kEdtion, obj.edition}};
}

inline void from_json(const nlohmann::json& j, LoginResponse& obj) {
  j.at(kEdtion).get_to(obj.edition);
}

struct PasswordRequest {
  std::string oldPwd;
  std::string newPwd;
};

inline void to_json(nlohmann::json& j, const PasswordRequest& obj) {
  j = nlohmann::json{{kOld, obj.oldPwd}, {kNew, obj.newPwd}};
}

inline void from_json(const nlohmann::json& j, PasswordRequest& obj) {
  j.at(kOld).get_to(obj.oldPwd);
  j.at(kNew).get_to(obj.newPwd);
}

}  // namespace Models
}  // namespace Common

#endif  // COMMON_MODELS_HPP
