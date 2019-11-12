#ifndef REST_TRANSACTION_CONTROLLER_HPP
#define REST_TRANSACTION_CONTROLLER_HPP

#include <nlohmann/json.hpp>
#include <pistache/endpoint.h>
#include <pistache/http.h>
#include <pistache/router.h>
#include <rest/custom_router.hpp>
#include <rest/zmq.hpp>

namespace Rest {

class TransactionController {
 public:
  explicit TransactionController(const std::shared_ptr<Rest::CustomRouter>& router,
                                 std::shared_ptr<ZMQWorker> zmqWorker);

 private:
  void setupRoutes(const std::shared_ptr<Rest::CustomRouter>& router);
  void handleTransaction(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response);

  std::shared_ptr<ZMQWorker> zmqWorker_;
  const std::string kBasePath_ = "/transaction/";
};

}  // namespace Rest

#endif  // REST_TRANSACTION_CONTROLLER_HPP
