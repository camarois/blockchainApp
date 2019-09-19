#pragma once

#include <pistache/http.h>
#include <pistache/router.h>
#include <pistache/endpoint.h>
#include <nlohmann/json.hpp>

#include <common/example.hpp>

using namespace std;
using namespace Pistache;
using json = nlohmann::json;

class StatsEndpoint {
public:
    StatsEndpoint(Address addr)
        : httpEndpoint(std::make_shared<Http::Endpoint>(addr))
    { }

    void init(size_t thr);
    void start();

private:
    void setupRoutes();
    static void handleReady(const Rest::Request&, Http::ResponseWriter response);

    std::shared_ptr<Http::Endpoint> httpEndpoint;
    Rest::Router router;
};
