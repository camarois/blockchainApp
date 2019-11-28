#ifndef MINER_BLOCKCHAIN_CONTROLLER_HPP
#define MINER_BLOCKCHAIN_CONTROLLER_HPP

#include <common/miner_models.hpp>
#include <filesystem>
#include <memory>
#include <optional>
#include <random>
#include <vector>

#include "miner/blockchain.hpp"

namespace Miner {

class BlockChainController {
 public:
  explicit BlockChainController();

  Common::optional_ref<Block> addTransaction(const std::string& transaction);
  Common::optional_ref<Block> addTransaction(const std::string& transaction, int nonce);
  bool receivedBlockMined(int id, int nonce);
  int getLastBlockId();
  std::vector<Common::Models::BlockMined> getLastBlocks(int lastId);
  std::vector<Common::Models::Block> getBlocks(unsigned int blockCount);

 private:
  std::mt19937 rng_;
  std::uniform_int_distribution<std::mt19937::result_type> dist_;

  std::unique_ptr<BlockChain> blockchain_;
};

}  // namespace Miner

#endif  // MINER_BLOCKCHAIN_CONTROLLER_HPP
