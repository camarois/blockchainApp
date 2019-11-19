#include "miner/blockchain-controller.hpp"

#include "common/logger.hpp"
#include <cstdlib>
#include <gflags/gflags.h>
#include <iostream>

DECLARE_int32(id);

namespace Miner {

BlockChainController::BlockChainController(std::unique_ptr<BlockChain> blockchain)
    : blockchain_(std::move(blockchain)), receivedNonce_(false) {
  // TODO(gabriel): use better random generator
  // NOLINTNEXTLINE(cert-msc32-c,cert-msc51-cpp)
  srand(time(nullptr));
}

std::optional<Block> BlockChainController::addTransaction(const std::string& transaction) {
  blockchain_->appendTransaction(transaction);
  currentBlock_ = blockchain_->lastBlock();

  // TODO(gabriel): use better random generator
  // NOLINTNEXTLINE(cert-msc30-c,cert-msc50-cpp)
  receivedBlockMined(currentBlock_->get().id(), rand());
  receivedNonce_ = false;

  // TODO(gabriel): va nous falloir une meilleure politique de vérification

  blockchain_->nextBlock();
  blockchain_->saveAll();
  if (receivedNonce_) {
    Common::Logger::get()->attention(FLAGS_id, "Mining aborted of block #" + std::to_string(currentBlock_->get().id()) +
                                                   " by external nonce " +
                                                   std::to_string(currentBlock_->get().nonce()) + "\n");
    return {};
  }

  Common::Logger::get()->info(FLAGS_id, "Mining of block #" + std::to_string(currentBlock_->get().id()) +
                                            " finished with nonce " + std::to_string(currentBlock_->get().nonce()) +
                                            "\n");
  return currentBlock_->get();
}

void BlockChainController::receivedBlockMined(unsigned int id, unsigned int nonce) {
  if (!currentBlock_) {
    return;
  }

  if (currentBlock_->get().id() != id) {
    return;
  }

  receivedNonce_ = true;
  currentBlock_->get().queueNonce(nonce);
}

}  // namespace Miner
