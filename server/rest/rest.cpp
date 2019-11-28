#include "common/gflags_helper.hpp"
#include <common/firebase_helper.hpp>
#include <common/logger.hpp>
#include <common/scripts_helper.hpp>
#include <iostream>
#include <rest/main_controller.hpp>
#include <rest/zmq.hpp>
#include <string>
#include <sys/types.h>
#include <unistd.h>

DEFINE_string(user, "server", "Developper using the service");            // NOLINT
DEFINE_string(cert, "server.crt", "Path to server cert");                 // NOLINT
DEFINE_string(key, "server.key", "Path to server key");                   // NOLINT
DEFINE_string(db, "rest.db", "Path to sqlite db file");                   // NOLINT
DEFINE_int32(port, 8080, "REST http port");                               // NOLINT
DEFINE_int32(threads, 4, "Number of threads");                            // NOLINT
DEFINE_string(transactions, "transactions/", "Path to transactions");     // NOLINT
DEFINE_int32(buffer_size, 4000000, "Maximum number of bytes in buffer");  // NOLINT
DEFINE_int32(timeout, 100, "Maximum number of seconds before timeout");   // NOLINT

int main(int argc, char* argv[]) {
  Common::GflagsHelper::init("Rest service", argc, argv);

  try {
    Common::Database::init(FLAGS_db);
    Rest::ZMQWorker::init("tcp://*");
    auto selfIpAddress = Common::FirebaseHelper::getSelfIpAddress();
    Common::Logger::get()->info("Running on: " + selfIpAddress + "\n");
    Common::ScriptsHelper::createCert(selfIpAddress);

    Pistache::Port port(FLAGS_port);
    Pistache::Address addr(Pistache::Ipv4::any(), port);

    Rest::MainController mainController(addr, FLAGS_threads);
    Common::FirebaseHelper::setIpAddressAsync(selfIpAddress, FLAGS_user);
    Rest::ZMQWorker::get()->start();
    mainController.start();

    return 0;
  }
  catch (const std::exception& e) {
    Common::Logger::get()->error(e.what());

    return 1;
  }
}
