#include <common/models.hpp>
#include <rest/admin_controller.hpp>

namespace Rest {

AdminController::AdminController(const std::shared_ptr<Pistache::Rest::Router>& router) {
  setupRoutes(router);
}

void AdminController::setupRoutes(const std::shared_ptr<Pistache::Rest::Router>& router) {
  Pistache::Rest::Routes::Post(*router, kBasePath_ + "login",
                               Pistache::Rest::Routes::bind(&AdminController::handleLogin, this));
  Pistache::Rest::Routes::Post(*router, kBasePath_ + "logout",
                               Pistache::Rest::Routes::bind(&AdminController::handleLogout, this));
  Pistache::Rest::Routes::Post(
      *router, kBasePath_ + "motdepasse",
      Pistache::Rest::Routes::bind(&AdminController::handlePassword, this));
  Pistache::Rest::Routes::Post(*router, kBasePath_ + "chaine/" + kId_,
                               Pistache::Rest::Routes::bind(&AdminController::handleChain, this));
  Pistache::Rest::Routes::Post(*router, kBasePath_ + "logs/" + kId_,
                               Pistache::Rest::Routes::bind(&AdminController::handleLogs, this));
  Pistache::Rest::Routes::Post(
      *router, kBasePath_ + "creationcompte",
      Pistache::Rest::Routes::bind(&AdminController::handleCreateAccount, this));
  Pistache::Rest::Routes::Post(
      *router, kBasePath_ + "suppressioncompte",
      Pistache::Rest::Routes::bind(&AdminController::handleDeleteAccount, this));
}

void AdminController::handleLogin(const Pistache::Rest::Request& request,
                                  Pistache::Http::ResponseWriter response) {
  Common::Models::LoginRequest loginRequest = nlohmann::json::parse(request.body());
  response.send(Pistache::Http::Code::I_m_a_teapot, "TODO");
}

void AdminController::handleLogout(const Pistache::Rest::Request& /*request*/,
                                   Pistache::Http::ResponseWriter response) {
  response.send(Pistache::Http::Code::I_m_a_teapot, "TODO");
}

void AdminController::handlePassword(const Pistache::Rest::Request& request,
                                     Pistache::Http::ResponseWriter response) {
  Common::Models::PasswordRequest passwordRequest = nlohmann::json::parse(request.body());
  response.send(Pistache::Http::Code::I_m_a_teapot, "TODO");
}

void AdminController::handleChain(const Pistache::Rest::Request& /*request*/,
                                  Pistache::Http::ResponseWriter response) {
  // auto miner = request.param(kId_).as<int>();
  // Common::Models::ChainRequest chainRequest = nlohmann::json::parse(request.body());
  Common::Models::ChainResponse chainResponse = {"test",
                                                 std::make_unique<Common::Models::ChainResponse>()};
  response.send(Pistache::Http::Code::I_m_a_teapot,
                static_cast<nlohmann::json>(chainResponse).dump(4));
}

void AdminController::handleLogs(const Pistache::Rest::Request& /*request*/,
                                 Pistache::Http::ResponseWriter response) {
  // auto miner = request.param(kId_).as<std::string>();
  // Common::Models::LogsRequest logsRequest = nlohmann::json::parse(request.body());
  Common::Models::LogsResponse logsResponse = {{{}}};
  response.send(Pistache::Http::Code::I_m_a_teapot,
                static_cast<nlohmann::json>(logsResponse).dump(4));
}

void AdminController::handleCreateAccount(const Pistache::Rest::Request& request,
                                          Pistache::Http::ResponseWriter response) {
  Common::Models::CreateAccountRequest logsRequest = nlohmann::json::parse(request.body());
  response.send(Pistache::Http::Code::I_m_a_teapot, "TODO");
}

void AdminController::handleDeleteAccount(const Pistache::Rest::Request& request,
                                          Pistache::Http::ResponseWriter response) {
  Common::Models::DeleteAccountRequest logsRequest = nlohmann::json::parse(request.body());
  response.send(Pistache::Http::Code::I_m_a_teapot, "TODO");
}

}  // namespace Rest
