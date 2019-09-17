#include <pistache/http.h>
#include <pistache/router.h>
#include <pistache/endpoint.h>
#include <nlohmann/json.hpp>

#include <common/example.h>

using namespace std;
using namespace Pistache;
using json = nlohmann::json;

namespace Generic {

void handleReady(const Rest::Request&, Http::ResponseWriter response) {
    static int lol = 0;
    Example example;
    json j = {
        {"lol", lol++},
        {"example", example.example()}
    };
    response.send(Http::Code::Ok, j.dump(4));
}

}

class StatsEndpoint {
public:
    StatsEndpoint(Address addr)
        : httpEndpoint(std::make_shared<Http::Endpoint>(addr))
    { }

    void init(size_t thr = 2) {
        auto opts = Http::Endpoint::options()
            .threads(thr);
        httpEndpoint->init(opts);
        setupRoutes();
    }

    void start() {
        httpEndpoint->setHandler(router.handler());
        httpEndpoint->serve();
    }

private:
    void setupRoutes() {
        using namespace Rest;

        Routes::Get(router, "/lol", Routes::bind(&Generic::handleReady));

    }

    std::shared_ptr<Http::Endpoint> httpEndpoint;
    Rest::Router router;
};
