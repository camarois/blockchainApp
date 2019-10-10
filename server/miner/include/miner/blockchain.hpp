#ifndef MINER_BLOCKCHAIN_HPP
#define MINER_BLOCKCHAIN_HPP

#include <filesystem>
#include <list>
#include <memory>

#include "miner/block.hpp"

namespace Miner {

using BlockPtr = std::shared_ptr<Block>;

class BlockChain {
 public:
  explicit BlockChain();
  explicit BlockChain(std::filesystem::path blockDir);

  BlockPtr newBlock();
  BlockPtr lastBlock() const;
 private:
  unsigned int difficulty_;
  std::list<BlockPtr> blocks_;
};

} // namespace Miner

#endif // MINER_BLOCKCHAIN_HPP
