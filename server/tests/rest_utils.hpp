#ifndef REST_UTILS_HPP
#define REST_UTILS_HPP

#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <gflags/gflags.h>
#include <sstream>
#include <string>

DEFINE_string(user, "server", "Developper using the service");            // NOLINT
DEFINE_string(cert, "server.crt", "Path to server cert");                 // NOLINT
DEFINE_string(key, "server.key", "Path to server key");                   // NOLINT
DEFINE_string(db, "rest.db", "Path to sqlite db file");                   // NOLINT
DEFINE_int32(port, 8081, "REST http port");                               // NOLINT
DEFINE_int32(threads, 1, "Number of threads");                            // NOLINT
DEFINE_string(transactions, "transactions/", "Path to transactions");     // NOLINT
DEFINE_int32(buffer_size, 1000000, "Maximum number of bytes in buffer");  // NOLINT
DEFINE_int32(timeout, 1, "Maximum number of seconds before timeout");     // NOLINT

const char* zmqNotInit = "ZMQWorker not created, you should initilize it first";

inline std::string postRequest(const std::string& url, const std::string& body) {
  curlpp::Easy request;
  request.setOpt(new curlpp::options::Url("http://localhost:8081" + url));
  request.setOpt(new curlpp::options::Verbose(true));
  std::list<std::string> header;
  header.push_back("Content-Type: application/json");
  request.setOpt(new curlpp::options::HttpHeader(header));

  request.setOpt(new curlpp::options::PostFields(body));
  request.setOpt(new curlpp::options::PostFieldSize(body.size() + 1));
  std::ostringstream response;
  request.setOpt(new curlpp::options::WriteStream(&response));

  request.perform();

  return std::string(response.str());
}

inline std::string getRequest(const std::string& url) {
  auto resp = curlpp::options::Url("http://localhost:8081" + url);
  std::stringstream ss;
  ss << resp;
  return ss.str();
}

#endif  // REST_UTILS_HPP
