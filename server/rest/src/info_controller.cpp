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
  std::vector<Common::Models::Result> results = {{}, {}};
  response.send(Pistache::Http::Code::I_m_a_teapot, Common::Models::toStr(results));
}

void InfoController::handleStudents(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response) {
  Common::Models::StudentRequest studentRequest = nlohmann::json::parse(request.body());
  Common::Models::StudentResponse studentResponse;
  response.send(Pistache::Http::Code::I_m_a_teapot, Common::Models::toStr(studentResponse));
}

}  // namespace Rest
