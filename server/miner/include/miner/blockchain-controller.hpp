#ifndef MINER_BLOCKCHAIN_CONTROLLER_HPP
#define MINER_BLOCKCHAIN_CONTROLLER_HPP

#include <common/miner_models.hpp>
#include <filesystem>
#include <memory>
#include <optional>
#include <random>

#include "miner/blockchain.hpp"

namespace Miner {

class BlockChainController {
 public:
  explicit BlockChainController();

  Common::optional_ref<Block> addTransaction(const std::string& transaction);
  bool receivedBlockMined(int id, int nonce);
  int getLastBlockId();
  std::vector<Common::Models::BlockMined> getLastBlocks(int lastId);

 private:
  // std::random_device dev_;
  std::mt19937 rng_;
  std::uniform_int_distribution<std::mt19937::result_type> dist_;

  std::unique_ptr<BlockChain> blockchain_;
};

}  // namespace Miner

#endif  // MINER_BLOCKCHAIN_CONTROLLER_HPP
