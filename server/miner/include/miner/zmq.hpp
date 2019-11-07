#ifndef MINER_ZMQ_HPP
#define MINER_ZMQ_HPP

#include <chrono>
#include <thread>
#include <zmq.hpp>

#include <miner/blockchain.hpp>

namespace Miner {

class ZMQWorker {
 public:
  // NOLINTNEXTLINE(google-runtime-references)
  explicit ZMQWorker(const std::string& serverHostname, BlockChain& blockchain);
  ~ZMQWorker();

  bool start();

 private:
  void join();
  void tryConnect(const std::unique_ptr<zmq::socket_t>& socket, const std::string& address);
  void handleSubServer();
  void handleSubBlockchain();
  void sendResponse(const std::string& token, const std::string& result);

  bool running_;
  const std::string serverHostname_;
  zmq::context_t context_;
  std::unique_ptr<zmq::socket_t> socketSubServer_;
  std::unique_ptr<zmq::socket_t> socketPushServer_;
  std::unique_ptr<zmq::socket_t> socketPubBlockchain_;
  std::unique_ptr<zmq::socket_t> socketSubBlockchain_;
  std::thread threadServer_;
  std::thread threadBlockchain_;
  BlockChain blockchain_;

  const int kMiner1Port_ = 5555;
  const int kMiner2Port_ = 5556;
  const int kMiner3Port_ = 5557;
  const int kMiner4Port_ = 5558;
};

}  // namespace Miner

#endif  // MINER_ZMQ_HPP
