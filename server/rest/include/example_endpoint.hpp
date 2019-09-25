#pragma once

#include <nlohmann/json.hpp>
#include <pistache/endpoint.h>
#include <pistache/http.h>
#include <pistache/router.h>
#include <zmq.hpp>

#include <common/example.hpp>

using json = nlohmann::json;

class ExampleEndpoint {
 public:
  ExampleEndpoint(Pistache::Address addr);

  void init(size_t thr);
  void start();

 private:
  void setupRoutes();
  void handleLol(const Pistache::Rest::Request&, Pistache::Http::ResponseWriter response);
  void handleStart(const Pistache::Rest::Request&, Pistache::Http::ResponseWriter response);

  std::shared_ptr<Pistache::Http::Endpoint> httpEndpoint;
  Pistache::Rest::Router router;
  zmq::context_t context;
  zmq::socket_t socket;
};
