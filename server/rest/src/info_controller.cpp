#include <common/models.hpp>
#include <rest/info_controller.hpp>

namespace Rest {

InfoController::InfoController(const std::shared_ptr<Pistache::Rest::Router>& router) { setupRoutes(router); }

void InfoController::setupRoutes(const std::shared_ptr<Pistache::Rest::Router>& router) {
  Pistache::Rest::Routes::Post(*router, kBasePath_ + "cours",
                               Pistache::Rest::Routes::bind(&InfoController::handleClasses, this));
  Pistache::Rest::Routes::Post(*router, kBasePath_ + "etudiant",
                               Pistache::Rest::Routes::bind(&InfoController::handleStudents, this));
}

void InfoController::handleClasses(const Pistache::Rest::Request& request,
                                   Pistache::Http::ResponseWriter response) {
  Common::Models::ClassesRequest classesRequest = nlohmann::json::parse(request.body());
  std::vector<Common::Models::Result> results = {{}, {}};
  response.send(Pistache::Http::Code::I_m_a_teapot, static_cast<nlohmann::json>(results).dump(4));
}

void InfoController::handleStudents(const Pistache::Rest::Request& request,
                                    Pistache::Http::ResponseWriter response) {
  Common::Models::StudentRequest studentRequest = nlohmann::json::parse(request.body());
  Common::Models::StudentResponse studentResponse;
  response.send(Pistache::Http::Code::I_m_a_teapot, static_cast<nlohmann::json>(studentResponse).dump(4));
}

}  // namespace Rest
