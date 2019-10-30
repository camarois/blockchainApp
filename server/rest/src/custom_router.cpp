#include <common/database.hpp>
#include <common/format_helper.hpp>
#include <common/logger.hpp>
#include <common/token_helper.hpp>
#include <ctime>
#include <future>
#include <gflags/gflags.h>
#include <rest/custom_router.hpp>
#include <sstream>

namespace Rest {

CustomRouter::CustomRouter() : Pistache::Rest::Router() { Common::Logger::init(); }

void CustomRouter::addRoute(Pistache::Http::Method method, const std::string& url,
                            Pistache::Rest::Route::Handler handler) {
  auto callback = [=](const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response) {
    auto body = request.body().empty() ? "NULL" : request.body();
    std::string authHeader = request.headers().getRaw("Authorization").value();
    std::optional<std::string> token = Common::TokenHelper::decode(&authHeader);

    try {
      if (token) {
        response.headers().add<Pistache::Http::Header::Authorization>(token.value());
        handler(request, std::move(response));
      } else {
        response.send(Pistache::Http::Code::Forbidden, "Invalid token.");
      }
      Common::Logger::get()->info(0, url + "\n" + body);

      return Pistache::Rest::Route::Result::Ok;
    } catch (const std::exception& e) {
      Common::Logger::get()->error(0, url + "\n" + body + "\n" + e.what());
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

}  // namespace Rest
