#include <chrono>
#include <ctime>
#include <rest/custom_router.hpp>

namespace Rest {

CustomRouter::CustomRouter() : Pistache::Rest::Router() {}

void CustomRouter::addRoute(Pistache::Http::Method method, const std::string& url,
                            Pistache::Rest::Route::Handler handler) {
  auto callback = [=](const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response) {
    log(url, request);
    handler(request, std::move(response));
    return Pistache::Rest::Route::Result::Ok;
    ;
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
  auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
  std::string nowStr = std::ctime(&now);
  auto body = request.body() == "" ? "NULL" : request.body();
  std::cout << std::endl << nowStr.substr(0, nowStr.length() - 1) << ": " << url << std::endl << body << std::endl;
}

}  // namespace Rest
