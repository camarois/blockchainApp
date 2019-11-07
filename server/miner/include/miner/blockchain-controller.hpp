#ifndef MINER_BLOCKCHAIN_CONTROLLER_HPP
#define MINER_BLOCKCHAIN_CONTROLLER_HPP

#include <filesystem>
#include <optional>

#include "miner/blockchain.hpp"

namespace Miner {

class BlockChainController {
 public:
  explicit BlockChainController(BlockChain& blockchain);

  std::optional<Block> addTransaction(const std::string& transaction);
  void receivedBlockMined(unsigned int id, unsigned int nonce);

 private:
  BlockChain& blockchain_;
  Common::optional_ref<Block> currentBlock_;
  bool receivedNonce_;
};

} // namespace Miner

#endif // MINER_BLOCKCHAIN_CONTROLLER_HPP
