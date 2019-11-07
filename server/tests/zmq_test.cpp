#include <chrono>
#include <filesystem>
#include <thread>

#include <gtest/gtest.h>
#include <miner/blockchain.hpp>
#include <miner/zmq.hpp>
#include <rest/zmq.hpp>

#include "utils.hpp"

TEST(ZMQ, connects) {
  std::filesystem::path blockDir1 = Tests::createEmptyDir("blockchain1");
  std::filesystem::path blockDir2 = Tests::createEmptyDir("blockchain2");
  std::filesystem::path blockDir3 = Tests::createEmptyDir("blockchain3");

  std::optional<Miner::BlockChain> blockchain1 = Miner::BlockChain::fromDirectory(blockDir1);
  std::optional<Miner::BlockChain> blockchain2 = Miner::BlockChain::fromDirectory(blockDir2);
  std::optional<Miner::BlockChain> blockchain3 = Miner::BlockChain::fromDirectory(blockDir3);

  Miner::ZMQWorker miner1("tcp://127.0.0.1", *blockchain1);
  Miner::ZMQWorker miner2("tcp://127.0.0.1", *blockchain2);
  Miner::ZMQWorker miner3("tcp://127.0.0.1", *blockchain3);
  Rest::ZMQWorker rest("tcp://*");

  ASSERT_TRUE(miner1.start());
  ASSERT_TRUE(miner2.start());
  ASSERT_TRUE(miner3.start());
  ASSERT_TRUE(rest.start());

  std::this_thread::sleep_for(std::chrono::seconds(2));
  rest.updateRequest("test-update 1");
  rest.updateRequest("test-update 2");
  rest.updateRequest("test-update 3");

  while(1);
}
