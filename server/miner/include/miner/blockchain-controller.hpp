#ifndef MINER_BLOCKCHAIN_CONTROLLER_HPP
#define MINER_BLOCKCHAIN_CONTROLLER_HPP

#include <filesystem>
#include <memory>
#include <optional>
#include <random>
#include <common/miner_models.hpp>

#include "miner/blockchain.hpp"

namespace Miner {

class BlockChainController {
 public:
  explicit BlockChainController();

  std::optional<Block> addTransaction(const std::string& transaction);
  bool receivedBlockMined(unsigned int id, unsigned int nonce);
  unsigned int getLastBlockId();
  std::vector<Common::Models::BlockMined> getLastBlocks(unsigned int lastId);

 private:
  std::random_device dev_;
  std::mt19937 rng_;
  std::uniform_int_distribution<std::mt19937::result_type> dist_;

  std::unique_ptr<BlockChain> blockchain_;
};

} // namespace Miner

#endif // MINER_BLOCKCHAIN_CONTROLLER_HPP
