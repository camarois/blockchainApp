#include <common/database.hpp>
#include <common/message_helper.hpp>
#include <common/models.hpp>
#include <gflags/gflags.h>
#include <rest/user_controller.hpp>

DECLARE_string(database);

namespace Rest {

UserController::UserController(const std::shared_ptr<Pistache::Rest::Router>& router) { setupRoutes(router); }

void UserController::setupRoutes(const std::shared_ptr<Pistache::Rest::Router>& router) {
  Pistache::Rest::Routes::Post(*router, kBasePath_ + "login",
                               Pistache::Rest::Routes::bind(&UserController::handleLogin, this));
  Pistache::Rest::Routes::Post(*router, kBasePath_ + "logout",
                               Pistache::Rest::Routes::bind(&UserController::handleLogout, this));
  Pistache::Rest::Routes::Post(*router, kBasePath_ + "password",
                               Pistache::Rest::Routes::bind(&UserController::handlePassword, this));
}

void UserController::handleLogin(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response) {
  Common::Models::LoginRequest loginRequest = nlohmann::json::parse(request.body());
  Common::Database db = Common::Database(FLAGS_database);
  auto user = db.getUser(loginRequest.username);

  if (loginRequest.username == user->username && loginRequest.password == user->password) {
    Rest::TokenManager tokenManager = Rest::TokenManager(loginRequest.username, loginRequest.password);
    tokenManager.decode(tokenManager.getSignature());

    Common::Models::LoginResponse loginResponse = {};
    response.headers().add<Pistache::Http::Header::Authorization>(tokenManager.getSignature());
    response.send(Pistache::Http::Code::Ok, Common::Models::toStr(loginResponse));
  }
  response.send(Pistache::Http::Code::Forbidden);
}

void UserController::handleLogout(const Pistache::Rest::Request& /*request*/, Pistache::Http::ResponseWriter response) {
  response.send(Pistache::Http::Code::I_m_a_teapot, "TODO");
}

void UserController::handlePassword(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response) {
  Common::Models::PasswordRequest loginRequest = nlohmann::json::parse(request.body());
  response.send(Pistache::Http::Code::I_m_a_teapot, "TODO");
}

}  // namespace Rest
