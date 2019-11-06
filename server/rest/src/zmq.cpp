#include <cerrno>
#include <chrono>
#include <iostream>
#include <set>
#include <uuid.h>

#include <common/message_helper.hpp>
#include <common/miner_models.hpp>
#include <common/models.hpp>
#include <rest/zmq.hpp>

namespace Rest {

ZMQWorker::ZMQWorker(const std::string& serverHostname)
    : running_(false),
      serverHostname_(std::move(serverHostname)),
      context_(1),
      socketPubToMiner_(context_, zmq::socket_type::pub),
      socketPullFromMiner_(context_, zmq::socket_type::pull),
      socketXPubBlockchain_(context_, zmq::socket_type::xpub),
      socketXSubBlockchain_(context_, zmq::socket_type::xsub) {}

bool ZMQWorker::start() {
  try {
    socketPubToMiner_.bind(serverHostname_ + ":" + std::to_string(kMiner1Port_));
    socketPullFromMiner_.bind(serverHostname_ + ":" + std::to_string(kMiner2Port_));
    socketXPubBlockchain_.bind(serverHostname_ + ":" + std::to_string(kMiner3Port_));
    socketXSubBlockchain_.bind(serverHostname_ + ":" + std::to_string(kMiner4Port_));
  } catch (const zmq::error_t& e) {
    std::cerr << "ZMQ: failed to bind socket: " << e.what() << std::endl;
    return false;
  }

  running_ = true;
  threadPullFromMiner_ = std::thread(&ZMQWorker::pullFromMiner, this);
  threadProxyBlockchain_ = std::thread(&ZMQWorker::proxyBlockchain, this);

  return true;
}

void ZMQWorker::join() {
  running_ = false;
  threadPullFromMiner_.join();
  threadProxyBlockchain_.join();
}

std::string ZMQWorker::getRequest(const std::string& sql) {
  std::future<std::string> request = createGetRequest(sql);
  request.wait_for(std::chrono::seconds(kWaitTimeout_));
  return request.get();
}

void ZMQWorker::updateRequest(const std::string& sql) {
  Common::Models::ZMQMessage message;
  message.type = Common::Models::kTypeTransaction;
  message.data = sql;
  nlohmann::json messageJSON = message;

  sendRequest(messageJSON.dump());
}

void ZMQWorker::pullFromMiner() {
  std::cout << "ZMQ/miners: thread started" << std::endl;

  while (running_) {
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
  }
}

void ZMQWorker::proxyBlockchain() {
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

std::future<std::string> ZMQWorker::createGetRequest(const std::string& sql) {
  std::string token = uuids::to_string(uuids::uuid_system_generator{}());

  Common::Models::ServerRequest request;
  request.token = token;
  request.command = sql;
  nlohmann::json requestJSON = request;

  Common::Models::ZMQMessage message;
  message.type = Common::Models::kTypeServerRequest;
  message.data = requestJSON.dump();
  nlohmann::json messageJSON = message;

  getRequests_.emplace(token, std::promise<std::string>{});
  sendRequest(messageJSON.dump());
  return getRequests_.at(token).get_future();
}

}  // namespace Rest
