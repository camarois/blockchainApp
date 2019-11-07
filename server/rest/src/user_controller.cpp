#include <common/database.hpp>
#include <common/format_helper.hpp>
#include <common/message_helper.hpp>
#include <common/models.hpp>
#include <common/token_helper.hpp>
#include <gflags/gflags.h>
#include <rest/user_controller.hpp>

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
  auto salt = zmqWorker_->getRequest({Common::Functions::getSalt, loginRequest.username});
  Common::Models::ContainsUserRequest containsUserRequest = {loginRequest, salt.data};
  if (salt.found &&
      zmqWorker_->getRequest({Common::Functions::containsUser, Common::Models::toStr(containsUserRequest)}).found) {
    auto token = Common::TokenHelper::encode(loginRequest.username, loginRequest.password);
    response.headers().add<Pistache::Http::Header::Authorization>(token);
    Common::Models::GetRoleRequest getRoleRequest = {loginRequest, salt.data};
    auto edition = zmqWorker_->getRequest({Common::Functions::getRole, Common::Models::toStr(getRoleRequest)});
    Common::Models::LoginResponse loginResponse = {edition.data == "1"};
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
  std::string authHeader = request.headers().getRaw("Authorization").value();
  std::optional<std::string> username = Common::TokenHelper::decodeUsername(authHeader);
  Common::Models::LoginRequest loginRequest = {username.value(), passwordRequest.oldPwd};

  auto salt = zmqWorker_->getRequest({Common::Functions::getSalt, loginRequest.username});
  Common::Models::ContainsUserRequest containsUserRequest = {loginRequest, salt.data};
  if (salt.found &&
      zmqWorker_->getRequest({Common::Functions::containsUser, Common::Models::toStr(containsUserRequest)}).found) {
    Common::Models::SetUserPasswordRequest setUserPasswordRequest = {loginRequest.username, passwordRequest, salt.data,
                                                                     false};
    zmqWorker_->updateRequest({Common::Functions::setUserPassword, Common::Models::toStr(setUserPasswordRequest)});
    response.send(Pistache::Http::Code::Ok);
  } else {
    response.send(Pistache::Http::Code::Forbidden);
  }
}

void UserController::handleRegister(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response) {
  Common::Models::LoginRequest registerRequest = nlohmann::json::parse(request.body());
  Common::Models::AddUserRequest addUserRequest = {registerRequest, false};
  zmqWorker_->updateRequest({Common::Functions::addUser, Common::Models::toStr(addUserRequest)});
  Common::Models::LoginResponse registerResponse = {};
  response.send(Pistache::Http::Code::Ok, Common::Models::toStr(registerResponse));
}

}  // namespace Rest
