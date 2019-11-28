#include "miner/blockchain-controller.hpp"
#include "common/logger.hpp"
#include <cstdlib>
#include <gflags/gflags.h>
#include <iostream>
#include <thread>

DECLARE_string(blockchain);

namespace Miner {

BlockChainController::BlockChainController() : rng_(rd_()), dist_(0, 1000000) {
  std::optional<Miner::BlockChain> maybeBlockchain =
      Miner::BlockChain::fromDirectory(std::filesystem::path(FLAGS_blockchain));
  blockchain_ = std::make_unique<Miner::BlockChain>(maybeBlockchain.value());
}

Common::optional_ref<Block> BlockChainController::addTransaction(const std::string& transaction) {
  blockchain_->addTransaction(transaction, dist_(rd_));
  return blockchain_->lastBlock();
}

Common::optional_ref<Block> BlockChainController::addTransaction(const std::string& transaction, int nonce) {
  blockchain_->addTransaction(transaction, nonce);
  return blockchain_->lastBlock();
}

bool BlockChainController::receivedBlockMined(int id, int nonce) {
  auto minedBlock = blockchain_->getBlock(id);
  if (!minedBlock) {
    std::this_thread::sleep_for(std::chrono::seconds(1));  // Give miner a chance to mine it by itself
    minedBlock = blockchain_->getBlock(id);
  }
  if (minedBlock) {
    std::cout << "Queueing nonce that we received " << nonce << std::endl;
    minedBlock->get().queueNonce(nonce);
    minedBlock->get().increaseVerification();
    minedBlock->get().save();
  }
  return minedBlock.has_value();
}

int BlockChainController::getLastBlockId() { return blockchain_->lastBlockID(); }

std::vector<Common::Models::BlockMined> BlockChainController::getLastBlocks(int lastId) {
  std::vector<Common::Models::BlockMined> lastBlocks;
  for (int i = lastId + 1; i <= blockchain_->lastBlockID(); ++i) {
    auto block = blockchain_->getBlock(i);
    Common::Models::BlockMined blockMined = {.id = i,
                                             .nonce = block->get().nonce(),
                                             .numberOfVerifications = block->get().numberOfVerifications(),
                                             .data = block->get().data()};
    lastBlocks.push_back(blockMined);
  }
  return lastBlocks;
}

std::vector<Common::Models::Block> BlockChainController::getBlocks(int blockCount) {
  int lastBlockID = getLastBlockId();
  if (blockCount == 0 || blockCount > lastBlockID) {
    blockCount = lastBlockID + 1;
  }

  std::vector<Common::Models::Block> blocks;
  for (int id = lastBlockID - blockCount + 1; id <= lastBlockID; id++) {
    Common::optional_ref<Block> blockRef = blockchain_->getBlock(static_cast<int>(id));
    if (blockRef) {
      Common::Models::Block block = {
          .id = blockRef->get().id(),
          .nonce = blockRef->get().nonce(),
          .hash = blockRef->get().previousHash(),
          .content = blockRef->get().data(),
          .numberOfVerifications = blockRef->get().numberOfVerifications(),
      };
      blocks.push_back(block);
    }
    else {
      Common::Logger::get()->info("Failed to get block #" + std::to_string(id) + "\n");
      // blocks.emplace(-1, -1, "error", "error");
    }
  }

  return blocks;
}

}  // namespace Miner
