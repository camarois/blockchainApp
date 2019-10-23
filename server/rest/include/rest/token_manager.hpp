#ifndef REST_TOKEN_MANAGER_HPP
#define REST_TOKEN_MANAGER_HPP

#include <cassert>
#include <chrono>
#include <common/example.hpp>
#include <iostream>
#include <jwt/jwt.hpp>
#include <pistache/router.h>

namespace Rest {

class TokenManager {
 public:
  explicit TokenManager(const std::string& username, const std::string& password);
  explicit TokenManager(const std::string& signature);

  jwt::jwt_object getToken() const;
  std::string getSignature() const;

  void decode(const std::string& signature);
  void refresh();

 private:
  void encode(const std::string& username, const std::string& passwordt);

  jwt::jwt_object token_{jwt::params::algorithm("hs256"), jwt::params::secret("inf3995"),
                         jwt::params::payload({{"role", "student"}})};
  std::string signature_;
  std::error_code errCode_;

  const uint64_t kExpirationTimeMax_ = 3600;
  const jwt::string_view kUsername_ = "username";
  const jwt::string_view kPassword_ = "password";
  const jwt::string_view kExpiration_ = "expiration";
};

}  // namespace Rest

#endif  // REST_TOKEN_MANAGER_HPP
