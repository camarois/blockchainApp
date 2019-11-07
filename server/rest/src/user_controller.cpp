#include <common/database.hpp>
#include <common/format_helper.hpp>
#include <common/message_helper.hpp>
#include <common/models.hpp>
#include <common/token_helper.hpp>
#include <gflags/gflags.h>
#include <rest/user_controller.hpp>

DECLARE_string(db);

namespace Rest {

UserController::UserController(const std::shared_ptr<Rest::CustomRouter>& router,
                               const std::shared_ptr<ZMQWorker>& zmqWorker)
    : zmqWorker_(zmqWorker) {
  setupRoutes(router);
}

void UserController::setupRoutes(const std::shared_ptr<Rest::CustomRouter>& router) {
  router->post(kBasePath_ + "login", Pistache::Rest::Routes::bind(&UserController::handleLogin, this), false);
  router->post(kBasePath_ + "logout", Pistache::Rest::Routes::bind(&UserController::handleLogout, this));
  router->post(kBasePath_ + "motdepasse", Pistache::Rest::Routes::bind(&UserController::handlePassword, this));
  router->post(kBasePath_ + "register", Pistache::Rest::Routes::bind(&UserController::handleRegister, this), false);
}

void UserController::handleLogin(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response) {
  Common::Models::LoginRequest loginRequest = nlohmann::json::parse(request.body());
  Common::Database db(FLAGS_db);
  auto salt = db.getSalt(loginRequest.username);
  if (salt && db.containsUser(loginRequest, salt.value())) {
    auto token = Common::TokenHelper::encode(loginRequest.username, loginRequest.password);
    response.headers().add<Pistache::Http::Header::Authorization>(token);
    Common::Models::LoginResponse loginResponse = {};
    response.send(Pistache::Http::Code::Ok, Common::Models::toStr(loginResponse));
  } else {
    response.send(Pistache::Http::Code::Forbidden);
  }
}

void UserController::handleLogout(const Pistache::Rest::Request& /*request*/, Pistache::Http::ResponseWriter response) {
  // TODO(camarois) jwt db verification
  response.send(Pistache::Http::Code::Ok);
}

void UserController::handlePassword(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response) {
  Common::Models::PasswordRequest passwordRequest = nlohmann::json::parse(request.body());
  Common::Database db(FLAGS_db);
  std::string authHeader = request.headers().getRaw("Authorization").value();
  std::optional<std::string> username = Common::TokenHelper::decodeUsername(authHeader);
  Common::Models::LoginRequest loginRequest = {username.value(), passwordRequest.oldPwd};

  auto salt = db.getSalt(loginRequest.username);
  if (salt && db.containsUser(loginRequest, salt.value())) {
    db.setUserPassword(loginRequest.username, passwordRequest, salt.value());
    response.send(Pistache::Http::Code::Ok);
  } else {
    response.send(Pistache::Http::Code::Forbidden);
  }
}

void UserController::handleRegister(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response) {
  Common::Models::LoginRequest registerRequest = nlohmann::json::parse(request.body());
  // Common::Database db(FLAGS_db);
  // db.addUser(registerRequest);
  zmqWorker_->updateRequest({Common::Functions::addUser, {registerRequest.username, registerRequest.password}});
  Common::Models::LoginResponse registerResponse = {};
  response.send(Pistache::Http::Code::Ok, Common::Models::toStr(registerResponse));
}

}  // namespace Rest
