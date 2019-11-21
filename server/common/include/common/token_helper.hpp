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
// TODO(frgraf): find a better solution
const std::string kSecret = "Authorizationinf3995authorization_token";

// Inspired by https://github.com/arun11299/cpp-jwt
inline std::string encode(const std::string& username, const std::string& password) {
  jwt::jwt_object token{jwt::params::algorithm(kAlgorithm), jwt::params::secret(kSecret),
                        jwt::params::payload({{"role", "student"}})};
  token.add_claim(std::string(kUsername), username)
      .add_claim(std::string(kPassword), password)
      .add_claim(jwt::registered_claims::expiration,
                 std::chrono::system_clock::now() + std::chrono::seconds{kExpirationDelaySeconds});
  return token.signature();
}

inline std::optional<std::string> decode(const std::string& token) {
  std::error_code errCode;
  auto decodedObj = jwt::decode(token, jwt::params::algorithms({kAlgorithm}), errCode, jwt::params::secret(kSecret));
  if (!decodedObj.payload().has_claim(kUsername) || !decodedObj.payload().has_claim(kPassword)) {
    return {};
  }
  
  return token;
}

inline bool timedOut(const std::string& token) {
  std::error_code errCode;
  jwt::decode(token, jwt::params::algorithms({kAlgorithm}), errCode, jwt::params::secret(kSecret));

  return errCode.value() == static_cast<int>(jwt::VerificationErrc::TokenExpired);
}

inline std::optional<std::string> decodeUsername(const std::string& token) {
  std::error_code errCode;
  auto decodedObj = jwt::decode(token, jwt::params::algorithms({kAlgorithm}), errCode, jwt::params::secret(kSecret));
  if (!decodedObj.payload().has_claim(kUsername)) {
    return {};
  }
  return decodedObj.payload().get_claim_value<std::string>(kUsername);
}

inline std::optional<std::string> decodePassword(const std::string& token) {
  std::error_code errCode;
  auto decodedObj = jwt::decode(token, jwt::params::algorithms({kAlgorithm}), errCode, jwt::params::secret(kSecret));
  if (!decodedObj.payload().has_claim(kPassword)) {
    return {};
  }
  return decodedObj.payload().get_claim_value<std::string>(kPassword);
}

}  // namespace TokenHelper
}  // namespace Common

#endif  // COMMON_TOKEN_HELPER_HPP
