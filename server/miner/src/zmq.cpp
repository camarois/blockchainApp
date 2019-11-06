#include <cerrno>
#include <iostream>

#include <common/message_helper.hpp>
#include <common/models.hpp>
#include <common/miner_models.hpp>
#include <miner/zmq.hpp>

namespace Miner {

ZMQWorker::ZMQWorker(const std::string& serverHostname, BlockChain& blockchain)
    : running_(false),
      serverHostname_(serverHostname),
      context_(1),
      socketSubServer_(context_, zmq::socket_type::sub),
      socketPushServer_(context_, zmq::socket_type::push),
      socketPubBlockchain_(context_, zmq::socket_type::pub),
      socketSubBlockchain_(context_, zmq::socket_type::sub),
      blockchain_(blockchain) {}

bool ZMQWorker::start() {
  try {
    socketSubServer_.setsockopt(ZMQ_SUBSCRIBE, "", 0);
  } catch (const zmq::error_t& e) {
    std::cerr << "ZMQ/setsockopt: " << e.what() << std::endl;
    return false;
  }

  running_ = true;
  threadServer_ = std::thread(&ZMQWorker::subServer, this);
  threadBlockchain_ = std::thread(&ZMQWorker::subBlockchain, this);

  return true;
}

void ZMQWorker::join() {
  running_ = false;
  threadServer_.join();
  threadBlockchain_.join();
}

void ZMQWorker::tryConnect(zmq::socket_t& socket, const std::string& address) {
  while (running_) {
    try {
      socket.connect(address);
    } catch (const zmq::error_t& e) {
      std::cerr << "ZMQ: failed to connect to " << address << ": " << e.what() << std::endl;
      std::this_thread::sleep_for(std::chrono::seconds(1));
      continue;
    }

    return;
  }
}

void ZMQWorker::subServer() {
  std::cout << "ZMQ/server: thread started" << std::endl;
  tryConnect(socketSubServer_, serverHostname_ + ":" + std::to_string(5555));
  tryConnect(socketPushServer_, serverHostname_ + ":" + std::to_string(5556));
  std::cout << "ZMQ/server: connected to server sub/push sockets" << std::endl;

  while (running_) {
    zmq::message_t msg;
    std::optional<size_t> len = socketSubServer_.recv(msg, zmq::recv_flags::none);
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
  }
}

void ZMQWorker::subBlockchain() {
  std::cout << "ZMQ/blockchain: thread started" << std::endl;
  tryConnect(socketSubBlockchain_, serverHostname_ + ":" + std::to_string(5557));
  tryConnect(socketPubBlockchain_, serverHostname_ + ":" + std::to_string(5558));
  std::cout << "ZMQ/blockchain: connected to sub/pub sockets" << std::endl;

  while (running_) {
    zmq::message_t msg;
    std::optional<size_t> len = socketSubBlockchain_.recv(msg, zmq::recv_flags::none);
    if (!len) {
      std::cerr << "ZMQ/blockchain: failed to receive message" << std::endl;
      continue;
    }

    Common::Models::ZMQMessage zmqMsg = Common::MessageHelper::toJSON(msg);
    // TODO(gabriel): handle blockchain message
  }
}

void ZMQWorker::sendResponse(const std::string& token, const std::string& result) {
  Common::Models::ServerResponse response;
  response.token = token;
  response.result = result;

  Common::Models::ZMQMessage message;
  message.type = Common::Models::kTypeServerResponse;
  message.data = Common::Models::toStr(response);

  zmq::message_t msg = Common::MessageHelper::fromModel(message);
  try {
    socketPushServer_.send(msg, zmq::send_flags::none);
  } catch(const zmq::error_t& e) {
    std::cerr << "ZMQ/server: failed to send message" << std::endl;
  }
}

}  // namespace Miner
