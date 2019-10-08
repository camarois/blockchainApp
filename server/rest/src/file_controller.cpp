#include <common/models.hpp>
#include <rest/file_controller.hpp>

namespace Rest {

FileController::FileController(const std::shared_ptr<Pistache::Rest::Router>& router) {
  setupRoutes(router);
}

void FileController::setupRoutes(const std::shared_ptr<Pistache::Rest::Router>& router) {
  Pistache::Rest::Routes::Post(*router, kBasePath_ + "notes",
                               Pistache::Rest::Routes::bind(&FileController::handleGrades, this));
}

void FileController::handleGrades(const Pistache::Rest::Request& request,
                                  Pistache::Http::ResponseWriter response) {
  Common::Models::GradesRequest gradesRequest = nlohmann::json::parse(request.body());
  Pistache::Http::serveFile(response, "build.sh");  // TODO(frgraf) change with a PDF
}

}  // namespace Rest
