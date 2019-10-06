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
  Common::Models::TransactionRequest transactionRequest = nlohmann::json::parse(request.body());
  response.send(Pistache::Http::Code::I_m_a_teapot, "TODO");
}

}  // namespace Rest
