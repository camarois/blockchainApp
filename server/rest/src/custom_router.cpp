#include <common/database.hpp>
#include <common/format_helper.hpp>
#include <ctime>
#include <gflags/gflags.h>
#include <rest/custom_router.hpp>

DECLARE_string(db);

namespace Rest {

CustomRouter::CustomRouter() : Pistache::Rest::Router() {}

void CustomRouter::addRoute(Pistache::Http::Method method, const std::string& url,
                            Pistache::Rest::Route::Handler handler) {
  auto callback = [=](const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response) {
    // Common::Database db(FLAGS_db);
    // Common::Models::LoginRequest expectedUser = {url, "LOL1234!"};

    // db.createUser(expectedUser);

    // auto receivedUser = db.getUser(expectedUser.username);
    log(url, request);
    handler(request, std::move(response));
    return Pistache::Rest::Route::Result::Ok;
  };
  Pistache::Rest::Router::addRoute(method, url, callback);
}

void CustomRouter::get(const std::string& url, Pistache::Rest::Route::Handler handler) {
  CustomRouter::addRoute(Pistache::Http::Method::Get, url, std::move(handler));
}

void CustomRouter::post(const std::string& url, Pistache::Rest::Route::Handler handler) {
  CustomRouter::addRoute(Pistache::Http::Method::Post, url, std::move(handler));
}

void CustomRouter::log(const std::string& url, const Pistache::Rest::Request& request) {
  auto body = request.body() == "" ? "NULL" : request.body();
  std::cout << std::endl << Common::FormatHelper::nowStr() << ": " << url << std::endl << body << std::endl;
}

}  // namespace Rest
