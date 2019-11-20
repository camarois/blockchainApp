#ifndef REST_ZMQ_HPP
#define REST_ZMQ_HPP

#include <common/miner_models.hpp>
#include <future>
#include <map>
#include <thread>
#include <vector>
#include <zmq.hpp>

namespace Rest {

class ZMQWorker {
 public:
  explicit ZMQWorker(const std::string& serverHostname);
  ~ZMQWorker();

  static std::shared_ptr<ZMQWorker> get();
  static void init(const std::string& serverHostname);

  bool start();
  void join();
  Common::Models::SqlResponse getRequest(const Common::Models::SqlRequest& sql);
  Common::Models::SqlResponse updateRequest(const Common::Models::SqlRequest& sql);

 private:
  void handlePullFromMiner();
  void handleProxyBlockchain();
  bool sendId(const std::string& token);
  bool sendRequest(const std::string& json);
  void receivedResponse(const std::string& token, const std::string& response);
  std::future<std::string> createRequest(const std::string& sql, const std::string& type);

  static std::shared_ptr<ZMQWorker> instance;

  std::atomic<bool> running_;
  const std::string serverHostname_;
  std::atomic<int> minersCount_;
  zmq::context_t context_;
  zmq::socket_t socketPubToMiner_;
  zmq::socket_t socketPullFromMiner_;
  zmq::socket_t socketXPubBlockchain_;
  zmq::socket_t socketXSubBlockchain_;
  std::thread threadPullFromMiner_;
  std::thread threadProxyBlockchain_;
  std::unordered_map<std::string, std::promise<std::string>> requests_;

  const int kMiner1Port_ = 5555;
  const int kMiner2Port_ = 5556;
  const int kMiner3Port_ = 5557;
  const int kMiner4Port_ = 5558;
  const int kWaitTimeout_ = 5;
};

}  // namespace Rest

#endif  // REST_ZMQ_HPP
