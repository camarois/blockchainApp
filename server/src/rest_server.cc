/*
   Mathieu Stefani, 07 février 2016

   Example of a REST endpoint with routing
*/

#include <algorithm>

#include <pistache/http.h>
#include <pistache/router.h>
#include <pistache/endpoint.h>
#include <nlohmann/json.hpp>

using namespace std;
using namespace Pistache;
using json = nlohmann::json;

namespace Generic {

void handleReady(const Rest::Request&, Http::ResponseWriter response) {
    static int lol = 0;
    json j = {
        {"lol", lol++}
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

int main(int argc, char *argv[]) {
    Port port(9080);

    int thr = 2;

    if (argc >= 2) {
        port = std::stol(argv[1]);

        if (argc == 3)
            thr = std::stol(argv[2]);
    }

    Address addr(Ipv4::any(), port);

    cout << "Cores = " << hardware_concurrency() << endl;
    cout << "Using " << thr << " threads" << endl;

    StatsEndpoint stats(addr);

    stats.init(thr);
    stats.start();
}
