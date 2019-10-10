#include "miner/blockchain.hpp"

namespace Miner {

BlockChain::BlockChain() { difficulty_ = 3; }

BlockChain::BlockChain(std::filesystem::path blockDir) : BlockChain() {
  if (blockDir.empty()) {
    return;
  }

  int lastBlock;
  for (lastBlock = 0; std::filesystem::exists(blockDir); lastBlock++) {
    blockDir.remove_filename();
    blockDir.append(std::to_string(lastBlock));
  }

  if (lastBlock == 0) {
    // TODO(gabriel): no blockchain files
  }
  lastBlock--;
}

BlockPtr BlockChain::newBlock() { return std::make_shared<Block>(); }

BlockPtr BlockChain::lastBlock() const { return std::make_shared<Block>(); }

}  // namespace Miner
