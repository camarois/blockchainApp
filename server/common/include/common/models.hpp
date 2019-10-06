#ifndef COMMON_MODELS_HPP
#define COMMON_MODELS_HPP

#include <string>
#include <nlohmann/json.hpp>

namespace Common {
namespace Models {

const std::string kUsernameStr = "usager";
const std::string kPasswordStr = "mot_de_passe";

template <typename T>
inline T from_str(const std::string& str) {
    return nlohmann::json::parse(str);
}

template <typename T>
inline std::string to_str(const T& obj) {
    return ((nlohmann::json)obj).dump();
}

struct LoginRequest {
  std::string username;
  std::string password;
};

inline void to_json(nlohmann::json& j, const LoginRequest& obj) {
  j = nlohmann::json{{kUsernameStr, obj.username}, {kPasswordStr, obj.password}};
}

inline void from_json(const nlohmann::json& j, LoginRequest& obj) {
  j.at(kUsernameStr).get_to(obj.username);
  j.at(kPasswordStr).get_to(obj.password);
}

}  // namespace Models
}  // namespace Common

#endif  // COMMON_MODELS_HPP
