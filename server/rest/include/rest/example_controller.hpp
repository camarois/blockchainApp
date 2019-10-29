#ifndef REST_EXAMPLE_CONTROLLER_HPP
#define REST_EXAMPLE_CONTROLLER_HPP

#include <nlohmann/json.hpp>
#include <pistache/endpoint.h>
#include <pistache/http.h>
#include <pistache/router.h>
#include <rest/custom_router.hpp>
#include <zmq.hpp>

namespace Rest {

class ExampleController {
 public:
  explicit ExampleController(const std::shared_ptr<Rest::CustomRouter>& router);

 private:
  void setupRoutes(const std::shared_ptr<Rest::CustomRouter>& router);
  void handlePing(const Pistache::Rest::Request& /*unused*/, Pistache::Http::ResponseWriter response);
  void handleStart(const Pistache::Rest::Request& /*unused*/, Pistache::Http::ResponseWriter response);

  zmq::context_t context_;
  zmq::socket_t socket_;
};

}  // namespace Rest

#endif  // REST_EXAMPLE_CONTROLLER_HPP
