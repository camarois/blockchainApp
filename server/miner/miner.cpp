#include "common/firebase_helper.hpp"
#include "common/message_helper.hpp"
#include <gflags/gflags.h>
#include <iostream>
#include <string>
#include <unistd.h>
#include <zmq.hpp>

DEFINE_string(addr, "", "REST service address");		// NOLINT
DEFINE_string(user, "server", "Developper using the service");  // NOLINT
DEFINE_int32(port, 5555, "REST service port");			// NOLINT

int main(int argc, char* argv[]) {
  for (int i = 0; i < argc; i++) {
    if (strcmp("--help", argv[i]) == 0) {
      argv[i] = "--helpshort";
    }
  }

  GFLAGS_NAMESPACE::SetUsageMessage("Blockchain miner service");  // NOLINT
  GFLAGS_NAMESPACE::ParseCommandLineFlags(&argc, &argv, false);   // NOLINT

  std::string addr = FLAGS_addr;
  if (addr.empty()) {
    addr = "tcp://" + Common::FirebaseHelper::getServerIpAddress(FLAGS_user) + ":" +
	   std::to_string(FLAGS_port);
  }
  std::cout << "Server ip address: " << addr << std::endl;

  try {
    zmq::context_t context(1);
    zmq::socket_t socket(context, ZMQ_REQ);
    socket.connect(addr);

    while (true) {
      std::cout << "Sending " << std::flush;
      auto request = Common::MessageHelper::fromString("from miner");
      socket.send(request, zmq::send_flags::none);
      std::cout << "Sent " << std::flush;

      zmq::message_t reply;
      std::cout << "Receving " << std::flush;
      socket.recv(reply, zmq::recv_flags::none);
      auto str = Common::MessageHelper::toString(reply);
      std::cout << str << std::endl;
    }
    return 0;
  } catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }
}
