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
  Pistache::Rest::Routes::Post(*router, kBasePath_ + "register",
                               Pistache::Rest::Routes::bind(&UserController::handleRegister, this));
}

void UserController::handleLogin(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response) {
  Common::Models::LoginRequest loginRequest = nlohmann::json::parse(request.body());
  Common::Database db(FLAGS_database);
  auto user = db.getUser(loginRequest.username);

  if (loginRequest.username == user->username && loginRequest.password == user->password) {
    Rest::TokenManager tokenManager(loginRequest.username, loginRequest.password);
    tokenManager.decode();

    Common::Models::LoginResponse loginResponse = {};
    response.headers().add<Pistache::Http::Header::Authorization>(tokenManager.getSignature());
    response.send(Pistache::Http::Code::Ok, Common::Models::toStr(loginResponse));
  }
  response.send(Pistache::Http::Code::Forbidden);
}

void UserController::handleLogout(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response) {
  Rest::TokenManager tokenManager(request);
  if (!tokenManager.decode()) {
    response.send(Pistache::Http::Code::Ok);
  }
  response.send(Pistache::Http::Code::Unauthorized);
}

void UserController::handlePassword(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response) {
  Common::Models::PasswordRequest passwordRequest = nlohmann::json::parse(request.body());
  Rest::TokenManager tokenManager(request);
  if (!tokenManager.decode()) {
    response.headers().add<Pistache::Http::Header::Authorization>(tokenManager.getSignature());
    response.send(Pistache::Http::Code::Ok);
  }
  response.send(Pistache::Http::Code::Unauthorized);
}

void UserController::handleRegister(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response) {
  Common::Models::LoginRequest registerRequest = nlohmann::json::parse(request.body());
  Common::Database db(FLAGS_database);

  if (db.createUser(registerRequest)) {
    Rest::TokenManager tokenManager(registerRequest.username, registerRequest.password);
    tokenManager.decode();

    Common::Models::LoginResponse registerResponse = {};
    response.headers().add<Pistache::Http::Header::Authorization>(tokenManager.getSignature());
    response.send(Pistache::Http::Code::Ok, Common::Models::toStr(registerResponse));
  }
  response.send(Pistache::Http::Code::Forbidden);
}

}  // namespace Rest
