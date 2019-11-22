#include "miner/blockchain-controller.hpp"
#include "common/logger.hpp"
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

Common::optional_ref<Block> BlockChainController::addTransaction(const std::string& transaction) {
  auto lastBlock = blockchain_->lastBlock();
  lastBlock->get().queueNonce(dist_(rng_));
  blockchain_->appendTransaction(transaction);
  blockchain_->saveAll();
  blockchain_->nextBlock();
  return lastBlock;
}

bool BlockChainController::receivedBlockMined(unsigned int id, unsigned int nonce) {
  blockchain_->lastBlock()->get().queueNonce(nonce);
  auto minedBlock = blockchain_->getBlock(id);
  if (!minedBlock) {
    std::this_thread::sleep_for(std::chrono::seconds(1));  // Give miner a chance to mine it by itself
    minedBlock = blockchain_->getBlock(id);
  }
  if (minedBlock) {
    minedBlock->get().increaseVerification();
    minedBlock->get().save();
  }
  return minedBlock.has_value();
}

unsigned int BlockChainController::getLastBlockId() {
  return blockchain_->lastBlockID() == 0 ? 0 : blockchain_->lastBlockID() - 1;
}

std::vector<Common::Models::BlockMined> BlockChainController::getLastBlocks(unsigned int lastId) {
  std::vector<Common::Models::BlockMined> lastBlocks;
  for (unsigned int i = lastId; i < blockchain_->lastBlockID(); ++i) {
    auto block = blockchain_->getBlock(i);
    Common::Models::BlockMined blockMined = {.id = i,
                                             .nonce = block->get().nonce(),
                                             .numberOfVerifications = block->get().numberOfVerifications(),
                                             .data = block->get().data()};
    lastBlocks.push_back(blockMined);
  }
  return lastBlocks;
}

}  // namespace Miner
