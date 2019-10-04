#include <iostream>
#include <rest/main_controller.hpp>
#include <common/firebase_helper.hpp>
#include <string>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  try {
    auto selfIpAddress = FirebaseHelper::getSelfIpAddress();
    std::cout << "Running on: " << selfIpAddress << std::endl;
    std::future<void> future;
    if (argc > 1) {
      future = FirebaseHelper::setIpAddressAsync(selfIpAddress, argv[1]);
    } else {
      future = FirebaseHelper::setIpAddressAsync(selfIpAddress);
    }

    const int kPortNumber = 10000;
    const int kNbThreads = 4;
    Pistache::Port port(kPortNumber);
    Pistache::Address addr(Pistache::Ipv4::any(), port);

    MainController mainController(addr, kNbThreads);
    mainController.start();

    return 0;
  } catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;

    return 1;
  }
}
