#ifndef REST_FILE_CONTROLLER_HPP
#define REST_FILE_CONTROLLER_HPP

#include <nlohmann/json.hpp>
#include <pistache/endpoint.h>
#include <pistache/http.h>
#include <pistache/router.h>

#include <common/example.hpp>

namespace Rest {

class FileController {
 public:
  explicit FileController(const std::shared_ptr<Pistache::Rest::Router>& router);

 private:
  void setupRoutes(const std::shared_ptr<Pistache::Rest::Router>& router);
  void handleGrades(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response);

  const std::string kBasePath = "/fichier/";
};

}  // namespace Rest

#endif  // REST_FILE_CONTROLLER_HPP
