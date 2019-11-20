#include <common/database.hpp>
#include <common/logger.hpp>
#include <common/message_helper.hpp>
#include <common/models.hpp>
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
  }
  catch (const zmq::error_t& e) {
    Common::Logger::get()->error(std::string("ZMQ/setsockopt: ") + e.what() + "\n");
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
    }
    catch (const zmq::error_t& e) {
      Common::Logger::get()->error(std::string("ZMQ: failed to connect to ") + address + e.what() + "\n");
      std::this_thread::sleep_for(std::chrono::seconds(1));
    }
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
      // if (syncing_) {
      //   std::cout << "Can't accept message, in syncing state." << std::endl;
      //   continue;
      // }
      // while (syncing_) {
      //   std::this_thread::sleep_for(std::chrono::seconds(1));
      // }

      Common::Models::ZMQMessage received = Common::MessageHelper::toJSON(msg);

      Common::Models::ServerRequest request = nlohmann::json::parse(received.data);
      if (received.type == Common::Models::kTypeMinerId) {
        if (request.token == token_) {
          Common::Logger::get()->setProvenance(std::stoi(request.command));
          Common::Logger::get()->info(std::string("This is miner #" + request.command + ", connected to server") +
                                      "\n");
        }
        else {
          Common::Logger::get()->info(std::string("Miner #" + request.command + " is connected to server") + "\n");
        }
      }
      else if (received.type == Common::Models::kTypeServerRequest) {
        Common::Models::SqlRequest sql = nlohmann::json::parse(request.command);
        printSqlRequest("ZMQ/blockchain: executing get transaction: ", sql);
        sendResponse(request.token, Common::Models::toStr(Common::Database::get()->executeRequest(sql)));
      }
      else if (received.type == Common::Models::kTypeTransaction) {
        Common::Models::SqlRequest sql = nlohmann::json::parse(request.command);
        printSqlRequest("ZMQ/blockchain: executing update transaction: ", sql);
        std::optional<Block> block = blockchainController_.addTransaction(received.data);
        if (block) {
          sendBlockMined(block->id(), block->nonce());
        }
        sendResponse(request.token, Common::Models::toStr(Common::Database::get()->executeRequest(sql)));
      }
      else {
        Common::Logger::get()->error("Type not found: " + received.type + "\n");
      }
    }
    catch (const std::exception& e) {
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
        if (blockchainController_.receivedBlockMined(blockMined.id, blockMined.nonce)) {
          Common::Logger::get()->info(std::string("ZMQ/blockchain: received nonce ") +
                                      std::to_string(blockMined.nonce) + std::string(" for block #") +
                                      std::to_string(blockMined.id) + "\n");
        }
        else {
          sendBlockSyncRequest(blockchainController_.getLastBlockId());
        }
      }
      else if (received.type == Common::Models::kTypeBlockSyncRequest) {
        Common::Models::BlockSyncRequest request = nlohmann::json::parse(received.data);
        auto lastBlocks = blockchainController_.getLastBlocks(request.lastId);
        if (!lastBlocks.empty()) {
          sendBlockSyncResponse(lastBlocks);
        }
      }
      else if (received.type == Common::Models::kTypeBlockSyncResponse) {
        Common::Models::BlockSyncResponse response = nlohmann::json::parse(received.data);
        for (auto block : response.blocks) {
          std::cout << "Syncing block #" << block.id << std::endl;
          blockchainController_.addTransaction(block.data);
          Common::Models::ServerRequest request = nlohmann::json::parse(received.data);
          Common::Models::SqlRequest sql = nlohmann::json::parse(request.command);
          Common::Models::toStr(Common::Database::get()->executeRequest(sql));
        }
        syncing_ = false;
      }
      else {
        std::cerr << "ZMQ/blockchain: Type not found: " << received.type << std::endl;
      }
    }
    catch (const std::exception& e) {
      Common::Logger::get()->error(std::string("ZMQ/blockchain handleSubBlockchain: \n") + e.what() + "\n");
    }
  }
}

void ZMQWorker::sendToSocket(const std::unique_ptr<zmq::socket_t>& socket, const Common::Models::ZMQMessage& message) {
  zmq::message_t msg = Common::MessageHelper::fromModel(message);
  try {
    socket->send(msg, zmq::send_flags::none);
  }
  catch (const zmq::error_t& e) {
    Common::Logger::get()->error("ZMQ/blockchain: failed to send message\n");
  }
}

void ZMQWorker::sendReady() {
  Common::Models::ServerResponse response = {.token = token_};
  Common::Models::ZMQMessage message = {.type = Common::Models::kTypeMinerReady,
                                        .data = Common::Models::toStr(response)};
  sendToSocket(socketPushServer_, message);
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
}  // namespace Miner

}  // namespace Miner
