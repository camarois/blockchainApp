#include <common/database.hpp>
#include <common/models.hpp>
#include <rest/info_controller.hpp>

namespace Rest {

InfoController::InfoController(const std::shared_ptr<Rest::CustomRouter>& router) { setupRoutes(router); }

void InfoController::setupRoutes(const std::shared_ptr<Rest::CustomRouter>& router) {
  router->post(kBasePath_ + "cours", Pistache::Rest::Routes::bind(&InfoController::handleClasses, this));
  router->post(kBasePath_ + "etudiant", Pistache::Rest::Routes::bind(&InfoController::handleStudents, this));
}

void InfoController::handleClasses(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response) {
  Common::Models::ClassesRequest classesRequest = nlohmann::json::parse(request.body());
  Common::Database db("blockchain.db");
  int classId = db.checkForExistingClass(classesRequest.acronym, classesRequest.trimester);
  std::vector<Common::Models::Result> results = {{}, {}};
  if (classId != -1) {
    results = db.getClassResult(classId);
  }
  response.send(Pistache::Http::Code::I_m_a_teapot, Common::Models::toStr(results));
}

void InfoController::handleStudents(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response) {
  Common::Models::StudentRequest studentRequest = nlohmann::json::parse(request.body());
  Common::Models::Result result;
  Common::Database db("blockchain.db");
  int classId = db.checkForExistingClass(studentRequest.acronym, studentRequest.trimester);
  if (classId != -1) {
    result = db.getStudentResult(classId, studentRequest.id);
  }
  response.send(Pistache::Http::Code::I_m_a_teapot, Common::Models::toStr(result));
}

}  // namespace Rest
