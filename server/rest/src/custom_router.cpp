#include <common/database.hpp>
#include <common/format_helper.hpp>
#include <common/logger.hpp>
#include <common/token_helper.hpp>
#include <ctime>
#include <future>
#include <gflags/gflags.h>
#include <rest/custom_router.hpp>
#include <sstream>

DECLARE_string(db);

namespace Rest {

CustomRouter::CustomRouter() { Common::Logger::init(FLAGS_db); }

void CustomRouter::addRoute(Pistache::Http::Method method, const std::string& url,
                            const Pistache::Rest::Route::Handler& handler, bool requiresAuth) {
  auto callback = [=](const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response) {
    response.timeoutAfter(std::chrono::seconds(30));
    auto body = request.body().empty() ? kDefaultBody_ : request.body();
    body = body.length() > kMaxPrintBody_ ? body.substr(0, kMaxPrintBody_) + " [...]" : body;
    try {
      if (requiresAuth) {
        std::string authHeader = request.headers().getRaw("Authorization").value();
        std::optional<std::string> token = Common::TokenHelper::decode(authHeader, FLAGS_db);
        if (token) {
          response.headers().add<Pistache::Http::Header::Authorization>(token.value());
          handler(request, std::move(response));
          Common::Logger::get()->info(0, url + "\n" + body);
        } else {
          Common::Logger::get()->attention(0, url + "\n" + body + "\n" + authHeader + "\n" + "Invalid token.");
          response.send(Pistache::Http::Code::Forbidden);
        }
      } else {
        handler(request, std::move(response));
        Common::Logger::get()->info(0, url + "\n" + body);
      }

      return Pistache::Rest::Route::Result::Ok;
    } catch (const nlohmann::json::exception& e) {
      Common::Logger::get()->error(0, url + "\n" + body + "\n" + e.what());
      response.send(Pistache::Http::Code::Bad_Request, e.what());

      return Pistache::Rest::Route::Result::Failure;
    } catch (const std::exception& e) {
      Common::Logger::get()->error(0, url + "\n" + body + "\n" + e.what());
      response.send(Pistache::Http::Code::Internal_Server_Error, e.what());

      return Pistache::Rest::Route::Result::Failure;
    }
  };
  Pistache::Rest::Router::addRoute(method, url, callback);
}

void CustomRouter::get(const std::string& url, const Pistache::Rest::Route::Handler& handler, bool requiresAuth) {
  CustomRouter::addRoute(Pistache::Http::Method::Get, url, handler, requiresAuth);
}

void CustomRouter::post(const std::string& url, const Pistache::Rest::Route::Handler& handler, bool requiresAuth) {
  CustomRouter::addRoute(Pistache::Http::Method::Post, url, handler, requiresAuth);
}

}  // namespace Rest
