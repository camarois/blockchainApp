#include <common/database.hpp>
#include <common/format_helper.hpp>
#include <common/logger.hpp>
#include <common/token_helper.hpp>
#include <ctime>
#include <future>
#include <gflags/gflags.h>
#include <rest/custom_router.hpp>
#include <rest/zmq.hpp>
#include <sstream>

namespace Rest {

CustomRouter::CustomRouter(bool useSSL) { useSSL_ = useSSL; }

void CustomRouter::addRoute(Pistache::Http::Method method, const std::string& url,
                            const Pistache::Rest::Route::Handler& handler, bool requiresAuth) {
  auto callback = [=](const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response) {
    auto body = request.body().empty() ? kDefaultBody_ : request.body();
    body = body.length() > kMaxPrintBody_ ? body.substr(0, kMaxPrintBody_) + " [...]" : body;
    try {
      Common::Logger::get()->info(url + "\n" + body);
      if (requiresAuth && useSSL_) {
        std::string authHeader = request.headers().getRaw("Authorization").value();
        std::optional<std::string> optToken = Common::TokenHelper::decode(authHeader);
        if (optToken) {
          auto token = optToken.value();
          if (Common::TokenHelper::timedOut(authHeader)) {
            auto username = Common::TokenHelper::decodeUsername(token).value_or("");
            auto password = Common::TokenHelper::decodePassword(token).value_or("");
            Common::Models::GetSaltRequest getSaltRequest = {username};
            auto salt =
                Rest::ZMQWorker::get()->getRequest({Common::Functions::GetSalt, Common::Models::toStr(getSaltRequest)});
            Common::Models::ContainsUserRequest containsUserRequest = {{username, password}, salt.data};
            if (salt.found &&
                Rest::ZMQWorker::get()
                    ->getRequest({Common::Functions::ContainsUser, Common::Models::toStr(containsUserRequest)})
                    .found) {
              token = Common::TokenHelper::encode(username, password);
            }
            else {
              Common::Logger::get()->attention(url + "\n" + body + "\n" + authHeader + "\n" + "Invalid token.");
              response.send(Pistache::Http::Code::Forbidden);
            }
          }
          response.headers().add<Pistache::Http::Header::Authorization>(token);
          handler(request, std::move(response));
        }
        else {
          Common::Logger::get()->attention(url + "\n" + body + "\n" + authHeader + "\n" + "Invalid token.");
          response.send(Pistache::Http::Code::Forbidden);
        }
      }
      else {
        handler(request, std::move(response));
      }

      return Pistache::Rest::Route::Result::Ok;
    }
    catch (const nlohmann::json::exception& e) {
      Common::Logger::get()->error(url + "\n" + body + "\n" + e.what());
      response.send(Pistache::Http::Code::Bad_Request, e.what());

      return Pistache::Rest::Route::Result::Failure;
    }
    catch (const std::exception& e) {
      Common::Logger::get()->error(url + "\n" + body + "\n" + e.what());
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
