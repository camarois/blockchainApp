#include <common/message_helper.hpp>
#include <common/models.hpp>
#include <rest/user_controller.hpp>

namespace Rest {

UserController::UserController(const std::shared_ptr<Pistache::Rest::Router>& router) {
  setupRoutes(router);
}

void UserController::setupRoutes(const std::shared_ptr<Pistache::Rest::Router>& router) {
  Pistache::Rest::Routes::Post(*router, kBasePath_ + "login",
                               Pistache::Rest::Routes::bind(&UserController::handleLogin, this));
  Pistache::Rest::Routes::Post(*router, kBasePath_ + "logout",
                               Pistache::Rest::Routes::bind(&UserController::handleLogout, this));
  Pistache::Rest::Routes::Post(*router, kBasePath_ + "password",
                               Pistache::Rest::Routes::bind(&UserController::handlePassword, this));
}

void UserController::handleLogin(const Pistache::Rest::Request& request,
                                 Pistache::Http::ResponseWriter response) {
  Common::Models::LoginRequest loginRequest = nlohmann::json::parse(request.body());
  Common::Models::LoginResponse loginResponse = {};
  response.send(Pistache::Http::Code::I_m_a_teapot,
                static_cast<nlohmann::json>(loginResponse).dump(4));
}

void UserController::handleLogout(const Pistache::Rest::Request& /*request*/,
                                  Pistache::Http::ResponseWriter response) {
  response.send(Pistache::Http::Code::I_m_a_teapot, "TODO");
}

void UserController::handlePassword(const Pistache::Rest::Request& request,
                                    Pistache::Http::ResponseWriter response) {
  Common::Models::PasswordRequest loginRequest = nlohmann::json::parse(request.body());
  response.send(Pistache::Http::Code::I_m_a_teapot, "TODO");
}

}  // namespace Rest
