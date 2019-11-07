#include <cerrno>
#include <iostream>

#include "common/database.hpp"
#include <common/message_helper.hpp>
#include <common/miner_models.hpp>
#include <common/models.hpp>
#include <gflags/gflags.h>
#include <miner/zmq.hpp>

DECLARE_string(db);

namespace Miner {

// NOLINTNEXTLINE(modernize-pass-by-value)
ZMQWorker::ZMQWorker(const std::string& serverHostname, BlockChain& blockchain)
    : running_(false),
      serverHostname_(serverHostname),  // NOLINT
      context_(1),
      socketSubServer_(std::make_unique<zmq::socket_t>(context_, zmq::socket_type::sub)),
      socketPushServer_(std::make_unique<zmq::socket_t>(context_, zmq::socket_type::push)),
      socketPubBlockchain_(std::make_unique<zmq::socket_t>(context_, zmq::socket_type::pub)),
      socketSubBlockchain_(std::make_unique<zmq::socket_t>(context_, zmq::socket_type::sub)),
      blockchainController_(blockchain) {}

ZMQWorker::~ZMQWorker() {
  running_ = false;
  socketSubServer_->close();
  socketPushServer_->close();
  socketPubBlockchain_->close();
  socketSubBlockchain_->close();
  context_.close();
  join();
}

bool ZMQWorker::start() {
  try {
    const int linger = 1000;
    socketSubServer_->setsockopt(ZMQ_LINGER, &linger, sizeof(linger));
    socketSubServer_->setsockopt(ZMQ_LINGER, &linger, sizeof(linger));
    socketSubServer_->setsockopt(ZMQ_LINGER, &linger, sizeof(linger));
    socketSubServer_->setsockopt(ZMQ_LINGER, &linger, sizeof(linger));

    socketSubServer_->setsockopt(ZMQ_SUBSCRIBE, "", 0);
    socketSubBlockchain_->setsockopt(ZMQ_SUBSCRIBE, "", 0);
  } catch (const zmq::error_t& e) {
    std::cerr << "ZMQ/setsockopt: " << e.what() << std::endl;
    return false;
  }

  running_ = true;
  threadServer_ = std::thread(&ZMQWorker::handleSubServer, this);
  threadBlockchain_ = std::thread(&ZMQWorker::handleSubBlockchain, this);

  return true;
}

void ZMQWorker::join() {
  if (threadServer_.joinable()) {
    threadServer_.join();
  }
  if (threadBlockchain_.joinable()) {
    threadBlockchain_.join();
  }
}

void ZMQWorker::tryConnect(const std::unique_ptr<zmq::socket_t>& socket, const std::string& address) {
  while (running_) {
    try {
      socket->connect(address);
    } catch (const zmq::error_t& e) {
      std::cerr << "ZMQ: failed to connect to " << address << ": " << e.what() << std::endl;
      std::this_thread::sleep_for(std::chrono::seconds(1));
      continue;
    }

    return;
  }
}

void ZMQWorker::handleSubServer() {
  std::cout << "ZMQ/server: thread started" << std::endl;
  tryConnect(socketSubServer_, serverHostname_ + ":" + std::to_string(kMiner1Port_));
  tryConnect(socketPushServer_, serverHostname_ + ":" + std::to_string(kMiner2Port_));
  std::cout << "ZMQ/server: connected to server sub/push sockets" << std::endl;
  Common::Database db("blockchain.db");

  while (running_) {
    try {
      zmq::message_t msg;
      std::optional<size_t> len = socketSubServer_->recv(msg, zmq::recv_flags::none);
      if (!len) {
        std::cerr << "ZMQ/server: failed to receive message" << std::endl;
        continue;
      }

      Common::Models::ZMQMessage received = Common::MessageHelper::toJSON(msg);

      Common::Models::ServerRequest request = nlohmann::json::parse(received.data);
      std::cout << "Received from server: " << request.command << std::endl;
      if (received.type == Common::Models::kTypeServerRequest) {
        sendResponse(request.token, Common::Models::toStr(db.get(nlohmann::json::parse(request.command))));
      } else if (received.type == Common::Models::kTypeTransaction) {
        std::cout << "ZMQ/blockchain: received data" << std::endl << "--> " << received.data << std::endl;
        std::optional<Block> block = blockchainController_.addTransaction(received.data);
        if (block) {
          sendBlockMined(block->id(), block->nonce());
        }
        sendResponse(request.token, Common::Models::toStr(db.get(nlohmann::json::parse(request.command))));
      }
    } catch (const std::exception& e) {
      std::cerr << "ZMQ/blockchain handleSubServer: " << e.what() << std::endl;
    }
  }
}

void ZMQWorker::handleSubBlockchain() {
  std::cout << "ZMQ/blockchain: thread started" << std::endl;
  tryConnect(socketSubBlockchain_, serverHostname_ + ":" + std::to_string(kMiner3Port_));
  tryConnect(socketPubBlockchain_, serverHostname_ + ":" + std::to_string(kMiner4Port_));
  std::cout << "ZMQ/blockchain: connected to sub/pub sockets" << std::endl;

  while (running_) {
    try {
      zmq::message_t msg;
      std::optional<size_t> len = socketSubBlockchain_->recv(msg, zmq::recv_flags::none);
      if (!len) {
        std::cerr << "ZMQ/blockchain: failed to receive message" << std::endl;
        continue;
      }

      Common::Models::ZMQMessage received = Common::MessageHelper::toJSON(msg);

      if (received.type == Common::Models::kTypeBlockMined) {
        Common::Models::BlockMined blockMined = nlohmann::json::parse(received.data);
        blockchainController_.receivedBlockMined(blockMined.id, blockMined.nonce);
        std::cout << "ZMQ/blockchain: received nonce " << blockMined.nonce << " for block #" << blockMined.id
                  << std::endl;
      }
    } catch (const std::exception& e) {
      std::cerr << "ZMQ/blockchain handleSubBlockchain: " << e.what() << std::endl;
    }
  }
}

void ZMQWorker::sendResponse(const std::string& token, const std::string& result) {
  Common::Models::ServerResponse response = {
      .token = token,
      .result = result,
  };

  Common::Models::ZMQMessage message = {
      .type = Common::Models::kTypeServerResponse,
      .data = Common::Models::toStr(response),
  };

  zmq::message_t msg = Common::MessageHelper::fromModel(message);
  try {
    socketPushServer_->send(msg, zmq::send_flags::none);
  } catch (const zmq::error_t& e) {
    std::cerr << "ZMQ/server: failed to send message" << std::endl;
  }
}

void ZMQWorker::sendBlockMined(unsigned int id, unsigned int nonce) {
  Common::Models::BlockMined response = {
      .id = id,
      .nonce = nonce,
  };

  Common::Models::ZMQMessage message = {
      .type = Common::Models::kTypeBlockMined,
      .data = Common::Models::toStr(response),
  };

  zmq::message_t msg = Common::MessageHelper::fromModel(message);
  try {
    socketPubBlockchain_->send(msg, zmq::send_flags::none);
  } catch (const zmq::error_t& e) {
    std::cerr << "ZMQ/blockchain: failed to publish message" << std::endl;
  }
}

}  // namespace Miner
