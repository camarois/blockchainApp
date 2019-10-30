#ifndef COMMON_TOKEN_HELPER_HPP
#define COMMON_TOKEN_HELPER_HPP

#include <cassert>
#include <chrono>
#include <common/database.hpp>
#include <gflags/gflags.h>
#include <iostream>
#include <jwt/jwt.hpp>
#include <memory>

namespace Common {
namespace TokenHelper {

const uint64_t kExpirationDelaySeconds = 3600;
const jwt::string_view kUsername = "username";
const jwt::string_view kPassword = "password";
const jwt::string_view kExpiration = "expiration";
const std::string kAlgorithm = "hs256";
const std::string kAuthorization = "Authorization";
const std::string kSecret = "Authorizationinf3995authorization_token";

// Inspired by https://github.com/arun11299/cpp-jwt
inline auto encode(const std::string& username, const std::string& password) {
  jwt::jwt_object token{jwt::params::algorithm(kAlgorithm), jwt::params::secret(kSecret),
                        jwt::params::payload({{"role", "student"}})};
  token.add_claim(std::string(kUsername), username)
      .add_claim(std::string(kPassword), password)
      .add_claim("exp", std::chrono::system_clock::now() + std::chrono::seconds{kExpirationDelaySeconds});
  return token;
}

inline std::optional<std::string> decode(const std::string* token) {
  std::error_code errCode;
  try {
    auto decodedObj = jwt::decode(*token, jwt::params::algorithms({kAlgorithm}), errCode, jwt::params::secret(kSecret));
    std::string username = decodedObj.payload().get_claim_value<std::string>(kUsername);
    std::string password = decodedObj.payload().get_claim_value<std::string>(kPassword);

    Common::Database db;
    auto user = db.getUser(username);
    if (errCode.value() == static_cast<int>(jwt::VerificationErrc::TokenExpired) && user) {
      jwt::jwt_object refreshToken = encode(user->username, user->password);
      return std::string(refreshToken.signature());
    }
    return *token;
  } catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
  }
  return {};
}

}  // namespace TokenHelper
}  // namespace Common

#endif  // COMMON_TOKEN_HELPER_HPP
