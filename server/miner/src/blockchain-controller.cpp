#include "miner/blockchain-controller.hpp"
#include <cstdlib>
#include <gflags/gflags.h>
#include <iostream>
#include <thread>

DECLARE_string(blockchain);

namespace Miner {

BlockChainController::BlockChainController() : dev_(), rng_(dev_()), dist_() {
  std::optional<Miner::BlockChain> maybeBlockchain =
      Miner::BlockChain::fromDirectory(std::filesystem::path(FLAGS_blockchain));
  blockchain_ = std::make_unique<Miner::BlockChain>(maybeBlockchain.value());
}

std::optional<Block> BlockChainController::addTransaction(const std::string& transaction) {
  blockchain_->appendTransaction(transaction);
  auto lastBlock = blockchain_->lastBlock();
  lastBlock->get().queueNonce(dist_(rng_));
  blockchain_->nextBlock();
  blockchain_->saveAll();
  return lastBlock;
}

void BlockChainController::receivedBlockMined(unsigned int id, unsigned int nonce) {
  blockchain_->lastBlock()->get().queueNonce(nonce);
  auto minedBlock = blockchain_->getBlock(id);
  while (!minedBlock) {
    std::cout << "Waiting for block to be mined" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    minedBlock = blockchain_->getBlock(id);
  }
  minedBlock->get().increaseVerification();
  minedBlock->get().save();
}

}  // namespace Miner
