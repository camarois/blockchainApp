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
  explicit TokenManager(const Pistache::Rest::Request& request);

  jwt::jwt_object getToken() const;
  std::string getSignature() const;

  bool decode();
  void refresh();

 private:
  void encode(const std::string& username, const std::string& passwordt);

  jwt::jwt_object token_;
  std::string signature_;
  std::error_code errCode_;

  const uint64_t kExpirationTimeMax_ = 3600;
  const jwt::string_view kUsername_ = "username";
  const jwt::string_view kPassword_ = "password";
  const jwt::string_view kExpiration_ = "expiration";
  const std::string kAuthorization_ = "Authorization";
};

}  // namespace Rest

#endif  // REST_TOKEN_MANAGER_HPP
