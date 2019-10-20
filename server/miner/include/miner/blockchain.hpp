#ifndef MINER_BLOCKCHAIN_HPP
#define MINER_BLOCKCHAIN_HPP

#include <exception>
#include <filesystem>
#include <list>
#include <memory>

#include "miner/block.hpp"

namespace Miner {

class BlockChain;

using BlockChainUPtr = std::unique_ptr<BlockChain>;

class BlockChain {
 public:
  BlockChain();
  explicit BlockChain(const std::filesystem::path& blockDir);
  static BlockChainUPtr fromDirectory(const std::filesystem::path& blockDir);

  void appendTransaction(const std::string& transaction);
  void saveAll() const;
  BlockPtr nextBlock();
  BlockPtr lastBlock() const;
  BlockPtr getBlock(unsigned int id);
  unsigned int difficulty() const;
  const std::map<unsigned int, BlockPtr>& blocks();

  // NOLINTNEXTLINE(readability-identifier-naming, google-runtime-references)
  friend void to_json(nlohmann::json& j, const BlockChain& obj);

  // NOLINTNEXTLINE(readability-identifier-naming, google-runtime-references)
  friend void from_json(const nlohmann::json& j, BlockChain& obj);

 private:
  BlockPtr createBlock();
  BlockPtr loadBlock(unsigned int id);
  bool saveMetadata() const;
  static BlockChainUPtr loadMetadata(const std::filesystem::path& blockDir);

  unsigned int difficulty_;
  std::filesystem::path blockDir_;
  std::map<unsigned int, BlockPtr> blocks_;

  const std::string kDifficulty_ = "difficulty";
  const std::string kLastBlock_ = "last_block";
};

} // namespace Miner

#endif // MINER_BLOCKCHAIN_HPP
