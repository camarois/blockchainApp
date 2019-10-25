#include <common/database.hpp>
#include <common/format_helper.hpp>
#include <ctime>
#include <future>
#include <gflags/gflags.h>
#include <rest/custom_router.hpp>
#include <sstream>

DECLARE_string(db);

namespace Rest {

CustomRouter::CustomRouter() : Pistache::Rest::Router() {
  Common::Database db(FLAGS_db);
  logSessionId_ = db.addLogSession();
  std::cout << "Currently in session id: " << logSessionId_ << std::endl;
}

void CustomRouter::addRoute(Pistache::Http::Method method, const std::string& url,
                            Pistache::Rest::Route::Handler handler) {
  auto callback = [&](const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response) {
    try {
      logAsync(url, request, logSessionId_);
      std::cout << "af" << std::endl;
      handler(request, std::move(response));
      return Pistache::Rest::Route::Result::Ok;
    } catch (const std::exception& e) {
      std::cerr << e.what() << std::endl;
      response.send(Pistache::Http::Code::Internal_Server_Error, e.what());
      return Pistache::Rest::Route::Result::Failure;
    }
  };
  Pistache::Rest::Router::addRoute(method, url, callback);
}

void CustomRouter::get(const std::string& url, Pistache::Rest::Route::Handler handler) {
  CustomRouter::addRoute(Pistache::Http::Method::Get, url, std::move(handler));
}

void CustomRouter::post(const std::string& url, Pistache::Rest::Route::Handler handler) {
  CustomRouter::addRoute(Pistache::Http::Method::Post, url, std::move(handler));
}

void CustomRouter::log(const std::string& url, const Pistache::Rest::Request& request, int logSessionId) {
  std::cout << "salut" << std::endl;
  Common::Database db(FLAGS_db);
  std::stringstream ss;
  std::cout << "allo" << std::endl;
  auto body = request.body() == "" ? "NULL" : request.body();
  std::cout << "yio" << std::endl;
  ss << std::endl << Common::FormatHelper::nowStr() << ": " << url << std::endl << body;
  std::cout << "lol" << std::endl;
  auto logStr = ss.str();
  std::cout << logStr << std::endl;
  db.addLog(logSessionId, logStr);
}

void CustomRouter::logAsync(const std::string& url, const Pistache::Rest::Request& request, int logSessionId) {
  std::cout << "bef" << std::endl;
  // std::thread(log, url, request, logSessionId).detach();
  std::thread([=]() {
    std::cout << "salut" << std::endl;
    Common::Database db(FLAGS_db);
    std::stringstream ss;
    std::cout << "allo" << std::endl;
    auto body = request.body() == "" ? "NULL" : request.body();
    std::cout << "yio" << std::endl;
    ss << std::endl << Common::FormatHelper::nowStr() << ": " << url << std::endl << body;
    std::cout << "lol" << std::endl;
    auto logStr = ss.str();
    std::cout << logStr << std::endl;
    db.addLog(logSessionId, logStr);
  });

  std::cout << "af" << std::endl;
}

}  // namespace Rest
