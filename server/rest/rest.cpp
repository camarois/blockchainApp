#include "common/gflags_helper.hpp"
#include <common/firebase_helper.hpp>
#include <common/logger.hpp>
#include <common/scripts_helper.hpp>
#include <iostream>
#include <rest/main_controller.hpp>
#include <string>
#include <sys/types.h>
#include <unistd.h>

DEFINE_string(user, "server", "Developper using the service");  // NOLINT
DEFINE_string(cert, "server.crt", "Path to server cert");       // NOLINT
DEFINE_string(key, "server.key", "Path to server key");         // NOLINT
DEFINE_string(db, "blockchain.db", "Path to sqlite db file");   // NOLINT
DEFINE_int32(port, 10000, "REST http port");                    // NOLINT
DEFINE_int32(threads, 4, "Number of threads");                  // NOLINT

int main(int argc, char* argv[]) {
  Common::GflagsHelper::init("Rest service", argc, argv);

  try {
    auto selfIpAddress = Common::FirebaseHelper::getSelfIpAddress();
    std::cout << "Running on: " << selfIpAddress << std::endl;
    Common::ScriptsHelper::createCert(selfIpAddress, FLAGS_db);

    Pistache::Port port(FLAGS_port);
    Pistache::Address addr(Pistache::Ipv4::any(), port);

    Rest::MainController mainController(addr, FLAGS_threads);
    Common::FirebaseHelper::setIpAddressAsync(selfIpAddress, FLAGS_user);
    mainController.start();

    return 0;
  } catch (const std::exception& e) {
    Common::Logger::get()->error(0, e.what());

    return 1;
  }
}
