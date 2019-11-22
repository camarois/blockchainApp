#ifndef REST_INFO_CONTROLLER_HPP
#define REST_INFO_CONTROLLER_HPP

#include <nlohmann/json.hpp>
#include <pistache/endpoint.h>
#include <pistache/http.h>
#include <pistache/router.h>
#include <rest/custom_router.hpp>
#include <rest/zmq.hpp>

namespace Rest {

class InfoController {
 public:
  explicit InfoController(const std::shared_ptr<Rest::CustomRouter>& router);

 private:
  void setupRoutes(const std::shared_ptr<Rest::CustomRouter>& router);
  void handleClasses(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response);
  void handleStudents(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response);
  void handleListStudents(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response);
  void handleListClasses(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response);

  const std::string kBasePath_ = "/info/";
};

}  // namespace Rest

#endif  // REST_INFO_CONTROLLER_HPP
