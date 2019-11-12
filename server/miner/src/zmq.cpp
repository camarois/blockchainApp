#include <common/database.hpp>
#include <common/logger.hpp>
#include <common/message_helper.hpp>
#include <common/miner_models.hpp>
#include <common/models.hpp>
#include <gflags/gflags.h>
#include <iostream>
#include <miner/zmq.hpp>

DECLARE_string(db);

namespace Miner {

// NOLINTNEXTLINE(modernize-pass-by-value)
ZMQWorker::ZMQWorker(const std::string& serverHostname, std::unique_ptr<BlockChain> blockchain)
    : running_(false),
      serverHostname_(serverHostname),  // NOLINT
      context_(1),
      socketSubServer_(std::make_unique<zmq::socket_t>(context_, zmq::socket_type::sub)),
      socketPushServer_(std::make_unique<zmq::socket_t>(context_, zmq::socket_type::push)),
      socketPubBlockchain_(std::make_unique<zmq::socket_t>(context_, zmq::socket_type::pub)),
      socketSubBlockchain_(std::make_unique<zmq::socket_t>(context_, zmq::socket_type::sub)),
      blockchainController_(std::move(blockchain)) {}

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
    Common::Logger::get()->error(1, std::string("ZMQ/setsockopt: ") + e.what() + "\n");
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
      Common::Logger::get()->error(1, std::string("ZMQ: failed to connect to ") + address + e.what() + "\n");
      std::this_thread::sleep_for(std::chrono::seconds(1));
      continue;
    }

    return;
  }
}

void ZMQWorker::handleSubServer() {
  Common::Logger::get()->info(1, "ZMQ/server: thread started\n");
  tryConnect(socketSubServer_, serverHostname_ + ":" + std::to_string(kMiner1Port_));
  tryConnect(socketPushServer_, serverHostname_ + ":" + std::to_string(kMiner2Port_));
  Common::Logger::get()->info(1, "ZMQ/server: connected to server sub/push sockets\n");
  Common::Database db(FLAGS_db);

  while (running_) {
    try {
      zmq::message_t msg;
      std::optional<size_t> len = socketSubServer_->recv(msg, zmq::recv_flags::none);
      if (!len) {
        Common::Logger::get()->error(1, "ZMQ/server: failed to receive message\n");
        continue;
      }

      Common::Models::ZMQMessage received = Common::MessageHelper::toJSON(msg);

      Common::Models::ServerRequest request = nlohmann::json::parse(received.data);
      Common::Logger::get()->info(1, std::string("Received from server: ") + request.command + "\n");
      if (received.type == Common::Models::kTypeServerRequest) {
        sendResponse(request.token, Common::Models::toStr(db.get(nlohmann::json::parse(request.command))));
      } else if (received.type == Common::Models::kTypeTransaction) {
        Common::Logger::get()->info(1, std::string("ZMQ/blockchain: received data\n") + "--> " + received.data + "\n");
        std::optional<Block> block = blockchainController_.addTransaction(received.data);
        if (block) {
          sendBlockMined(block->id(), block->nonce());
          Common::Logger::get()->info(
              1, std::to_string(block->id()) + "\n" + std::to_string(block->nonce()) + "\n" + block->hash());
        }
        sendResponse(request.token, Common::Models::toStr(db.get(nlohmann::json::parse(request.command))));
      }
    } catch (const std::exception& e) {
      Common::Logger::get()->error(1, std::string("ZMQ/blockchain handleSubServer: \n") + e.what());
    }
  }
}

void ZMQWorker::handleSubBlockchain() {
  Common::Logger::get()->info(1, "ZMQ/blockchain: thread started\n");
  tryConnect(socketSubBlockchain_, serverHostname_ + ":" + std::to_string(kMiner3Port_));
  tryConnect(socketPubBlockchain_, serverHostname_ + ":" + std::to_string(kMiner4Port_));
  Common::Logger::get()->info(1, "ZMQ/blockchain: connected to sub/pub sockets\n");

  while (running_) {
    try {
      zmq::message_t msg;
      std::optional<size_t> len = socketSubBlockchain_->recv(msg, zmq::recv_flags::none);
      if (!len) {
        Common::Logger::get()->error(1, "ZMQ/blockchain: failed to receive message\n");
        continue;
      }

      Common::Models::ZMQMessage received = Common::MessageHelper::toJSON(msg);

      if (received.type == Common::Models::kTypeBlockMined) {
        Common::Models::BlockMined blockMined = nlohmann::json::parse(received.data);
        blockchainController_.receivedBlockMined(blockMined.id, blockMined.nonce);
        Common::Logger::get()->info(1, std::string("ZMQ/blockchain: received nonce ") +
                                           std::to_string(blockMined.nonce) + std::string("for block #") +
                                           std::to_string(blockMined.id) + "\n");
      }
    } catch (const std::exception& e) {
      Common::Logger::get()->error(1, std::string("ZMQ/blockchain handleSubBlockchain: \n") + e.what() + "\n");
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
    Common::Logger::get()->error(1, "ZMQ/server: failed to send message\n");
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
    Common::Logger::get()->error(1, "ZMQ/blockchain: failed to publish message\n");
  }
}

}  // namespace Miner
