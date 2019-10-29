#include <common/message_helper.hpp>
#include <gflags/gflags.h>
#include <rest/main_controller.hpp>

DECLARE_string(cert);
DECLARE_string(key);

namespace Rest {

MainController::MainController(Pistache::Address addr, size_t thr)
    : httpEndpoint_(addr),
      router_(std::make_shared<Rest::CustomRouter>()),
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
  httpEndpoint_.useSSL(FLAGS_cert, FLAGS_key);
  httpEndpoint_.serve();
}

}  // namespace Rest
