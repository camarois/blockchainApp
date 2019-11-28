#include "miner/blockchain-controller.hpp"
#include "common/logger.hpp"
#include <cstdlib>
#include <gflags/gflags.h>
#include <iostream>
#include <thread>

DECLARE_string(blockchain);

namespace Miner {

BlockChainController::BlockChainController() {
  std::optional<Miner::BlockChain> maybeBlockchain =
      Miner::BlockChain::fromDirectory(std::filesystem::path(FLAGS_blockchain));
  blockchain_ = std::make_unique<Miner::BlockChain>(maybeBlockchain.value());
}

Common::optional_ref<Block> BlockChainController::addTransaction(const std::string& transaction) {
  blockchain_->addTransaction(transaction);
  return blockchain_->lastBlock();
}

bool BlockChainController::receivedBlockMined(int id, int nonce) {
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
    blockCount = lastBlockID;
  }
  std::cout << "count" << blockCount << std::endl;
  std::cout << "lbid" << lastBlockID << std::endl;

  std::vector<Common::Models::Block> blocks;
  for (int id = lastBlockID - blockCount + 1; id <= lastBlockID; id++) {
    std::cout << "count" << blockCount << std::endl;
    std::cout << "lbid" << lastBlockID << std::endl;
    std::cout << "id" << id << std::endl;
    Common::optional_ref<Block> blockRef = blockchain_->getBlock(static_cast<int>(id));
    if (blockRef) {
      Common::Models::Block block = {
          .id = blockRef->get().id(),
          .nonce = blockRef->get().nonce(),
          .hash = blockRef->get().hash(),
          .content = blockRef->get().hash(),
      };
      blocks.push_back(block);
    }
    else {
      Common::Logger::get()->info("Failed to get block #" + std::to_string(id) + "\n");
      // blocks.emplace(-1, -1, "error", "error");
    }
  }

  std::cout << "out" << std::endl;
  return blocks;
}

}  // namespace Miner
