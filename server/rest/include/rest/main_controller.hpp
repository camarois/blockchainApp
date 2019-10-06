#ifndef REST_MAIN_CONTROLLER_HPP
#define REST_MAIN_CONTROLLER_HPP

#include <nlohmann/json.hpp>
#include <pistache/endpoint.h>
#include <pistache/http.h>
#include <pistache/router.h>
#include <zmq.hpp>
#include <rest/user_controller.hpp>
#include <rest/example_controller.hpp>
#include <common/example.hpp>

using json = nlohmann::json;

namespace Rest {

class MainController {
 public:
  explicit MainController(Pistache::Address addr, size_t thr);

  void start();

 private:
  Pistache::Http::Endpoint httpEndpoint_;
  std::shared_ptr<Pistache::Rest::Router> router_;
  Rest::UserController userController_;
  Rest::ExampleController exampleController_;
};

} // namespace Rest

#endif // REST_MAIN_CONTROLLER_HPP
