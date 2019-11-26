#ifndef MINER_ZMQ_HPP
#define MINER_ZMQ_HPP

#include <atomic>
#include <chrono>
#include <common/miner_models.hpp>
#include <memory>
#include <miner/blockchain-controller.hpp>
#include <thread>
#include <uuid.h>
#include <zmq.hpp>

namespace Miner {

class ZMQWorker {
 public:
  // NOLINTNEXTLINE(google-runtime-references)
  explicit ZMQWorker(const std::string& serverHostname);
  ~ZMQWorker();

  bool start();
  void join();

 private:
  void tryConnect(const std::unique_ptr<zmq::socket_t>& socket, const std::string& address);
  void handleSubServer();
  void handleSubBlockchain();
  void sendReady();
  void printSqlRequest(const std::string& message, const Common::Models::SqlRequest& sql);
  void sendToSocket(const std::unique_ptr<zmq::socket_t>& socket, const Common::Models::ZMQMessage& message);
  void sendResponse(const std::string& token, const std::string& result);
  void sendBlockMined(int id, int nonce);
  void sendBlockSyncRequest();
  void sendBlockSyncResponse(const std::vector<Common::Models::BlockMined>& blocks);

  std::atomic<bool> running_;
  std::atomic<bool> syncing_;
  const std::string serverHostname_;
  zmq::context_t context_;
  std::unique_ptr<zmq::socket_t> socketSubServer_;
  std::unique_ptr<zmq::socket_t> socketPushServer_;
  std::unique_ptr<zmq::socket_t> socketPubBlockchain_;
  std::unique_ptr<zmq::socket_t> socketSubBlockchain_;
  std::thread threadServer_;
  std::thread threadBlockchain_;
  BlockChainController blockchainController_;

  const std::string token_ = uuids::to_string(uuids::uuid_system_generator{}());
  const int kMiner1Port_ = 5555;
  const int kMiner2Port_ = 5556;
  const int kMiner3Port_ = 5557;
  const int kMiner4Port_ = 5558;
};

}  // namespace Miner

#endif  // MINER_ZMQ_HPP
