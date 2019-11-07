#include <common/base64.hpp>
#include <common/database.hpp>
#include <common/format_helper.hpp>
#include <common/models.hpp>
#include <filesystem>
#include <fstream>
#include <gflags/gflags.h>
#include <iostream>
#include <rest/transaction_controller.hpp>

DECLARE_string(db);
DECLARE_string(transactions);

namespace Rest {

TransactionController::TransactionController(const std::shared_ptr<Rest::CustomRouter>& router,
                                             const std::shared_ptr<ZMQWorker>& zmqWorker)
    : zmqWorker_(zmqWorker) {
  setupRoutes(router);
}

void TransactionController::setupRoutes(const std::shared_ptr<Rest::CustomRouter>& router) {
  router->post(kBasePath_, Pistache::Rest::Routes::bind(&TransactionController::handleTransaction, this));
}

void TransactionController::handleTransaction(const Pistache::Rest::Request& request,
                                              Pistache::Http::ResponseWriter response) {
  Common::Models::TransactionRequest transactionRequest = nlohmann::json::parse(request.body());
  Common::Database db(FLAGS_db);
  auto classId = zmqWorker_->getRequest({Common::Functions::checkForExistingClass,
                                         {transactionRequest.acronym, std::to_string(transactionRequest.trimester)}});
  if (classId.found) {
    zmqWorker_->updateRequest({Common::Functions::deleteExistingClass, {classId.data}});
    zmqWorker_->updateRequest({Common::Functions::deleteExistingResults, {classId.data}});
  }
  auto newClassId = zmqWorker_->updateRequest(
      {Common::Functions::addNewClass, {transactionRequest.acronym, std::to_string(transactionRequest.trimester)}});
  auto newClassId = zmqWorker_->updateRequest(
      {Common::Functions::addNewResult, {transactionRequest.acronym, std::to_string(transactionRequest.trimester), newClassId.data}});

  std::filesystem::create_directories(FLAGS_transactions);
  // Example: transactions/3-inf3995.pdf -> Project in fall
  std::ofstream out(FLAGS_transactions + std::to_string(transactionRequest.trimester) + "-" +
                    Common::FormatHelper::toLower(transactionRequest.acronym) + ".pdf");
  out << Common::Base64::decode(transactionRequest.base64Pdf);
  out.close();
  response.send(Pistache::Http::Code::Ok);
}

}  // namespace Rest
