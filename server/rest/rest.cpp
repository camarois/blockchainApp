#include "common/gflags_helper.hpp"
#include <common/firebase_helper.hpp>
#include <iostream>
#include <rest/main_controller.hpp>
#include <string>
#include <sys/types.h>
#include <unistd.h>

DEFINE_string(user, "server", "Developper using the service");  // NOLINT

int main(int argc, char* argv[]) {
  Common::GflagsHelper::init("Rest service", argc, argv);

  try {
    auto selfIpAddress = Common::FirebaseHelper::getSelfIpAddress();
    std::cout << "Running on: " << selfIpAddress << std::endl;
    std::future<void> future = Common::FirebaseHelper::setIpAddressAsync(selfIpAddress, FLAGS_user);

    const int kPortNumber = 10000;
    const int kNbThreads = 4;
    Pistache::Port port(kPortNumber);
    Pistache::Address addr(Pistache::Ipv4::any(), port);

    Rest::MainController mainController(addr, kNbThreads);
    mainController.start();

    return 0;
  } catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;

    return 1;
  }
}
