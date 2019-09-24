/*
   Mathieu Stefani, 07 février 2016

   Example of a REST endpoint with routing
*/

#include <algorithm>
#include <nlohmann/json.hpp>
#include <pistache/endpoint.h>
#include <pistache/http.h>
#include <pistache/router.h>

using namespace Pistache;
using json = nlohmann::json;

namespace generic {

void handleReady(const Rest::Request& /*request*/, Http::ResponseWriter response) {
  static int lol = 0;
  json j = {{"lol", lol++}, {"bonjour", "allo"}};
  response.send(Http::Code::Ok, j.dump(4));
}

}  // namespace generic

class StatsEndpoint {
 public:
  explicit StatsEndpoint(Address addr) : httpEndpoint_(std::make_shared<Http::Endpoint>(addr)) {}

  void init(size_t thr) {
    auto opts = Http::Endpoint::options().threads(thr);
    httpEndpoint_->init(opts);
    setupRoutes();
  }

  void start() {
    httpEndpoint_->setHandler(router_.handler());
    httpEndpoint_->serve();
  }

 private:
  void setupRoutes() {
    using namespace Rest;

    Routes::Get(router_, "/lol", Routes::bind(&generic::handleReady));
  }

  std::shared_ptr<Http::Endpoint> httpEndpoint_;
  Rest::Router router_;
};

int main(int argc, char* argv[]) {
  const int kDefaultPort = 10000;
  Port port(kDefaultPort);

  int thr = 2;

  if (argc >= 2) {
    port = std::stoi(argv[1]);

    if (argc == 3) {
      thr = std::stoi(argv[2]);
    }
  }

  Address addr(Ipv4::any(), port);

  std::cout << "Cores = " << hardware_concurrency() << std::endl;
  std::cout << "Using " << thr << " threads" << std::endl;

  StatsEndpoint stats(addr);

  stats.init(thr);
  stats.start();
}
