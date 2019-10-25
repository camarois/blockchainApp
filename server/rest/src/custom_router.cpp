#include <common/database.hpp>
#include <common/format_helper.hpp>
#include <ctime>
#include <future>
#include <gflags/gflags.h>
#include <rest/custom_router.hpp>
#include <common/logger.hpp>
#include <sstream>

DECLARE_string(db);

namespace Rest {

CustomRouter::CustomRouter() : Pistache::Rest::Router() {
  logSessionId_ = Common::Logger::init(FLAGS_db);
  std::cout << "Currently in session id: " << logSessionId_ << std::endl;
}

void CustomRouter::addRoute(Pistache::Http::Method method, const std::string& url,
                            Pistache::Rest::Route::Handler handler) {
  auto callback = [=](const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response) {
    try {
      log(url, request, logSessionId_);
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
  // Common::Database db(FLAGS_db);
  auto body = request.body().empty() ? "NULL" : request.body();
  // std::cout << std::endl << Common::FormatHelper::nowStr() << ": " << url << std::endl << body << std::endl;
  // db.addLog(0, 0, 0, url + "\n" + body, logSessionId);
  Common::Logger::log(0, 0, url + "\n" + body, logSessionId);
}

}  // namespace Rest
