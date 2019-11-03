#include <common/format_helper.hpp>
#include <common/models.hpp>
#include <gflags/gflags.h>
#include <rest/file_controller.hpp>

DECLARE_string(transactions);

namespace Rest {

FileController::FileController(const std::shared_ptr<Rest::CustomRouter>& router) { setupRoutes(router); }

void FileController::setupRoutes(const std::shared_ptr<Rest::CustomRouter>& router) {
  router->post(kBasePath_ + "notes", Pistache::Rest::Routes::bind(&FileController::handleGrades, this));
}

void FileController::handleGrades(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response) {
  Common::Models::GradesRequest gradesRequest = nlohmann::json::parse(request.body());
  Pistache::Http::serveFile(response, FLAGS_transactions + std::to_string(gradesRequest.trimester) + "-" +
                                          Common::FormatHelper::toLower(gradesRequest.acronym) + ".pdf");
}

}  // namespace Rest
