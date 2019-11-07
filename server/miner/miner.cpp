#include "common/firebase_helper.hpp"
#include "common/gflags_helper.hpp"
#include "common/message_helper.hpp"
#include <iostream>
#include <miner/blockchain.hpp>
#include <miner/zmq.hpp>
#include <string>
#include <unistd.h>
#include <zmq.hpp>

DEFINE_string(addr, "", "REST service address");                // NOLINT
DEFINE_string(user, "server", "Developper using the service");  // NOLINT
DEFINE_string(db, "blockchain.db", "Path to sqlite db file");   // NOLINT

int main(int argc, char* argv[]) {
  Common::GflagsHelper::init("Blockchain miner service", argc, argv);

  std::string addr = FLAGS_addr;
  if (addr.empty()) {
    addr = "tcp://" + Common::FirebaseHelper::getServerIpAddress(FLAGS_user);
  }
  std::cout << "Server ip address: " << addr << std::endl;

  std::optional<Miner::BlockChain> blockchain = Miner::BlockChain::fromDirectory(std::filesystem::path("."));

  Miner::ZMQWorker miner(addr, blockchain.value());
  miner.start();
  miner.join();

  // try {
  //   zmq::context_t context(1);
  //   zmq::socket_t socket(context, ZMQ_REQ);
  //   socket.connect(addr);

  //   while (true) {
  //     std::cout << "Sending " << std::flush;
  //     auto request = Common::MessageHelper::fromString("from miner");
  //     socket.send(request, zmq::send_flags::none);
  //     std::cout << "Sent " << std::flush;

  //     zmq::message_t reply;
  //     std::cout << "Receving " << std::flush;
  //     socket.recv(reply, zmq::recv_flags::none);
  //     auto str = Common::MessageHelper::toString(reply);
  //     std::cout << str << std::endl;
  //   }
  //   return 0;
  // } catch (const std::exception& e) {
  //   std::cerr << e.what() << std::endl;
  //   return 1;
  // }
}
