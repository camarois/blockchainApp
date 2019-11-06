#include <cerrno>
#include <iostream>

#include <common/message_helper.hpp>
#include <common/miner_models.hpp>
#include <common/models.hpp>
#include <miner/zmq.hpp>

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
      blockchain_(blockchain) {}

ZMQWorker::~ZMQWorker() {
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
  running_ = false;
  socketSubServer_->close();
  socketPushServer_->close();
  socketPubBlockchain_->close();
  socketSubBlockchain_->close();
  context_.close();
  threadServer_.join();
  threadBlockchain_.join();
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

  while (running_) {
    try {
      zmq::message_t msg;
      std::optional<size_t> len = socketSubServer_->recv(msg, zmq::recv_flags::none);
      if (!len) {
        std::cerr << "ZMQ/server: failed to receive message" << std::endl;
        continue;
      }

      Common::Models::ZMQMessage received = Common::MessageHelper::toJSON(msg);

      if (received.type == Common::Models::kTypeServerRequest) {
        Common::Models::ServerRequest request = nlohmann::json::parse(received.data);
        // TODO(gabriel): database request
        sendResponse(request.token, request.command);
      } else if (received.type == Common::Models::kTypeTransaction) {
        std::cout << received.data << std::endl;
        blockchain_.appendTransaction(received.data);
        blockchain_.nextBlock();
        blockchain_.saveAll();
        // TODO(gabriel): update database
      }
    } catch (const std::exception& e) {
      std::cerr << "ZMQ/blockchain: " << e.what() << std::endl;
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
      } else {
        Common::Models::ZMQMessage zmqMsg = Common::MessageHelper::toJSON(msg);
        // TODO(gabriel): handle blockchain message
        std::cout << "TODO: " << Common::MessageHelper::toJSON(msg) << std::endl;
      }
    } catch (const std::exception& e) {
      std::cerr << "ZMQ/blockchain: " << e.what() << std::endl;
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

}  // namespace Miner
