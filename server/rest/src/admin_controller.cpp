#include <common/database.hpp>
#include <common/models.hpp>
#include <gflags/gflags.h>
#include <rest/admin_controller.hpp>

DECLARE_string(db);

namespace Rest {

AdminController::AdminController(const std::shared_ptr<Rest::CustomRouter>& router) { setupRoutes(router); }

void AdminController::setupRoutes(const std::shared_ptr<Rest::CustomRouter>& router) {
  router->post(kBasePath_ + "login", Pistache::Rest::Routes::bind(&AdminController::handleLogin, this));
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
  response.send(Pistache::Http::Code::I_m_a_teapot, "TODO");
}

void AdminController::handleLogout(const Pistache::Rest::Request& /*request*/,
                                   Pistache::Http::ResponseWriter response) {
  response.send(Pistache::Http::Code::I_m_a_teapot, "TODO");
}

void AdminController::handlePassword(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response) {
  Common::Models::PasswordRequest passwordRequest = nlohmann::json::parse(request.body());
  response.send(Pistache::Http::Code::I_m_a_teapot, "TODO");
}

void AdminController::handleChain(const Pistache::Rest::Request& /*request*/, Pistache::Http::ResponseWriter response) {
  // auto miner = request.param(kId_).as<int>();
  // Common::Models::ChainRequest chainRequest = nlohmann::json::parse(request.body());
  Common::Models::ChainResponse chainResponse = {"test", std::make_unique<Common::Models::ChainResponse>()};
  response.send(Pistache::Http::Code::I_m_a_teapot, Common::Models::toStr(chainResponse));
}

void AdminController::handleLogs(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response) {
  Common::Models::LogsRequest logsRequest = nlohmann::json::parse(request.body());
  Common::Database db(FLAGS_db);

  int provenance = request.param(kId_).as<std::string>().compare("serveurweb") ? request.param(kId_).as<int>() : 0;
  std::vector<Common::Models::Information> logs;
  logs = db.getLogs(logsRequest.last, provenance);
  Common::Models::LogsResponse logsResponse = {{{logs}}};
  response.send(Pistache::Http::Code::Ok, Common::Models::toStr(logsResponse));
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
