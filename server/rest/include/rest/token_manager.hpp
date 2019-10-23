#ifndef REST_TOKEN_MANAGER_HPP
#define REST_TOKEN_MANAGER_HPP

#include <common/database.hpp>
#include <common/example.hpp>
#include <chrono>
#include <cassert>
#include <iostream>
#include <jwt/jwt.hpp>
#include <pistache/router.h>

namespace Rest {

class TokenManager {
    public:
    explicit TokenManager(const Pistache::Rest::Request& request);
    TokenManager(const std::string signature);

    jwt::jwt_object token() const;
    std::string signature() const;
    void decode(std::string signature) const;

    private:
    void encode(const Pistache::Rest::Request& request);

    jwt::jwt_object token_{jwt::params::algorithm("hs256"), jwt::params::secret("inf3995"), jwt::params::payload({{"role", "student"}})};
    std::string signature_;
    const int kExpiration_ = 3600;
};

}  // namespace Rest

#endif  // REST_TOKEN_MANAGER_HPP
