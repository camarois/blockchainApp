#ifndef REST_USER_CONTROLLER_HPP
#define REST_USER_CONTROLLER_HPP

#include <nlohmann/json.hpp>
#include <pistache/endpoint.h>
#include <pistache/http.h>
#include <pistache/router.h>

#include <common/example.hpp>

namespace Rest {

class UserController {
 public:
  explicit UserController(const std::shared_ptr<Pistache::Rest::Router>& router);

 private:
  void setupRoutes(const std::shared_ptr<Pistache::Rest::Router>& router);
  void handleLogin(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response);
  void handleLogout(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response);
  void handlePassword(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response);

  const std::string kBasePath = "/usager/";
};

}  // namespace Rest

#endif  // REST_USER_CONTROLLER_HPP
