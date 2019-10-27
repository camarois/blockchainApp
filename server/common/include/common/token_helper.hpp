#ifndef COMMON_TOKEN_HELPER_HPP
#define COMMON_TOKEN_HELPER_HPP

#include <arpa/inet.h>
#include <cstring>
#include <curlpp/Options.hpp>
#include <future>
#include <ifaddrs.h>
#include <sstream>
#include <string>

namespace Common {
namespace TokenHelper {
const uint64_t kExpirationTimeMax = 3600;
const jwt::string_view kUsername = "username";
const jwt::string_view kPassword = "password";
const jwt::string_view kExpiration = "expiration";
const std::string kAuthorization = "Authorization";

// Inspired by https://github.com/arun11299/cpp-jwt
inline jwt::jwt_object encode(const std::string& username, const std::string& password) {
  jwt::jwt_object token{jwt::params::algorithm("hs256"), jwt::params::secret("inf3995"),
                         jwt::params::payload({{"role", "student"}})};
  token.add_claim("username", username)
      .add_claim("password", password)
      .add_claim("exp", std::chrono::system_clock::now() + std::chrono::seconds{kExpirationTimeMax});
  //signature_ = token.signature(errCode_);
  return token
}

inline jwt_object refresh(jwt::jwt_object token) {
  token.remove_claim("exp");
  token.add_claim("exp", std::chrono::system_clock::now() + std::chrono::seconds{kExpirationTimeMax});
  signature_ = token.signature(errCode);
}

inline std::error_code decode(jwt::jwt_object token) {
  try {
    jwt::jwt_object decodedObj =
        jwt::decode(getSignature(), jwt::params::algorithms({"hs256"}), errCode, jwt::params::secret("inf3995"));

    if (errCode.value() == static_cast<int>(jwt::VerificationErrc::TokenExpired)) {
      std::string username = decodedObj.payload().get_claim_value<std::string>(kUsername);
      std::string password = decodedObj.payload().get_claim_value<std::string>(kPassword);

      Common::Database db = Common::Database(FLAGS_database);
      auto user = db.getUser(username);

      if (user) {
        this->refresh();
      }
    }
  } catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
  }
  return errCode;
}

}  // namespace TokenHelper
}  // namespace Common

#endif  // COMMON_TOKEN_HELPER_HPP
