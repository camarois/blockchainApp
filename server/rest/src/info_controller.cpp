#include <common/models.hpp>
#include <rest/info_controller.hpp>

namespace Rest {

InfoController::InfoController(const std::shared_ptr<Pistache::Rest::Router>& router) {
  setupRoutes(router);
}

void InfoController::setupRoutes(const std::shared_ptr<Pistache::Rest::Router>& router) {
  Pistache::Rest::Routes::Post(*router, kBasePath + "cours",
			       Pistache::Rest::Routes::bind(&InfoController::handleClasses, this));
}

void InfoController::handleClasses(const Pistache::Rest::Request& request,
				   Pistache::Http::ResponseWriter response) {
  Common::Models::ClassesRequest classesRequest = nlohmann::json::parse(request.body());
  std::vector<Common::Models::Result> results = {{}, {}};
  response.send(Pistache::Http::Code::I_m_a_teapot, ((nlohmann::json)results).dump(4));
}

}  // namespace Rest
