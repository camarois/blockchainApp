#include <common/message_helper.hpp>
#include <common/models.hpp>
#include <rest/user_controller.hpp>

namespace Rest {

UserController::UserController(const std::shared_ptr<Pistache::Rest::Router>& router) {
  setupRoutes(router);
}

void UserController::setupRoutes(const std::shared_ptr<Pistache::Rest::Router>& router) {
  Pistache::Rest::Routes::Get(*router, kBasePath + "lol",
			      Pistache::Rest::Routes::bind(&UserController::handleLol, this));
  Pistache::Rest::Routes::Post(*router, kBasePath + "login",
			      Pistache::Rest::Routes::bind(&UserController::handleLogin, this));
}

void UserController::handleLol(const Pistache::Rest::Request& /*unused*/,
			       Pistache::Http::ResponseWriter response) {
  static int lol = 0;
  Example example;
  nlohmann::json j = {{"lol", lol++}, {"example", example.example()}};
  response.send(Pistache::Http::Code::Ok, j.dump(4));
}

void UserController::handleLogin(const Pistache::Rest::Request& request,
				 Pistache::Http::ResponseWriter response) {
  auto loginRequest = Common::Models::from_str<Common::Models::LoginRequest>(request.body());
  // Common::Models::LoginRequest loginRequest = nlohmann::json::parse(request.body());
  response.send(Pistache::Http::Code::Ok, Common::Models::to_str(loginRequest));
  // response.send(Pistache::Http::Code::Ok, ((nlohmann::json)loginRequest).dump());
}

}  // namespace Rest
