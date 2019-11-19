#ifndef REST_MAIN_CONTROLLER_HPP
#define REST_MAIN_CONTROLLER_HPP

#include <common/database.hpp>
#include <pistache/endpoint.h>
#include <pistache/http.h>
#include <pistache/router.h>
#include <rest/admin_controller.hpp>
#include <rest/custom_router.hpp>
#include <rest/file_controller.hpp>
#include <rest/info_controller.hpp>
#include <rest/ping_controller.hpp>
#include <rest/transaction_controller.hpp>
#include <rest/user_controller.hpp>
#include <rest/zmq.hpp>

namespace Rest {

class MainController {
 public:
  explicit MainController(Pistache::Address addr, size_t thr);

  void start();

 private:
  Pistache::Http::Endpoint httpEndpoint_;
  std::shared_ptr<Rest::CustomRouter> router_;
  Rest::UserController userController_;
  Rest::PingController pingController_;
  Rest::TransactionController transactionController_;
  Rest::InfoController infoController_;
  Rest::FileController fileController_;
  Rest::AdminController adminController_;
};

}  // namespace Rest

#endif  // REST_MAIN_CONTROLLER_HPP
