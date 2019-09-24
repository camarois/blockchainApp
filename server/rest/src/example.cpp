#include <example.hpp>

using json = nlohmann::json;

void StatsEndpoint::init(size_t thr = 2) {
  auto opts = Pistache::Http::Endpoint::options().threads(thr);
  httpEndpoint->init(opts);
  setupRoutes();
}

void StatsEndpoint::start() {
  httpEndpoint->setHandler(router.handler());
  httpEndpoint->serve();
}

void StatsEndpoint::setupRoutes() {
  Pistache::Rest::Routes::Get(router, "/lol",
			      Pistache::Rest::Routes::bind(&StatsEndpoint::handleReady));
}

void StatsEndpoint::handleReady(const Pistache::Rest::Request& /*unused*/,
				Pistache::Http::ResponseWriter response) {
  static int lol = 0;
  Example example;
  json j = {{"lol", lol++}, {"example", example.example()}};
  response.send(Pistache::Http::Code::Ok, j.dump(4));
}
