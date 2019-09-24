#include "example.hpp"
#include <common/message_helper.hpp>
#include <iostream>
#include <string>
#include <unistd.h>
#include <zmq.hpp>

int main() {
  try {
    zmq::context_t context(1);
    zmq::socket_t socket(context, ZMQ_REQ);
    socket.connect("tcp://localhost:5555");

    while (true) {
      auto request = MessageHelper::from_string("from miner");
      socket.send(request, zmq::send_flags::none);

      zmq::message_t reply;
      socket.recv(reply, zmq::recv_flags::none);
      auto str = MessageHelper::to_string(reply);
      std::cout << str << std::endl;
    }
    return 0;
  } catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }
}
