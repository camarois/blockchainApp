#ifndef MINER_BLOCKCHAIN_HPP
#define MINER_BLOCKCHAIN_HPP

#include <exception>
#include <filesystem>
#include <list>
#include <memory>
#include <optional>

#include "miner/block.hpp"

namespace Miner {

class BlockChain {
 public:
  BlockChain();
  explicit BlockChain(const std::filesystem::path& blockDir);
  static std::optional<BlockChain> fromDirectory(const std::filesystem::path& blockDir);

  void appendTransaction(const std::string& transaction);
  void saveAll() const;
  std::shared_ptr<Block> nextBlock();
  std::shared_ptr<Block> lastBlock() const;
  std::shared_ptr<Block> getBlock(unsigned int id);
  unsigned int difficulty() const;
  const std::map<unsigned int, std::shared_ptr<Block>>& blocks();

  // NOLINTNEXTLINE(readability-identifier-naming, google-runtime-references)
  friend void to_json(nlohmann::json& j, const BlockChain& obj);

  // NOLINTNEXTLINE(readability-identifier-naming, google-runtime-references)
  friend void from_json(const nlohmann::json& j, BlockChain& obj);

  static const std::string kMetadata;

 private:
  std::shared_ptr<Block> createBlock();
  std::shared_ptr<Block> loadBlock(unsigned int id);
  bool saveMetadata() const;
  static std::optional<BlockChain> loadMetadata(const std::filesystem::path& blockDir);

  unsigned int difficulty_;
  std::filesystem::path blockDir_;
  std::map<unsigned int, std::shared_ptr<Block>> blocks_;

  const std::string kDifficulty_ = "difficulty";
  const std::string kLastBlock_ = "last_block";
};

} // namespace Miner

#endif // MINER_BLOCKCHAIN_HPP
