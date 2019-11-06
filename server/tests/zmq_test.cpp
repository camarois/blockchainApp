#include <chrono>
#include <filesystem>
#include <thread>

#include <gtest/gtest.h>
#include <miner/blockchain.hpp>
#include <miner/zmq.hpp>
#include <rest/zmq.hpp>

TEST(ZMQ, connects) {
  std::optional<Miner::BlockChain> blockchain = Miner::BlockChain::fromDirectory(std::filesystem::path("."));

  Miner::ZMQWorker miner1("tcp://127.0.0.1", *blockchain);
  //Miner::ZMQWorker miner2("tcp://127.0.0.1", *blockchain);
  //Miner::ZMQWorker miner3("tcp://127.0.0.1", *blockchain);
  Rest::ZMQWorker rest("tcp://*");

  ASSERT_TRUE(miner1.start());
  //ASSERT_TRUE(miner2.start());
  //ASSERT_TRUE(miner3.start());
  ASSERT_TRUE(rest.start());

  std::this_thread::sleep_for(std::chrono::seconds(1));

  rest.getRequest("test-get");
  rest.updateRequest("test-update 1");
  rest.updateRequest("test-update 2");
  rest.updateRequest("test-update 3");
  rest.updateRequest("test-update 4");

  while(1);
}
