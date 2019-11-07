#include <cerrno>
#include <chrono>
#include <common/message_helper.hpp>
#include <common/models.hpp>
#include <iostream>
#include <rest/zmq.hpp>
#include <set>
#include <uuid.h>

namespace Rest {

// NOLINTNEXTLINE(modernize-pass-by-value)
ZMQWorker::ZMQWorker(const std::string& serverHostname)
    : running_(false),
      serverHostname_(serverHostname),  // NOLINT
      context_(1),
      socketPubToMiner_(context_, zmq::socket_type::pub),
      socketPullFromMiner_(context_, zmq::socket_type::pull),
      socketXPubBlockchain_(context_, zmq::socket_type::xpub),
      socketXSubBlockchain_(context_, zmq::socket_type::xsub) {}

ZMQWorker::~ZMQWorker() {
  running_ = false;
  socketPubToMiner_.close();
  socketPullFromMiner_.close();
  socketXPubBlockchain_.close();
  socketXSubBlockchain_.close();
  context_.close();
  join();
}

bool ZMQWorker::start() {
  try {
    const int linger = 1000;
    socketPubToMiner_.setsockopt(ZMQ_LINGER, &linger, sizeof(linger));
    socketPullFromMiner_.setsockopt(ZMQ_LINGER, &linger, sizeof(linger));
    socketXPubBlockchain_.setsockopt(ZMQ_LINGER, &linger, sizeof(linger));
    socketXSubBlockchain_.setsockopt(ZMQ_LINGER, &linger, sizeof(linger));

    socketPubToMiner_.bind(serverHostname_ + ":" + std::to_string(kMiner1Port_));
    socketPullFromMiner_.bind(serverHostname_ + ":" + std::to_string(kMiner2Port_));
    socketXPubBlockchain_.bind(serverHostname_ + ":" + std::to_string(kMiner3Port_));
    socketXSubBlockchain_.bind(serverHostname_ + ":" + std::to_string(kMiner4Port_));
  } catch (const zmq::error_t& e) {
    std::cerr << "ZMQ: failed to bind socket: " << e.what() << std::endl;
    return false;
  }

  running_ = true;
  threadPullFromMiner_ = std::thread(&ZMQWorker::handlePullFromMiner, this);
  threadProxyBlockchain_ = std::thread(&ZMQWorker::handleProxyBlockchain, this);

  return true;
}

void ZMQWorker::join() {
  if (threadPullFromMiner_.joinable()) {
    threadPullFromMiner_.join();
  }
  if (threadProxyBlockchain_.joinable()) {
    threadProxyBlockchain_.join();
  }
}

Common::Models::SqlResponse ZMQWorker::getRequest(const Common::Models::SqlRequest& sql) {
  std::future<std::string> request = createRequest(Common::Models::toStr(sql), Common::Models::kTypeServerRequest);
  request.wait_for(std::chrono::seconds(kWaitTimeout_));
  return nlohmann::json::parse(request.get());
}

Common::Models::SqlResponse ZMQWorker::updateRequest(const Common::Models::SqlRequest& sql) {
  std::future<std::string> request = createRequest(Common::Models::toStr(sql), Common::Models::kTypeTransaction);
  request.wait_for(std::chrono::seconds(kWaitTimeout_));
  return nlohmann::json::parse(request.get());
}

void ZMQWorker::handlePullFromMiner() {
  std::cout << "ZMQ/miners: thread started" << std::endl;

  while (running_) {
    try {
      zmq::message_t msg;
      std::optional<size_t> len = socketPullFromMiner_.recv(msg, zmq::recv_flags::none);
      if (!len) {
        std::cerr << "ZMQ/miners: failed to pull message" << std::endl;
        continue;
      }

      auto message = Common::MessageHelper::toModel<Common::Models::ZMQMessage>(msg);

      if (message.type == Common::Models::kTypeServerResponse) {
        auto response = Common::Models::fromStr<Common::Models::ServerResponse>(message.data);
        receivedResponse(response.token, response.result);
      }
    } catch (const std::exception& e) {
      std::cerr << "ZMQ/miners: " << e.what() << std::endl;
    }
  }
}

void ZMQWorker::handleProxyBlockchain() {
  std::cout << "ZMQ/proxy: thread started" << std::endl;

  try {
    zmq::proxy(socketXSubBlockchain_, socketXPubBlockchain_);
  } catch (const zmq::error_t& e) {
    std::cerr << "ZMQ/proxy: failed to create proxy: " << e.what() << std::endl;
  }
}

bool ZMQWorker::sendRequest(const std::string& json) {
  zmq::message_t data = Common::MessageHelper::fromString(json);

  try {
    socketPubToMiner_.send(data, zmq::send_flags::none);
  } catch (const zmq::error_t& e) {
    std::cerr << "ZMQ/miners: failed to send request" << std::endl;
    return false;
  }

  return true;
}

void ZMQWorker::receivedResponse(const std::string& token, const std::string& response) {
  if (getRequests_.find(token) == getRequests_.end()) {
    return;
  }

  getRequests_.at(token).set_value(response);
  getRequests_.erase(token);
}

std::future<std::string> ZMQWorker::createRequest(const std::string& sql, const std::string& type) {
  std::string token = uuids::to_string(uuids::uuid_system_generator{}());

  Common::Models::ServerRequest request;
  request.token = token;
  request.command = sql;
  nlohmann::json requestJSON = request;

  Common::Models::ZMQMessage message;
  message.type = type;
  message.data = requestJSON.dump();
  nlohmann::json messageJSON = message;

  getRequests_.emplace(token, std::promise<std::string>{});
  sendRequest(messageJSON.dump());
  return getRequests_.at(token).get_future();
}

}  // namespace Rest
