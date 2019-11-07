#include "miner/blockchain-controller.hpp"

#include <cstdlib>
#include <iostream>

namespace Miner {

BlockChainController::BlockChainController(BlockChain& blockchain) : blockchain_(blockchain) { srand(time(NULL)); }

std::optional<Block> BlockChainController::addTransaction(const std::string& transaction) {
  blockchain_.appendTransaction(transaction);
  currentBlock_ = blockchain_.lastBlock();

  receivedBlockMined(currentBlock_->get().id(), rand());
  receivedNonce_ = false;

  // TODO(gabriel): va nous falloir une meilleure politique de vérification

  blockchain_.nextBlock();
  blockchain_.saveAll();
  if (receivedNonce_) {
    std::cout << "mining aborted of block #" << currentBlock_->get().id() << " by external nonce "
              << currentBlock_->get().nonce() << std::endl;
    return {};
  } else {
    std::cout << "mining of block #" << currentBlock_->get().id() << " finished with nonce "
              << currentBlock_->get().nonce() << std::endl;
    return currentBlock_->get();
  }
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
