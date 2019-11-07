#ifndef REST_ADMIN_CONTROLLER_HPP
#define REST_ADMIN_CONTROLLER_HPP

#include <nlohmann/json.hpp>
#include <pistache/endpoint.h>
#include <pistache/http.h>
#include <pistache/router.h>
#include <rest/custom_router.hpp>
#include <rest/zmq.hpp>

namespace Rest {

class AdminController {
 public:
  explicit AdminController(const std::shared_ptr<Rest::CustomRouter>& router,
                           const std::shared_ptr<ZMQWorker>& zmqWorker);

 private:
  void setupRoutes(const std::shared_ptr<Rest::CustomRouter>& router);
  void handleLogin(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response);
  void handleLogout(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response);
  void handlePassword(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response);
  void handleChain(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response);
  void handleLogs(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response);
  void handleCreateAccount(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response);
  void handleDeleteAccount(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response);

  std::shared_ptr<ZMQWorker> zmqWorker_;
  const std::string kBasePath_ = "/admin/";
  const std::string kId_ = ":id";
};

}  // namespace Rest

#endif  // REST_ADMIN_CONTROLLER_HPP
