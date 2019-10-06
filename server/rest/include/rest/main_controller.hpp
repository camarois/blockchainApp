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

namespace rest {

class MainController {
 public:
  explicit MainController(Pistache::Address addr, size_t thr);

  void start();

 private:
  Pistache::Http::Endpoint httpEndpoint_;
  std::shared_ptr<Pistache::Rest::Router> router_;
  rest::UserController userController_;
  rest::ExampleController exampleController_;
};

} // namespace rest

#endif // REST_MAIN_CONTROLLER_HPP
