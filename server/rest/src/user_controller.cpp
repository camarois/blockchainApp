#include <common/message_helper.hpp>
#include <rest/user_controller.hpp>

using json = nlohmann::json;

namespace rest {

UserController::UserController(const std::shared_ptr<Pistache::Rest::Router>& router) {
  setupRoutes(router);
}

void UserController::setupRoutes(const std::shared_ptr<Pistache::Rest::Router>& router) {
  Pistache::Rest::Routes::Get(*router, "/lol",
			      Pistache::Rest::Routes::bind(&UserController::handleLol, this));
}

void UserController::handleLol(const Pistache::Rest::Request& /*unused*/,
			       Pistache::Http::ResponseWriter response) {
  static int lol = 0;
  Example example;
  json j = {{"lol", lol++}, {"example", example.example()}};
  response.send(Pistache::Http::Code::Ok, j.dump(4));
}

} // namespace rest
