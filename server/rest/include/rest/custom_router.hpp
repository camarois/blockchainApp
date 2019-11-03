#ifndef REST_CUSTOM_ROUTER_HPP
#define REST_CUSTOM_ROUTER_HPP

#include <memory>
#include <pistache/endpoint.h>
#include <pistache/http.h>
#include <pistache/router.h>

namespace Rest {

class CustomRouter : public Pistache::Rest::Router {
 public:
  CustomRouter();

  void addRoute(Pistache::Http::Method method, const std::string& url, const Pistache::Rest::Route::Handler& handler,
                bool requiresAuth = true);

  void get(const std::string& url, const Pistache::Rest::Route::Handler& handler, bool requiresAuth = true);
  void post(const std::string& url, const Pistache::Rest::Route::Handler& handler, bool requiresAuth = true);

 private:
  const std::string kDefaultBody_ = "NULL";
  const size_t kMaxPrintBody_ = 200;
};

}  // namespace Rest

#endif  // REST_CUSTOM_ROUTER_HPP
