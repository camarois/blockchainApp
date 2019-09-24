#include <common/message_helper.hpp>
#include <example.hpp>
#include <iostream>
#include <string>
#include <unistd.h>
#include <zmq.hpp>

int main() {
  try {
    zmq::context_t context(1);
    zmq::socket_t socket(context, ZMQ_REP);
    socket.bind("tcp://*:5555");

    while (true) {
      zmq::message_t request;
      socket.recv(request, zmq::recv_flags::none);
      auto str = MessageHelper::to_string(request);
      std::cout << str << std::endl;

      sleep(1);

      auto reply = MessageHelper::from_string("from rest");
      socket.send(reply, zmq::send_flags::none);
    }
    return 0;
  } catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }

  // Port port(10000);

  // int thr = 2;

  // if (argc >= 2) {
  //     port = std::stol(argv[1]);

  //     if (argc == 3)
  //         thr = std::stol(argv[2]);
  // }

  // Address addr(Ipv4::any(), port);

  // cout << "Cores = " << hardware_concurrency() << endl;
  // cout << "Using " << thr << " threads" << endl;

  // StatsEndpoint stats(addr);

  // stats.init(thr);
  // stats.start();
}
