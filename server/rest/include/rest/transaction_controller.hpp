#ifndef REST_TRANSACTION_CONTROLLER_HPP
#define REST_TRANSACTION_CONTROLLER_HPP

#include <nlohmann/json.hpp>
#include <pistache/endpoint.h>
#include <pistache/http.h>
#include <pistache/router.h>

#include <common/example.hpp>

namespace Rest {

class TransactionController {
 public:
  explicit TransactionController(const std::shared_ptr<Pistache::Rest::Router>& router);

 private:
  void setupRoutes(const std::shared_ptr<Pistache::Rest::Router>& router);
  void handleTransaction(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response);

  const std::string kBasePath = "/transaction/";
};

}  // namespace Rest

#endif  // REST_TRANSACTION_CONTROLLER_HPP
