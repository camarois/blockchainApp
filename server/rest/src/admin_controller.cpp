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
  router->get(kBasePath_ + "listeUsagers", Pistache::Rest::Routes::bind(&AdminController::handleListeUsagers, this));
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
  }
  else {
    response.send(Pistache::Http::Code::Forbidden);
  }
}

// TODO(gabriel): faire dequoi d'utile avec cette fonction
// NOLINTNEXTLINE(readability-convert-member-functions-to-static)
void AdminController::handleLogout(const Pistache::Rest::Request& /*request*/,
                                   Pistache::Http::ResponseWriter response) {
  response.send(Pistache::Http::Code::Ok);
}

void AdminController::handlePassword(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response) {
  Common::Models::PasswordRequest passwordRequest = nlohmann::json::parse(request.body());
  std::string authHeader = request.headers().getRaw("Authorization").value();
  std::optional<std::string> username = Common::TokenHelper::decodeUsername(authHeader);
  Common::Models::LoginRequest loginRequest = {username.value(), passwordRequest.oldPwd};
  Common::Models::GetSaltRequest getSaltRequest = {loginRequest.username};
  auto salt = Rest::ZMQWorker::get()->getRequest({Common::Functions::GetSalt, Common::Models::toStr(getSaltRequest)});
  Common::Models::ContainsAdminRequest containsAdminRequest = {loginRequest, salt.data, true};
  if (salt.found && Rest::ZMQWorker::get()
                        ->getRequest({Common::Functions::ContainsAdmin, Common::Models::toStr(containsAdminRequest)})
                        .found) {
    Common::Models::SetUserPasswordRequest setUserPasswordRequest = {loginRequest.username, passwordRequest, salt.data,
                                                                     true};
    Rest::ZMQWorker::get()->updateRequest(
        {Common::Functions::SetUserPassword, Common::Models::toStr(setUserPasswordRequest)});
    response.send(Pistache::Http::Code::Ok);
  }
  else {
    response.send(Pistache::Http::Code::Forbidden);
  }
}

// NOLINTNEXTLINE(readability-convert-member-functions-to-static)
void AdminController::handleChain(const Pistache::Rest::Request& /*request*/, Pistache::Http::ResponseWriter response) {
  // auto miner = request.param(kId_).as<int>();
  // Common::Models::ChainRequest chainRequest = nlohmann::json::parse(request.body());
  Common::Models::ChainResponse chainResponse = {"test", std::make_unique<Common::Models::ChainResponse>()};
  response.send(Pistache::Http::Code::I_m_a_teapot, Common::Models::toStr(chainResponse));
}

void AdminController::handleLogs(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response) {
  Common::Models::LogsRequest logsRequest = nlohmann::json::parse(request.body());
  Common::Models::LogsResponse logsResponse;
  if (request.param(kId_).as<std::string>() != "serveurweb") {
    int provenance = request.param(kId_).as<int>();
    Common::Models::GetLogsRequest getLogsRequest = {logsRequest.last, provenance};
    auto logsResults =
        Rest::ZMQWorker::get()->getRequest({Common::Functions::GetLogs, Common::Models::toStr(getLogsRequest)});
    logsResponse = {nlohmann::json::parse(logsResults.data)};
  }
  else {
    int provenance = 0;
    Common::Models::GetLogsRequest getLogsRequest = {logsRequest.last, provenance};
    logsResponse = {Common::Database::get()->getLogs(getLogsRequest)};
  }
  response.send(Pistache::Http::Code::Ok, Common::Models::toStr(logsResponse));
}

// NOLINTNEXTLINE(readability-convert-member-functions-to-static)
void AdminController::handleCreateAccount(const Pistache::Rest::Request& request,
                                          Pistache::Http::ResponseWriter response) {
  Common::Models::AddUserRequest addUserRequest = nlohmann::json::parse(request.body());
  Rest::ZMQWorker::get()->updateRequest({Common::Functions::AddUser, Common::Models::toStr(addUserRequest)});
  Common::Models::LoginResponse registerResponse = {};
  response.send(Pistache::Http::Code::Ok, Common::Models::toStr(registerResponse));
}

// NOLINTNEXTLINE(readability-convert-member-functions-to-static)
void AdminController::handleDeleteAccount(const Pistache::Rest::Request& request,
                                          Pistache::Http::ResponseWriter response) {
  Common::Models::DeleteAccountRequest deleteAccountRequest = nlohmann::json::parse(request.body());
  Rest::ZMQWorker::get()->updateRequest({Common::Functions::DeleteUser, Common::Models::toStr(deleteAccountRequest)});
  response.send(Pistache::Http::Code::Ok);
}

void AdminController::handleListeUsagers(const Pistache::Rest::Request& /*request*/,
                                         Pistache::Http::ResponseWriter response) {
  auto users = Rest::ZMQWorker::get()->getRequest({Common::Functions::GetAllUsers, ""});
  Common::Models::AllUsersResponse allUsersResponse = nlohmann::json::parse(users.data);
  response.send(Pistache::Http::Code::Ok, Common::Models::toStr(allUsersResponse));
}
}  // namespace Rest
