#include <cerrno>
#include <common/database.hpp>
#include <common/message_helper.hpp>
#include <common/models.hpp>
#include <gflags/gflags.h>
#include <iostream>
#include <magic_enum.hpp>
#include <miner/zmq.hpp>

namespace Miner {

// NOLINTNEXTLINE(modernize-pass-by-value)
ZMQWorker::ZMQWorker(const std::string& serverHostname)
    : running_(false),
      serverHostname_(serverHostname),  // NOLINT
      context_(1),
      socketSubServer_(std::make_unique<zmq::socket_t>(context_, zmq::socket_type::sub)),
      socketPushServer_(std::make_unique<zmq::socket_t>(context_, zmq::socket_type::push)),
      socketPubBlockchain_(std::make_unique<zmq::socket_t>(context_, zmq::socket_type::pub)),
      socketSubBlockchain_(std::make_unique<zmq::socket_t>(context_, zmq::socket_type::sub)),
      blockchainController_() {}

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
  std::this_thread::sleep_for(std::chrono::seconds(1));
  sendBlockSyncRequest(blockchainController_.getLastBlockId());

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
      return;
    } catch (const zmq::error_t& e) {
      std::cerr << "ZMQ: failed to connect to " << address << ": " << e.what() << std::endl;
      std::this_thread::sleep_for(std::chrono::seconds(1));
    }
  }
}

void ZMQWorker::handleSubServer() {
  std::cout << "ZMQ/server: thread started" << std::endl;
  tryConnect(socketSubServer_, serverHostname_ + ":" + std::to_string(kMiner1Port_));
  tryConnect(socketPushServer_, serverHostname_ + ":" + std::to_string(kMiner2Port_));
  std::cout << "ZMQ/server: connected to server sub/push sockets" << std::endl;

  while (running_) {
    try {
      zmq::message_t msg;
      std::optional<size_t> len = socketSubServer_->recv(msg, zmq::recv_flags::none);
      if (!len) {
        std::cerr << "ZMQ/blockchain: failed to receive message" << std::endl;
        continue;
      }
      if (syncing_) {
        std::cout << "Can't accept message, in syncing state." << std::endl;
        continue;
      }
      // while (syncing_) {
      //   std::this_thread::sleep_for(std::chrono::seconds(1));
      // }

      Common::Models::ZMQMessage received = Common::MessageHelper::toJSON(msg);

      Common::Models::ServerRequest request = nlohmann::json::parse(received.data);
      if (received.type == Common::Models::kTypeServerRequest) {
        Common::Models::SqlRequest sql = nlohmann::json::parse(request.command);
        printSqlRequest("ZMQ/blockchain: executing get transaction: ", sql);
        sendResponse(request.token, Common::Models::toStr(Common::Database::get()->executeRequest(sql)));
      } else if (received.type == Common::Models::kTypeTransaction) {
        std::optional<Block> block = blockchainController_.addTransaction(received.data);
        if (block) {
          sendBlockMined(block->id(), block->nonce());
        }
        Common::Models::SqlRequest sql = nlohmann::json::parse(request.command);
        printSqlRequest("ZMQ/blockchain: executing update transaction: ", sql);
        sendResponse(request.token, Common::Models::toStr(Common::Database::get()->executeRequest(sql)));
      } else {
        std::cerr << "ZMQ/blockchain: Type not found: " << received.type << std::endl;
      }
    } catch (const std::exception& e) {
      std::cerr << "ZMQ/blockchain error in: handleSubServer for " << e.what() << std::endl;
    }
  }
}

void ZMQWorker::printSqlRequest(const std::string& message, const Common::Models::SqlRequest& sql) {
  std::string params = sql.params.empty() ? "NULL" : nlohmann::json::parse(sql.params).dump(Common::Models::kDumpTab);
  std::cout << message << magic_enum::enum_name(sql.function) << " with params:" << std::endl << params << std::endl;
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
        if (blockchainController_.receivedBlockMined(blockMined.id, blockMined.nonce)) {
          std::cout << "ZMQ/blockchain: received nonce " << blockMined.nonce << " for block #" << blockMined.id
                    << std::endl;
        } else {
          sendBlockSyncRequest(blockchainController_.getLastBlockId());
        }
      } else if (received.type == Common::Models::kTypeBlockSyncRequest) {
        Common::Models::BlockSyncRequest request = nlohmann::json::parse(received.data);
        auto lastBlocks = blockchainController_.getLastBlocks(request.lastId);
        if (!lastBlocks.empty()) {
          sendBlockSyncResponse(lastBlocks);
        }
      } else if (received.type == Common::Models::kTypeBlockSyncResponse) {
        Common::Models::BlockSyncResponse response = nlohmann::json::parse(received.data);
        for (auto block : response.blocks) {
          std::cout << "Syncing block #" << block.id << std::endl;
          blockchainController_.addTransaction(block.data);
          Common::Models::ServerRequest request = nlohmann::json::parse(received.data);
          Common::Models::SqlRequest sql = nlohmann::json::parse(request.command);
          Common::Models::toStr(Common::Database::get()->executeRequest(sql));
        }
        syncing_ = false;
      } else {
        std::cerr << "ZMQ/blockchain: Type not found: " << received.type << std::endl;
      }
    } catch (const std::exception& e) {
      std::cerr << "ZMQ/blockchain error in: handleSubBlockchain for " << e.what() << std::endl;
    }
  }
}

void ZMQWorker::sendToSocket(const std::unique_ptr<zmq::socket_t>& socket, const Common::Models::ZMQMessage& message) {
  zmq::message_t msg = Common::MessageHelper::fromModel(message);
  try {
    socket->send(msg, zmq::send_flags::none);
  } catch (const zmq::error_t& e) {
    std::cerr << "ZMQ/blockchain: failed to send message" << std::endl;
  }
}

void ZMQWorker::sendResponse(const std::string& token, const std::string& result) {
  Common::Models::ServerResponse response = {.token = token, .result = result};
  Common::Models::ZMQMessage message = {.type = Common::Models::kTypeServerResponse,
                                        .data = Common::Models::toStr(response)};
  sendToSocket(socketPushServer_, message);
}

void ZMQWorker::sendBlockMined(unsigned int id, unsigned int nonce) {
  Common::Models::BlockMined response = {.id = id, .nonce = nonce};
  Common::Models::ZMQMessage message = {.type = Common::Models::kTypeBlockMined,
                                        .data = Common::Models::toStr(response)};
  sendToSocket(socketPubBlockchain_, message);
}

void ZMQWorker::sendBlockSyncRequest(unsigned int lastId) {
  syncing_ = true;
  Common::Models::BlockSyncRequest sync = {.lastId = lastId};
  Common::Models::ZMQMessage message = {.type = Common::Models::kTypeBlockSyncRequest,
                                        .data = Common::Models::toStr(sync)};
  sendToSocket(socketPubBlockchain_, message);
}

void ZMQWorker::sendBlockSyncResponse(const std::vector<Common::Models::BlockMined>& blocks) {
  Common::Models::BlockSyncResponse sync = {.blocks = blocks};
  Common::Models::ZMQMessage message = {.type = Common::Models::kTypeBlockSyncResponse,
                                        .data = Common::Models::toStr(sync)};
  sendToSocket(socketPubBlockchain_, message);
}

}  // namespace Miner
