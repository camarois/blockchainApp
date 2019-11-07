#include <common/base64.hpp>
#include <common/format_helper.hpp>
#include <common/models.hpp>
#include <fstream>
#include <gflags/gflags.h>
#include <rest/file_controller.hpp>
#include <streambuf>
#include <string>

DECLARE_string(transactions);

namespace Rest {

FileController::FileController(const std::shared_ptr<Rest::CustomRouter>& router) { setupRoutes(router); }

void FileController::setupRoutes(const std::shared_ptr<Rest::CustomRouter>& router) {
  router->post(kBasePath_ + "notes", Pistache::Rest::Routes::bind(&FileController::handleGrades, this));
}

void FileController::handleGrades(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response) {
  Common::Models::GradesRequest gradesRequest = nlohmann::json::parse(request.body());
  std::ifstream file(FLAGS_transactions + std::to_string(gradesRequest.trimester) + "-" +
                         Common::FormatHelper::toLower(gradesRequest.acronym) + ".pdf",
                     std::ios::binary);
  std::vector<unsigned char> buffer;
  file.unsetf(std::ios::skipws);
  std::streampos fileSize;
  file.seekg(0, std::ios::end);
  fileSize = file.tellg();
  file.seekg(0, std::ios::beg);
  buffer.reserve(fileSize);
  buffer.insert(buffer.begin(), std::istream_iterator<unsigned char>(file), std::istream_iterator<unsigned char>());

  response.send(Pistache::Http::Code::Ok, Common::Base64::encode(buffer));
}

}  // namespace Rest
