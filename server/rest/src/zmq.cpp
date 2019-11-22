#include <chrono>
#include <common/database.hpp>
#include <common/logger.hpp>
#include <common/message_helper.hpp>
#include <common/models.hpp>
#include <iostream>
#include <rest/zmq.hpp>
#include <set>
#include <uuid.h>

namespace Rest {

std::shared_ptr<ZMQWorker> ZMQWorker::instance;

// NOLINTNEXTLINE(modernize-pass-by-value)
ZMQWorker::ZMQWorker(const std::string& serverHostname)
    : running_(false),
      serverHostname_(serverHostname),  // NOLINT
      minersCount_(0),
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

std::shared_ptr<ZMQWorker> ZMQWorker::get() {
  if (instance) {
    return instance;
  }
  throw std::runtime_error("ZMQWorker not created, you should initilize it first");
}

void ZMQWorker::init(const std::string& serverHostname) {
  if (!instance) {
    instance = std::make_shared<ZMQWorker>(serverHostname);
    Common::Logger::get()->info("ZMQWorker created\n");
  }
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
  }
  catch (const zmq::error_t& e) {
    Common::Logger::get()->error(std::string("ZMQ: failed to bind socket: ") + e.what() + "\n");
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
  auto status = request.wait_for(std::chrono::seconds(kWaitTimeout_));
  if (status == std::future_status::timeout) {
    throw std::runtime_error("Timeout exceeded, miner not responding");
  }
  return nlohmann::json::parse(request.get());
}

Common::Models::SqlResponse ZMQWorker::updateRequest(const Common::Models::SqlRequest& sql) {
  std::future<std::string> request = createRequest(Common::Models::toStr(sql), Common::Models::kTypeTransaction);
  auto status = request.wait_for(std::chrono::seconds(kWaitTimeout_));
  if (status == std::future_status::timeout) {
    throw std::runtime_error("Timeout exceeded, miner not responding");
  }
  return nlohmann::json::parse(request.get());
}

void ZMQWorker::handlePullFromMiner() {
  Common::Logger::get()->info("ZMQ/miners: thread started\n");

  while (running_) {
    try {
      zmq::message_t msg;
      std::optional<size_t> len = socketPullFromMiner_.recv(msg, zmq::recv_flags::none);
      if (!len) {
        Common::Logger::get()->error("ZMQ/miners: failed to pull message\n");
        continue;
      }

      auto message = Common::MessageHelper::toModel<Common::Models::ZMQMessage>(msg);

      if (message.type == Common::Models::kTypeMinerReady) {
        Common::Models::ServerResponse response = nlohmann::json::parse(message.data);
        sendId(response.token);
      }
      else if (message.type == Common::Models::kTypeServerResponse) {
        auto response = Common::Models::fromStr<Common::Models::ServerResponse>(message.data);
        std::cout << "Setting last block id " << response.lastBlockId << std::endl;
        Common::Database::get()->setLastLogId(response.lastBlockId);
        receivedResponse(response.token, response.result);
      }
    }
    catch (const std::exception& e) {
      Common::Logger::get()->error(std::string("ZMQ/miners: ") + e.what() + "\n");
    }
  }
}

void ZMQWorker::handleProxyBlockchain() {
  Common::Logger::get()->info("ZMQ/proxy: thread started\n");

  try {
    zmq::proxy(socketXSubBlockchain_, socketXPubBlockchain_);
  }
  catch (const zmq::error_t& e) {
    Common::Logger::get()->error(std::string("ZMQ/proxy: failed to create proxy: ") + e.what() + "\n");
  }
}

bool ZMQWorker::sendId(const std::string& token) {
  // TODO(frgraf): handle what happens if server crashes or closes.
  minersCount_++;

  Common::Models::ServerRequest request = {
      .token = token, .command = std::to_string(minersCount_), .lastBlockId = Common::Database::get()->getLastLogId()};
  Common::Models::ZMQMessage message = {.type = Common::Models::kTypeMinerId, .data = Common::Models::toStr(request)};

  return sendRequest(Common::Models::toStr(message));
}

bool ZMQWorker::sendRequest(const std::string& json) {
  zmq::message_t data = Common::MessageHelper::fromString(json);

  try {
    socketPubToMiner_.send(data, zmq::send_flags::none);
  }
  catch (const zmq::error_t& e) {
    Common::Logger::get()->error("ZMQ/miners: failed to send request\n");
    return false;
  }

  return true;
}

void ZMQWorker::receivedResponse(const std::string& token, const std::string& response) {
  if (requests_.find(token) == requests_.end()) {
    return;
  }

  requests_.at(token).set_value(response);
  requests_.erase(token);
}

std::future<std::string> ZMQWorker::createRequest(const std::string& sql, const std::string& type) {
  std::string token = uuids::to_string(uuids::uuid_system_generator{}());

  Common::Models::ServerRequest request = {
      .token = token, .command = sql, .lastBlockId = Common::Database::get()->getLastLogId()};
  Common::Models::ZMQMessage message = {.type = type, .data = Common::Models::toStr(request)};

  requests_.emplace(token, std::promise<std::string>{});
  sendRequest(Common::Models::toStr(message));
  return requests_.at(token).get_future();
}

}  // namespace Rest
