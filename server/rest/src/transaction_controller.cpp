#include <common/base64.hpp>
#include <common/database.hpp>
#include <common/format_helper.hpp>
#include <common/models.hpp>
#include <filesystem>
#include <fstream>
#include <gflags/gflags.h>
#include <iostream>
#include <rest/transaction_controller.hpp>

DECLARE_string(transactions);

namespace Rest {

TransactionController::TransactionController(const std::shared_ptr<Rest::CustomRouter>& router,
                                             std::shared_ptr<ZMQWorker> zmqWorker)
    : zmqWorker_(std::move(zmqWorker)) {
  setupRoutes(router);
}

void TransactionController::setupRoutes(const std::shared_ptr<Rest::CustomRouter>& router) {
  router->post(kBasePath_, Pistache::Rest::Routes::bind(&TransactionController::handleTransaction, this));
}

void TransactionController::handleTransaction(const Pistache::Rest::Request& request,
                                              Pistache::Http::ResponseWriter response) {
  Common::Models::TransactionRequest transactionRequest = nlohmann::json::parse(request.body());

  std::filesystem::create_directories(FLAGS_transactions);
  // Example: transactions/3-inf3995.pdf -> Project in fall
  std::ofstream out(FLAGS_transactions + std::to_string(transactionRequest.trimester) + "-" +
                    Common::FormatHelper::toLower(transactionRequest.acronym) + ".pdf");
  out << Common::Base64::decode(transactionRequest.base64Pdf);
  out.close();
  transactionRequest.base64Pdf = "[not saved]";

  Common::Models::CheckForExistingClassRequest checkForExistingClass = {transactionRequest.acronym,
                                                                        transactionRequest.trimester};
  auto classId =
      zmqWorker_->getRequest({Common::Functions::CheckForExistingClass, Common::Models::toStr(checkForExistingClass)});
  if (classId.found) {
    zmqWorker_->updateRequest({Common::Functions::DeleteExistingClass, classId.data});
    zmqWorker_->updateRequest({Common::Functions::DeleteExistingResults, classId.data});
  }
  auto newClassId =
      zmqWorker_->updateRequest({Common::Functions::AddNewClass, Common::Models::toStr(transactionRequest)});
  Common::Models::AddNewResultRequest addNewResultRequest = {transactionRequest, std::stoi(newClassId.data)};
  zmqWorker_->updateRequest({Common::Functions::AddNewResult, Common::Models::toStr(addNewResultRequest)});

  response.send(Pistache::Http::Code::Ok);
}

}  // namespace Rest
