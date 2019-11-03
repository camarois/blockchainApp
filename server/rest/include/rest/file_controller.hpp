#ifndef REST_FILE_CONTROLLER_HPP
#define REST_FILE_CONTROLLER_HPP

#include <nlohmann/json.hpp>
#include <pistache/endpoint.h>
#include <pistache/http.h>
#include <pistache/router.h>
#include <rest/custom_router.hpp>

namespace Rest {

class FileController {
 public:
  explicit FileController(const std::shared_ptr<Rest::CustomRouter>& router);

 private:
  void setupRoutes(const std::shared_ptr<Rest::CustomRouter>& router);
  void handleGrades(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response);

  const std::string kBasePath_ = "/fichier/";
};

}  // namespace Rest

#endif  // REST_FILE_CONTROLLER_HPP
