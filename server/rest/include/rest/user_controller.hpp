#ifndef REST_USER_CONTROLLER_HPP
#define REST_USER_CONTROLLER_HPP

#include <nlohmann/json.hpp>
#include <pistache/endpoint.h>
#include <pistache/http.h>
#include <pistache/router.h>
#include <rest/custom_router.hpp>
#include <rest/zmq.hpp>

namespace Rest {

class UserController {
 public:
  explicit UserController(const std::shared_ptr<Rest::CustomRouter>& router, std::shared_ptr<ZMQWorker> zmqWorker);

 private:
  void setupRoutes(const std::shared_ptr<Rest::CustomRouter>& router);
  void handleLogin(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response);
  void handleLogout(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response);
  void handlePassword(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response);
  void handleRegister(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response);

  std::shared_ptr<ZMQWorker> zmqWorker_;
  const std::string kBasePath_ = "/usager/";
};

}  // namespace Rest

#endif  // REST_USER_CONTROLLER_HPP
