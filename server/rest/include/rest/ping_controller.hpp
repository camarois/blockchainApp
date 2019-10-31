#ifndef REST_PING_CONTROLLER_HPP
#define REST_PING_CONTROLLER_HPP

#include <nlohmann/json.hpp>
#include <pistache/endpoint.h>
#include <pistache/http.h>
#include <pistache/router.h>
#include <rest/custom_router.hpp>

namespace Rest {

class PingController {
 public:
  explicit PingController(const std::shared_ptr<Rest::CustomRouter>& router);

 private:
  void setupRoutes(const std::shared_ptr<Rest::CustomRouter>& router);
  void handlePing(const Pistache::Rest::Request& /*unused*/, Pistache::Http::ResponseWriter response);
};

}  // namespace Rest

#endif  // REST_PING_CONTROLLER_HPP
