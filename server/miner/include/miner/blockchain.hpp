#ifndef MINER_BLOCKCHAIN_HPP
#define MINER_BLOCKCHAIN_HPP

#include <exception>
#include <filesystem>
#include <list>
#include <memory>

#include "miner/block.hpp"

namespace Miner {

class BlockChain;

using BlockPtr = std::shared_ptr<Block>;
using BlockChainUPtr = std::unique_ptr<BlockChain>;

class BlockChain {
 public:
  BlockChain();
  explicit BlockChain(std::filesystem::path blockDir);

  void appendTransaction(const std::string& transaction);
  void saveAll() const;
  BlockPtr nextBlock();
  BlockPtr lastBlock() const;
  unsigned int difficulty() const;

  // NOLINTNEXTLINE(readability-identifier-naming, google-runtime-references)
  friend void to_json(nlohmann::json& j, const BlockChain& obj);

  // NOLINTNEXTLINE(readability-identifier-naming, google-runtime-references)
  friend void from_json(const nlohmann::json& j, BlockChain& obj);

 private:
  BlockPtr createBlock();

  unsigned int difficulty_;
  std::filesystem::path blockDir_;
  std::map<unsigned int, BlockPtr> blocks_;

  const std::string kDifficulty_ = "difficulty";
  const std::string kLastBlock_ = "last_block";
  const std::string kMetadata_ = "metadata";

  static BlockChainUPtr loadFromJSON(std::filesystem::path path);
};

// NOLINTNEXTLINE(readability-identifier-naming, google-runtime-references)
inline void to_json(nlohmann::json& j, const BlockChain& obj) {
  j = {
    {obj.kLastBlock_, obj.lastBlock()->id()},
    {obj.kDifficulty_, obj.difficulty()},
  };
}

// NOLINTNEXTLINE(readability-identifier-naming, google-runtime-references)
inline void from_json(const nlohmann::json& j, BlockChain& obj) {
  unsigned int lastBlock;
  j[obj.kLastBlock_].get_to(lastBlock);
  j[obj.kDifficulty_].get_to(obj.difficulty_);
  obj.blocks_.insert(std::pair<unsigned int, BlockPtr>(lastBlock, nullptr));
}

} // namespace Miner

#endif // MINER_BLOCKCHAIN_HPP
