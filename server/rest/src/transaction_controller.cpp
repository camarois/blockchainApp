#include <common/base64.hpp>
#include <common/models.hpp>
#include <filesystem>
#include <fstream>
#include <gflags/gflags.h>
#include <rest/transaction_controller.hpp>
#include <common/format_helper.hpp>

DECLARE_string(transactions);

namespace Rest {

TransactionController::TransactionController(const std::shared_ptr<Rest::CustomRouter>& router) { setupRoutes(router); }

void TransactionController::setupRoutes(const std::shared_ptr<Rest::CustomRouter>& router) {
  router->post(kBasePath_, Pistache::Rest::Routes::bind(&TransactionController::handleTransaction, this));
}

void TransactionController::handleTransaction(const Pistache::Rest::Request& request,
                                              Pistache::Http::ResponseWriter response) {
  Common::Models::TransactionRequest transactionRequest = nlohmann::json::parse(request.body());
  std::filesystem::create_directories(FLAGS_transactions);
  // Example: transactions/3-inf3995.pdf -> Project 3 in fall
  std::ofstream out(FLAGS_transactions + std::to_string(transactionRequest.trimester) + "-" +
                    Common::FormatHelper::toLower(transactionRequest.acronym) + ".pdf");
  out << Common::Base64::decode(transactionRequest.base64Pdf);
  out.close();
  response.send(Pistache::Http::Code::I_m_a_teapot, "TODO");
}

}  // namespace Rest
