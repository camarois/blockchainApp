#include <common/models.hpp>
#include <rest/transaction_controller.hpp>

namespace Rest {

TransactionController::TransactionController(
    const std::shared_ptr<Pistache::Rest::Router>& router) {
  setupRoutes(router);
}

void TransactionController::setupRoutes(const std::shared_ptr<Pistache::Rest::Router>& router) {
  Pistache::Rest::Routes::Post(
      *router, kBasePath,
      Pistache::Rest::Routes::bind(&TransactionController::handleTransaction, this));
}

void TransactionController::handleTransaction(const Pistache::Rest::Request& request,
					      Pistache::Http::ResponseWriter response) {
  std::cout << nlohmann::json::parse(request.body()).dump(4) << std::endl;
  Common::Models::TransactionRequest transactionRequest = nlohmann::json::parse(request.body());
  std::cout << ((nlohmann::json)transactionRequest).dump(4) << std::endl;
  Common::Models::LoginResponse loginResponse;
  response.send(Pistache::Http::Code::I_m_a_teapot, ((nlohmann::json)loginResponse).dump(4));
}

}  // namespace Rest
