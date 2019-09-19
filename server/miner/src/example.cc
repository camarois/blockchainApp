// #include <example.h>

// using namespace std;
// using namespace Pistache;
// using json = nlohmann::json;

// void StatsEndpoint::init(size_t thr = 2) {
//     auto opts = Http::Endpoint::options()
//         .threads(thr);
//     httpEndpoint->init(opts);
//     setupRoutes();
// }

// void StatsEndpoint::start() {
//     httpEndpoint->setHandler(router.handler());
//     httpEndpoint->serve();
// }

// void StatsEndpoint::setupRoutes() {
//     using namespace Rest;
//     Routes::Get(router, "/lol", Routes::bind(&StatsEndpoint::handleReady));
// }

// void StatsEndpoint::handleReady(const Rest::Request&, Http::ResponseWriter response) {
//     static int lol = 0;
//     Example example;
//     json j = {
//         {"lol", lol++},
//         {"example", example.example()}
//     };
//     response.send(Http::Code::Ok, j.dump(4));
// }
