#include <common/message_helper.hpp>
#include <rest/main_controller.hpp>

using json = nlohmann::json;

MainController::MainController(Pistache::Address addr, size_t thr)
    : router_(std::make_shared<Pistache::Rest::Router>()),
      httpEndpoint_(addr),
      userController_(router_),
      exampleController_(router_) {
  auto opts = Pistache::Http::Endpoint::options().threads(thr);
  httpEndpoint_.init(opts);
}

void MainController::start() {
  httpEndpoint_.setHandler(router_->handler());
  httpEndpoint_.serve();
}
