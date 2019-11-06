#include <common/database.hpp>
#include <common/models.hpp>
#include <gflags/gflags.h>
#include <rest/info_controller.hpp>

DECLARE_string(db);

namespace Rest {

InfoController::InfoController(const std::shared_ptr<Rest::CustomRouter>& router) { setupRoutes(router); }

void InfoController::setupRoutes(const std::shared_ptr<Rest::CustomRouter>& router) {
  router->post(kBasePath_ + "cours", Pistache::Rest::Routes::bind(&InfoController::handleClasses, this));
  router->post(kBasePath_ + "etudiant", Pistache::Rest::Routes::bind(&InfoController::handleStudents, this));
}

void InfoController::handleClasses(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response) {
  Common::Models::ClassesRequest classesRequest = nlohmann::json::parse(request.body());
  Common::Database db(FLAGS_db);
  std::optional<int> classId = db.checkForExistingClass(classesRequest.acronym, classesRequest.trimester);
  std::vector<Common::Models::Result> results;
  if (classId) {
    results = db.getClassResult(classId.value());
  }
  Common::Models::ClassesResponse classesResponse = {results};
  response.send(Pistache::Http::Code::Ok, Common::Models::toStr(classesResponse));
}

void InfoController::handleStudents(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response) {
  Common::Models::StudentRequest studentRequest = nlohmann::json::parse(request.body());
  std::optional<Common::Models::Result> result;
  Common::Database db(FLAGS_db);
  std::optional<int> classId = db.checkForExistingClass(studentRequest.acronym, studentRequest.trimester);
  if (classId) {
    result = db.getStudentResult(classId.value(), studentRequest.id);
  }
  response.send(Pistache::Http::Code::Ok, Common::Models::toStr(result.value()));
}

}  // namespace Rest
