#include <common/base64.hpp>
#include <common/models.hpp>
#include <rest/transaction_controller.hpp>

namespace Rest {

TransactionController::TransactionController(const std::shared_ptr<Rest::CustomRouter>& router) { setupRoutes(router); }

void TransactionController::setupRoutes(const std::shared_ptr<Rest::CustomRouter>& router) {
  router->post(kBasePath_, Pistache::Rest::Routes::bind(&TransactionController::handleTransaction, this));
}

void TransactionController::handleTransaction(const Pistache::Rest::Request& request,
                                              Pistache::Http::ResponseWriter response) {
  Common::Models::TransactionRequest transactionRequest = nlohmann::json::parse(request.body());
  // std::cout << transactionRequest.base64Pdf << std::endl;
  // std::cout << Common::Base64::decode(transactionRequest.base64Pdf) << std::endl;
  // response.send(Pistache::Http::Code::I_m_a_teapot, Common::Base64::decode(transactionRequest.base64Pdf));
  response.send(Pistache::Http::Code::I_m_a_teapot, "TODO");
}

}  // namespace Rest
