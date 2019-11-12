#include "common/database.hpp"
#include "rest/zmq.hpp"
#include <rest/ping_controller.hpp>

namespace Rest {

PingController::PingController(const std::shared_ptr<Rest::CustomRouter>& router, std::shared_ptr<ZMQWorker> zmqWorker)
    : zmqWorker_(std::move(zmqWorker)) {
  setupRoutes(router);
}

void PingController::setupRoutes(const std::shared_ptr<Rest::CustomRouter>& router) {
  router->get("/ping", Pistache::Rest::Routes::bind(&PingController::handlePing), false);
  router->get("/sping", Pistache::Rest::Routes::bind(&PingController::handlePing));
}

void PingController::handlePing(const Pistache::Rest::Request& /*unused*/, Pistache::Http::ResponseWriter response) {
  response.send(Pistache::Http::Code::Ok, "PONG");
}

}  // namespace Rest
