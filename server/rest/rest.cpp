#include <common/firebase_helper.hpp>
#include <gflags/gflags.h>
#include <iostream>
#include <rest/main_controller.hpp>
#include <string>
#include <sys/types.h>
#include <unistd.h>

DEFINE_string(user, "server", "Developper using the service");  // NOLINT

int main(int argc, char* argv[]) {
  for (int i = 0; i < argc; i++) {
    if (strcmp("--help", argv[i]) == 0) {
      argv[i] = "--helpshort";
    }
  }

  // TODO put in method in common.
  GFLAGS_NAMESPACE::SetUsageMessage("Rest service");		 // NOLINT
  GFLAGS_NAMESPACE::ParseCommandLineFlags(&argc, &argv, false);  // NOLINT

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
