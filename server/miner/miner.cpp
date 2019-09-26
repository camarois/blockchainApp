#include "common/message_helper.hpp"
#include <common/firebase_helper.hpp>
#include <iostream>
#include <string>
#include <unistd.h>
#include <zmq.hpp>

int main(int argc, char* argv[]) {
  try {
    zmq::context_t context(1);
    zmq::socket_t socket(context, ZMQ_REQ);
    std::ostringstream oss;
    std::string serverIpAddress;
    if (argc > 1) {
      serverIpAddress = FirebaseHelper::getServerIpAddress(argv[1]);
    } else {
      serverIpAddress = FirebaseHelper::getServerIpAddress();
    }
    oss << "tcp://" << serverIpAddress << ":5555";
    std::cout << "Server ip address: " << serverIpAddress << std::endl;
    socket.connect(oss.str());

    while (true) {
      std::cout << "Sending " << std::flush;
      auto request = MessageHelper::fromString("from miner");
      socket.send(request, zmq::send_flags::none);
      std::cout << "Sent " << std::flush;

      zmq::message_t reply;
      std::cout << "Receving " << std::flush;
      socket.recv(reply, zmq::recv_flags::none);
      auto str = MessageHelper::toString(reply);
      std::cout << str << std::endl;
    }
    return 0;
  } catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }
}
