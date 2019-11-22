#include <common/database.hpp>
#include <common/logger.hpp>
#include <common/message_helper.hpp>
#include <common/miner_models.hpp>
#include <common/models.hpp>
#include <iostream>
#include <magic_enum.hpp>
#include <miner/zmq.hpp>

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
    Common::Logger::get()->error(std::string("ZMQ/setsockopt: ") + e.what() + "\n");
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
      Common::Logger::get()->error(std::string("ZMQ: failed to connect to ") + address + e.what() + "\n");
      std::this_thread::sleep_for(std::chrono::seconds(1));
      continue;
    }

    return;
  }
}

void ZMQWorker::handleSubServer() {
  Common::Logger::get()->info("ZMQ/server: thread started\n");
  tryConnect(socketSubServer_, serverHostname_ + ":" + std::to_string(kMiner1Port_));
  tryConnect(socketPushServer_, serverHostname_ + ":" + std::to_string(kMiner2Port_));
  Common::Logger::get()->info("ZMQ/server: connected to server sub/push sockets\n");
  sendReady();

  while (running_) {
    try {
      zmq::message_t msg;
      std::optional<size_t> len = socketSubServer_->recv(msg, zmq::recv_flags::none);
      if (!len) {
        Common::Logger::get()->error("ZMQ/blockchain: failed to receive message\n");
        continue;
      }

      Common::Models::ZMQMessage received = Common::MessageHelper::toJSON(msg);

      Common::Models::ServerRequest request = nlohmann::json::parse(received.data);
      if (received.type == Common::Models::kTypeMinerId) {
        Common::Logger::get()->setProvenance(std::stoi(request.command));
        Common::Logger::get()->info(std::string("Miner" + request.command + " is connected to server") + "\n");
      } else if (received.type == Common::Models::kTypeServerRequest) {
        Common::Models::SqlRequest sql = nlohmann::json::parse(request.command);
        printSqlRequest("ZMQ/blockchain: executing get transaction: ", sql);
        sendResponse(request.token, Common::Models::toStr(Common::Database::get()->executeRequest(sql)));
      } else if (received.type == Common::Models::kTypeTransaction) {
        Common::Models::SqlRequest sql = nlohmann::json::parse(request.command);
        printSqlRequest("ZMQ/blockchain: executing update transaction: ", sql);
        std::optional<Block> block = blockchainController_.addTransaction(received.data);
        if (block) {
          sendBlockMined(block->id(), block->nonce());
        }
        sendResponse(request.token, Common::Models::toStr(Common::Database::get()->executeRequest(sql)));
      } else {
        Common::Logger::get()->error("Type not found: " + received.type + "\n");
      }
    } catch (const std::exception& e) {
      Common::Logger::get()->error(std::string("ZMQ/blockchain handleSubServer: \n") + e.what());
    }
  }
}

void ZMQWorker::printSqlRequest(const std::string& message, const Common::Models::SqlRequest& sql) {
  std::string params = sql.params.empty() ? "NULL" : nlohmann::json::parse(sql.params).dump(Common::Models::kDumpTab);
  Common::Logger::get()->info(message + std::string(magic_enum::enum_name(sql.function)) + " with params:\n" + params +
                              "\n");
}

void ZMQWorker::handleSubBlockchain() {
  Common::Logger::get()->info("ZMQ/blockchain: thread started\n");
  tryConnect(socketSubBlockchain_, serverHostname_ + ":" + std::to_string(kMiner3Port_));
  tryConnect(socketPubBlockchain_, serverHostname_ + ":" + std::to_string(kMiner4Port_));
  Common::Logger::get()->info("ZMQ/blockchain: connected to sub/pub sockets\n");

  while (running_) {
    try {
      zmq::message_t msg;
      std::optional<size_t> len = socketSubBlockchain_->recv(msg, zmq::recv_flags::none);
      if (!len) {
        Common::Logger::get()->error("ZMQ/blockchain: failed to receive message\n");
        continue;
      }

      Common::Models::ZMQMessage received = Common::MessageHelper::toJSON(msg);

      if (received.type == Common::Models::kTypeBlockMined) {
        Common::Models::BlockMined blockMined = nlohmann::json::parse(received.data);
        blockchainController_.receivedBlockMined(blockMined.id, blockMined.nonce);
        Common::Logger::get()->info(std::string("ZMQ/blockchain: received nonce ") + std::to_string(blockMined.nonce) +
                                    std::string(" for block #") + std::to_string(blockMined.id) + "\n");
      }
    } catch (const std::exception& e) {
      Common::Logger::get()->error(std::string("ZMQ/blockchain handleSubBlockchain: \n") + e.what() + "\n");
    }
  }
}

void ZMQWorker::sendReady() {
  Common::Models::ZMQMessage message = {
      .type = Common::Models::kTypeMinerReady,
      .data = Common::Models::toStr("READY"),
  };

  zmq::message_t msg = Common::MessageHelper::fromModel(message);
  try {
    socketPushServer_->send(msg, zmq::send_flags::none);
  } catch (const zmq::error_t& e) {
    Common::Logger::get()->error("ZMQ/server: failed to send ready message\n");
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
    Common::Logger::get()->error("ZMQ/blockchain: failed to send message\n");
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
    Common::Logger::get()->error("ZMQ/blockchain: failed to publish message\n");
  }
}

}  // namespace Miner
