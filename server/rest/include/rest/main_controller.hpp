#ifndef REST_MAIN_CONTROLLER_HPP
#define REST_MAIN_CONTROLLER_HPP

#include <common/example.hpp>
#include <pistache/endpoint.h>
#include <pistache/http.h>
#include <pistache/router.h>
#include <rest/example_controller.hpp>
#include <rest/user_controller.hpp>
#include <rest/transaction_controller.hpp>
#include <rest/info_controller.hpp>
#include <rest/file_controller.hpp>

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
  Rest::TransactionController transactionController_;
  Rest::InfoController infoController_;
  Rest::FileController fileController_;
};

}  // namespace Rest

#endif  // REST_MAIN_CONTROLLER_HPP
