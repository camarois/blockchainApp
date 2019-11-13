#include <common/database.hpp>
#include <common/models.hpp>
#include <common/token_helper.hpp>
#include <gflags/gflags.h>
#include <rest/admin_controller.hpp>

DECLARE_string(db);

namespace Rest {

AdminController::AdminController(const std::shared_ptr<Rest::CustomRouter>& router,
                                 std::shared_ptr<ZMQWorker> zmqWorker)
    : zmqWorker_(std::move(zmqWorker)) {
  setupRoutes(router);
}

void AdminController::setupRoutes(const std::shared_ptr<Rest::CustomRouter>& router) {
  router->post(kBasePath_ + "login", Pistache::Rest::Routes::bind(&AdminController::handleLogin, this), false);
  router->post(kBasePath_ + "logout", Pistache::Rest::Routes::bind(&AdminController::handleLogout, this));
  router->post(kBasePath_ + "motdepasse", Pistache::Rest::Routes::bind(&AdminController::handlePassword, this));
  router->post(kBasePath_ + "chaine/" + kId_, Pistache::Rest::Routes::bind(&AdminController::handleChain, this));
  router->post(kBasePath_ + "logs/" + kId_, Pistache::Rest::Routes::bind(&AdminController::handleLogs, this));
  router->get(kBasePath_ + "allUsers", Pistache::Rest::Routes::bind(&AdminController::handleAllUsers, this));
  router->post(kBasePath_ + "creationcompte",
               Pistache::Rest::Routes::bind(&AdminController::handleCreateAccount, this));
  router->post(kBasePath_ + "suppressioncompte",
               Pistache::Rest::Routes::bind(&AdminController::handleDeleteAccount, this));
}

void AdminController::handleLogin(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response) {
  Common::Models::LoginRequest loginRequest = nlohmann::json::parse(request.body());
  auto salt = zmqWorker_->getRequest({Common::Functions::GetSalt, loginRequest.username});
  Common::Models::ContainsAdminRequest containsAdminRequest = {loginRequest, salt.data, true};
  if (salt.found &&
      zmqWorker_->getRequest({Common::Functions::ContainsAdmin, Common::Models::toStr(containsAdminRequest)}).found) {
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

  auto salt = zmqWorker_->getRequest({Common::Functions::GetSalt, {loginRequest.username}});
  Common::Models::ContainsAdminRequest containsAdminRequest = {loginRequest, salt.data, true};
  if (salt.found &&
      zmqWorker_->getRequest({Common::Functions::ContainsUser, Common::Models::toStr(containsAdminRequest)}).found) {
    Common::Models::SetUserPasswordRequest setUserPasswordRequest = {loginRequest.username, passwordRequest, salt.data,
                                                                     true};
    zmqWorker_->updateRequest({Common::Functions::SetUserPassword, Common::Models::toStr(containsAdminRequest)});
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
  Common::Database db(FLAGS_db);

  int provenance = request.param(kId_).as<std::string>() != "serveurweb" ? request.param(kId_).as<int>() : 0;
  std::vector<Common::Models::Information> logs;
  logs = db.getLogs(logsRequest.last, provenance);
  Common::Models::LogsResponse logsResponse = {{{logs}}};
  response.send(Pistache::Http::Code::Ok, Common::Models::toStr(logsResponse));
}

// NOLINTNEXTLINE(readability-convert-member-functions-to-static)
void AdminController::handleAllUsers(const Pistache::Rest::Request& /*request*/, Pistache::Http::ResponseWriter response) {
  auto result = zmqWorker_->getRequest({Common::Functions::GetAllUsers, ""});
  std::vector<Common::Models::User> users = nlohmann::json::parse(result.data);
  Common::Models::AllUsersResponse allUsersResponse = {users};
  response.send(Pistache::Http::Code::Ok, Common::Models::toStr(allUsersResponse));
}

// NOLINTNEXTLINE(readability-convert-member-functions-to-static)
void AdminController::handleCreateAccount(const Pistache::Rest::Request& request,
                                          Pistache::Http::ResponseWriter response) {
  Common::Models::AddUserRequest addUserRequest = nlohmann::json::parse(request.body());
  zmqWorker_->updateRequest({Common::Functions::AddUser, Common::Models::toStr(addUserRequest)});
  response.send(Pistache::Http::Code::Ok);
}

// NOLINTNEXTLINE(readability-convert-member-functions-to-static)
void AdminController::handleDeleteAccount(const Pistache::Rest::Request& request,
                                          Pistache::Http::ResponseWriter response) {
  Common::Models::DeleteAccountRequest deleteAccountRequest = nlohmann::json::parse(request.body());
  zmqWorker_->updateRequest({Common::Functions::DeleteUser, Common::Models::toStr(deleteAccountRequest)});
  response.send(Pistache::Http::Code::Ok);
}

}  // namespace Rest
