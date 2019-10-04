#ifndef REST_EXAMPLE_ENDPOINT_HPP
#define REST_EXAMPLE_ENDPOINT_HPP

#include <nlohmann/json.hpp>
#include <pistache/endpoint.h>
#include <pistache/http.h>
#include <pistache/router.h>
#include <zmq.hpp>

#include <common/example.hpp>

using json = nlohmann::json;

class ExampleEndpoint {
 public:
  explicit ExampleEndpoint(Pistache::Address addr);

  void init(size_t thr);
  void start();

 private:
  void setupRoutes();
  void handleLol(const Pistache::Rest::Request& /*unused*/, Pistache::Http::ResponseWriter response);
  void handleStart(const Pistache::Rest::Request& /*unused*/, Pistache::Http::ResponseWriter response);

  Pistache::Http::Endpoint httpEndpoint_;
  Pistache::Rest::Router router_;
  zmq::context_t context_;
  zmq::socket_t socket_;
};

#endif // REST_EXAMPLE_ENDPOINT_HPP
