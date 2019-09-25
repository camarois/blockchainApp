#include <example_endpoint.hpp>
#include <iostream>
#include <string>
#include <unistd.h>

int main() {
  try {
    const int kNbThreads = 4;
    const int kPortNumber = 10000;
    Pistache::Port port(kPortNumber);
    Pistache::Address addr(Pistache::Ipv4::any(), port);

    std::cout << "Number of cores: " << Pistache::hardware_concurrency() << std::endl;
    std::cout << "Number of threads: " << kNbThreads << std::endl;
    std::cout << "Running on port: " << kPortNumber << std::endl;

    ExampleEndpoint stats(addr);
    stats.init(kNbThreads);
    stats.start();

    return 0;
  } catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;

    return 1;
  }
}
