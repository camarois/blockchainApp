#include <common/database.hpp>
#include <common/logger.hpp>
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
      syncing_(false),
      serverHostname_(serverHostname),  // NOLINT
      context_(1),
      socketSubServer_(std::make_unique<zmq::socket_t>(context_, zmq::socket_type::sub)),
      socketPushServer_(std::make_unique<zmq::socket_t>(context_, zmq::socket_type::push)),
      socketPubBlockchain_(std::make_unique<zmq::socket_t>(context_, zmq::socket_type::pub)),
      socketSubBlockchain_(std::make_unique<zmq::socket_t>(context_, zmq::socket_type::sub)) {}

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

      Common::Models::ZMQMessage received = Common::MessageHelper::toJSON(msg);

      Common::Models::ServerRequest request = nlohmann::json::parse(received.data);
      std::cout << "Last block id " << request.lastBlockId << " vs our last id "
                << blockchainController_.getLastBlockId() << std::endl;
      if (request.lastBlockId > blockchainController_.getLastBlockId()) {
        sendBlockSyncRequest();
      }

      if (syncing_) {
        Common::Logger::get()->attention("Can't accept message, in syncing state.\n");
        continue;
      }
      if (received.type == Common::Models::kTypeMinerId) {
        if (request.token == token_) {
          auto selfId = std::stoi(request.command);
          Common::Database::get()->setSelfId(selfId);
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
      else if (received.type == Common::Models::kTypeLogRequest) {
        Common::Models::SqlRequest sql = nlohmann::json::parse(request.command);
        Common::Models::GetLogsRequest params = nlohmann::json::parse(sql.params);
        if (params.provenance == Common::Database::get()->getSelfId()) {
          printSqlRequest("ZMQ/blockchain: executing get log transaction: ", sql);
          sendResponse(request.token, Common::Models::toStr(Common::Database::get()->executeRequest(sql)));
        }
        continue;
      }
      else if (received.type == Common::Models::kTypeTransaction) {
        Common::Models::SqlRequest sql = nlohmann::json::parse(request.command);
        printSqlRequest("ZMQ/blockchain: executing update transaction: ", sql);
        auto block = blockchainController_.addTransaction(received.data);
        if (block) {
          sendBlockMined(block->get().id(), block->get().nonce());
          std::this_thread::sleep_for(std::chrono::seconds(1));
        }
        sendResponse(request.token, Common::Models::toStr(Common::Database::get()->executeRequest(sql)));
      }
      else if (received.type == Common::Models::kTypeGetBlocksRequest) {
        Common::Models::GetBlocksRequest req = nlohmann::json::parse(request.command);
        if (req.minerID == Common::Database::get()->getSelfId()) {
          auto blocks = blockchainController_.getBlocks(req.blockCount);
          sendGetBlocksResponse(request.token, blocks);
        }
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
        Common::Logger::get()->info("Received block mined with id " + std::to_string(blockMined.id) + "\n");
        if (blockchainController_.receivedBlockMined(blockMined.id, blockMined.nonce)) {
          Common::Logger::get()->info("ZMQ/blockchain: received nonce " + std::to_string(blockMined.nonce) +
                                      " for block #" + std::to_string(blockMined.id) + "\n");
        }
        else {
          sendBlockSyncRequest();
        }
      }
      else if (received.type == Common::Models::kTypeBlockSyncRequest) {
        if (!syncing_) {
          Common::Models::BlockSyncRequest request = nlohmann::json::parse(received.data);
          auto lastBlocks = blockchainController_.getLastBlocks(request.lastId);
          if (!lastBlocks.empty()) {
            sendBlockSyncResponse(lastBlocks);
          }
        }
      }
      else if (received.type == Common::Models::kTypeBlockSyncResponse) {
        if (syncing_) {
          Common::Models::BlockSyncResponse response = nlohmann::json::parse(received.data);
          std::sort(response.blocks.begin(), response.blocks.end(),
                    [](const auto& lhs, const auto& rhs) { return lhs.id < rhs.id; });
          std::cout << "Syncing blocks from " << response.blocks.front().id << " to " << response.blocks.back().id
                    << std::endl;
          for (const auto& block : response.blocks) {
            if (block.id > blockchainController_.getLastBlockId()) {
              std::cout << "Syncing block #" << block.id << " with nonce " << block.nonce << std::endl;
              auto b = blockchainController_.addTransaction(block.data, block.nonce);
              for (auto i = 0; i < block.numberOfVerifications; ++i) {
                b->get().increaseVerification();
              }
              b->get().save();
              Common::Models::ServerRequest request = nlohmann::json::parse(block.data);
              Common::Models::SqlRequest sql = nlohmann::json::parse(request.command);
              Common::Models::toStr(Common::Database::get()->executeRequest(sql));
              sendBlockMined(block.id, block.nonce);
              std::cout << "Syncing block #" << block.id << " done" << std::endl;
            }
          }
          std::cout << "Syncing blocks done" << std::endl;
          syncing_ = false;
        }
      }
      else {
        Common::Logger::get()->error("ZMQ/blockchain: Type not found: " + received.type + "\n");
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
  Common::Models::ReadyResponse response = {.token = token_,
                                            .selfId = Common::Database::get()->getSelfId(),
                                            .lastBlockId = blockchainController_.getLastBlockId()};
  Common::Models::ZMQMessage message = {.type = Common::Models::kTypeMinerReady,
                                        .data = Common::Models::toStr(response)};
  sendToSocket(socketPushServer_, message);
}

void ZMQWorker::sendResponse(const std::string& token, const std::string& result) {
  Common::Models::ServerResponse response = {
      .token = token, .result = result, .lastBlockId = blockchainController_.getLastBlockId()};
  Common::Models::ZMQMessage message = {.type = Common::Models::kTypeServerResponse,
                                        .data = Common::Models::toStr(response)};
  sendToSocket(socketPushServer_, message);
}

void ZMQWorker::sendBlockMined(int id, int nonce) {
  Common::Logger::get()->info("Sending mined block " + std::to_string(id) + "\n");
  Common::Models::BlockMined response = {.id = id, .nonce = nonce};
  Common::Models::ZMQMessage message = {.type = Common::Models::kTypeBlockMined,
                                        .data = Common::Models::toStr(response)};
  sendToSocket(socketPubBlockchain_, message);
}

void ZMQWorker::sendBlockSyncRequest() {
  Common::Logger::get()->info("Trying to sync\n");
  syncing_ = true;
  Common::Models::BlockSyncRequest sync = {.lastId = blockchainController_.getLastBlockId()};
  Common::Models::ZMQMessage message = {.type = Common::Models::kTypeBlockSyncRequest,
                                        .data = Common::Models::toStr(sync)};
  sendToSocket(socketPubBlockchain_, message);
}

void ZMQWorker::sendGetBlocksResponse(const std::string& token, std::vector<Common::Models::Block> blocks) {
  std::vector<std::string> blocksCopy;
  for (auto b : blocks) {
    try {
      blocksCopy.push_back(Common::Models::toStr(b));
    }
    catch (const std::exception& e) {
      b.hash = "0";
      blocksCopy.push_back(Common::Models::toStr(b));
    }
  }
  Common::Models::GetBlocksResponse result = {
      .blocks = blocksCopy,
  };

  Common::Models::ServerResponse response = {
      .token = token,
      .result = Common::Models::toStr(result),
  };

  Common::Models::ZMQMessage message = {
      .type = Common::Models::kTypeGetBlocksResponse,
      .data = Common::Models::toStr(response),
  };

  sendToSocket(socketPushServer_, message);
}

void ZMQWorker::sendBlockSyncResponse(const std::vector<Common::Models::BlockMined>& blocks) {
  Common::Logger::get()->info("Sending syncing blocks, from id " + std::to_string(blocks.front().id) + " to " +
                              std::to_string(blocks.back().id) + "\n");
  Common::Models::BlockSyncResponse sync = {.blocks = blocks};
  Common::Models::ZMQMessage message = {.type = Common::Models::kTypeBlockSyncResponse,
                                        .data = Common::Models::toStr(sync)};
  sendToSocket(socketPubBlockchain_, message);
}  // namespace Miner

}  // namespace Miner
