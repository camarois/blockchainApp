#include <common/database.hpp>
#include <common/models.hpp>
#include <common/token_helper.hpp>
#include <gflags/gflags.h>
#include <rest/admin_controller.hpp>

namespace Rest {

AdminController::AdminController(const std::shared_ptr<Rest::CustomRouter>& router) { setupRoutes(router); }

void AdminController::setupRoutes(const std::shared_ptr<Rest::CustomRouter>& router) {
  router->post(kBasePath_ + "login", Pistache::Rest::Routes::bind(&AdminController::handleLogin, this), false);
  router->post(kBasePath_ + "logout", Pistache::Rest::Routes::bind(&AdminController::handleLogout, this));
  router->post(kBasePath_ + "motdepasse", Pistache::Rest::Routes::bind(&AdminController::handlePassword, this));
  router->post(kBasePath_ + "chaine/" + kId_, Pistache::Rest::Routes::bind(&AdminController::handleChain, this));
  router->post(kBasePath_ + "logs/" + kId_, Pistache::Rest::Routes::bind(&AdminController::handleLogs, this));
  router->post(kBasePath_ + "creationcompte",
               Pistache::Rest::Routes::bind(&AdminController::handleCreateAccount, this));
  router->post(kBasePath_ + "suppressioncompte",
               Pistache::Rest::Routes::bind(&AdminController::handleDeleteAccount, this));
}

void AdminController::handleLogin(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response) {
  Common::Models::LoginRequest loginRequest = nlohmann::json::parse(request.body());
  Common::Models::GetSaltRequest getSaltRequest = {loginRequest.username};
  auto salt = Rest::ZMQWorker::get()->getRequest({Common::Functions::GetSalt, Common::Models::toStr(getSaltRequest)});
  Common::Models::ContainsAdminRequest containsAdminRequest = {loginRequest, salt.data, true};
  if (salt.found && Rest::ZMQWorker::get()
                        ->getRequest({Common::Functions::ContainsAdmin, Common::Models::toStr(containsAdminRequest)})
                        .found) {
    auto token = Common::TokenHelper::encode(loginRequest.username, loginRequest.password);
    response.headers().add<Pistache::Http::Header::Authorization>(token);
    response.send(Pistache::Http::Code::Ok);
  } else {
    response.send(Pistache::Http::Code::Forbidden);
  }
}

// TODO(gabriel): faire dequoi d'utile avec cette fonction
// NOLINTNEXTLINE(readability-convert-member-functions-to-static)
void AdminController::handleLogout(const Pistache::Rest::Request& /*request*/,
                                   Pistache::Http::ResponseWriter response) {
  // TODO(camarois) jwt db verification
  response.send(Pistache::Http::Code::Ok);
}

void AdminController::handlePassword(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response) {
  Common::Models::PasswordRequest passwordRequest = nlohmann::json::parse(request.body());
  std::string authHeader = request.headers().getRaw("Authorization").value();
  std::optional<std::string> username = Common::TokenHelper::decodeUsername(authHeader);
  Common::Models::LoginRequest loginRequest = {username.value(), passwordRequest.oldPwd};

  auto salt = Rest::ZMQWorker::get()->getRequest({Common::Functions::GetSalt, {loginRequest.username}});
  Common::Models::ContainsAdminRequest containsAdminRequest = {loginRequest, salt.data, true};
  if (salt.found && Rest::ZMQWorker::get()
                        ->getRequest({Common::Functions::ContainsUser, Common::Models::toStr(containsAdminRequest)})
                        .found) {
    Common::Models::SetUserPasswordRequest setUserPasswordRequest = {loginRequest.username, passwordRequest, salt.data,
                                                                     true};
    Rest::ZMQWorker::get()->updateRequest(
        {Common::Functions::SetUserPassword, Common::Models::toStr(containsAdminRequest)});
    response.send(Pistache::Http::Code::Ok);
  } else {
    response.send(Pistache::Http::Code::Forbidden);
  }
}

// TODO(gabriel): faire dequoi d'utile avec cette fonction
// NOLINTNEXTLINE(readability-convert-member-functions-to-static)
void AdminController::handleChain(const Pistache::Rest::Request& /*request*/, Pistache::Http::ResponseWriter response) {
  // auto miner = request.param(kId_).as<int>();
  // Common::Models::ChainRequest chainRequest = nlohmann::json::parse(request.body());
  Common::Models::ChainResponse chainResponse = {"test", std::make_unique<Common::Models::ChainResponse>()};
  response.send(Pistache::Http::Code::I_m_a_teapot, Common::Models::toStr(chainResponse));
}

void AdminController::handleLogs(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response) {
  Common::Models::LogsRequest logsRequest = nlohmann::json::parse(request.body());
  int provenance = request.param(kId_).as<std::string>() != "serveurweb" ? request.param(kId_).as<int>() : 0;
  std::vector<Common::Models::Information> logs;
  logs = Common::Database::get()->getLogs(logsRequest.last, provenance);
  Common::Models::LogsResponse logsResponse = {{{logs}}};
  response.send(Pistache::Http::Code::Ok, Common::Models::toStr(logsResponse));
}

// TODO(gabriel): faire dequoi d'utile avec cette fonction
// NOLINTNEXTLINE(readability-convert-member-functions-to-static)
void AdminController::handleCreateAccount(const Pistache::Rest::Request& request,
                                          Pistache::Http::ResponseWriter response) {
  Common::Models::AddUserRequest addUserRequest = nlohmann::json::parse(request.body());
  Rest::ZMQWorker::get()->updateRequest({Common::Functions::AddUser, Common::Models::toStr(addUserRequest)});
  Common::Models::LoginResponse registerResponse = {};
  response.send(Pistache::Http::Code::Ok, Common::Models::toStr(registerResponse));
}

// TODO(gabriel): faire dequoi d'utile avec cette fonction
// NOLINTNEXTLINE(readability-convert-member-functions-to-static)
void AdminController::handleDeleteAccount(const Pistache::Rest::Request& request,
                                          Pistache::Http::ResponseWriter response) {
  Common::Models::DeleteAccountRequest logsRequest = nlohmann::json::parse(request.body());
  response.send(Pistache::Http::Code::I_m_a_teapot, "TODO");
}

}  // namespace Rest
