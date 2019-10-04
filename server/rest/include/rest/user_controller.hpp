#ifndef REST_USER_CONTROLLER_HPP
#define REST_USER_CONTROLLER_HPP

#include <nlohmann/json.hpp>
#include <pistache/endpoint.h>
#include <pistache/http.h>
#include <pistache/router.h>

#include <common/example.hpp>

using json = nlohmann::json;

class UserController {
 public:
  explicit UserController(const std::shared_ptr<Pistache::Rest::Router>& router);

 private:
  void setupRoutes(const std::shared_ptr<Pistache::Rest::Router>& router);
  void handleLol(const Pistache::Rest::Request& /*unused*/, Pistache::Http::ResponseWriter response);
};

#endif // REST_USER_CONTROLLER_HPP
