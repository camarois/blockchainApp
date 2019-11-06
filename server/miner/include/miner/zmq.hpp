#ifndef MINER_ZMQ_HPP
#define MINER_ZMQ_HPP

#include <chrono>
#include <thread>
#include <zmq.hpp>

#include <miner/blockchain.hpp>

namespace Miner {

class ZMQWorker {
 public:
  explicit ZMQWorker(const std::string& serverHostname, BlockChain& blockchain);

  bool start();
  void join();
 private:
  bool running_;
  const std::string serverHostname_;
  zmq::context_t context_;
  zmq::socket_t socketSubServer_;
  zmq::socket_t socketPushServer_;
  zmq::socket_t socketPubBlockchain_;
  zmq::socket_t socketSubBlockchain_;
  std::thread threadServer_;
  std::thread threadBlockchain_;
  BlockChain blockchain_;

  void tryConnect(zmq::socket_t& socket, const std::string& address);
  void subServer();
  void subBlockchain();
  void sendResponse(const std::string& token, const std::string& response);
};

} // namespace Miner

#endif // MINER_ZMQ_HPP
