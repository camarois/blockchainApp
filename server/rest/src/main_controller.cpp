#include <common/message_helper.hpp>
#include <rest/main_controller.hpp>

namespace Rest {

MainController::MainController(Pistache::Address addr, size_t thr)
    : httpEndpoint_(addr),
      router_(std::make_shared<Pistache::Rest::Router>()),
      // List of controllers:
      userController_(router_),
      exampleController_(router_),
      transactionController_(router_),
      infoController_(router_),
      fileController_(router_),
      adminController_(router_) {
  auto opts = Pistache::Http::Endpoint::options().threads(thr);
  httpEndpoint_.init(opts);
}

void MainController::start() {
  httpEndpoint_.setHandler(router_->handler());
  httpEndpoint_.serve();
}

}  // namespace Rest
