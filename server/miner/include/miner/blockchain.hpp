#ifndef MINER_BLOCKCHAIN_HPP
#define MINER_BLOCKCHAIN_HPP

#include <exception>
#include <filesystem>
#include <list>
#include <optional>

#include "common/optional_ref.hpp"
#include "miner/block.hpp"

namespace Miner {

class BlockChain {
 public:
  BlockChain();
  explicit BlockChain(const std::filesystem::path& blockDir);
  static std::optional<BlockChain> fromDirectory(const std::filesystem::path& blockDir);

  void addTransaction(const std::string& transaction);
  void saveAll();
  void clearAll();
  // Block& nextBlock();
  Common::optional_ref<Block> lastBlock();
  Common::optional_ref<Block> getBlock(unsigned int id);
  int lastBlockID() const;
  unsigned int difficulty() const;
  const std::map<unsigned int, Block>& blocks();

  // NOLINTNEXTLINE(readability-identifier-naming, google-runtime-references)
  friend void to_json(nlohmann::json& j, const BlockChain& obj);

  // NOLINTNEXTLINE(readability-identifier-naming, google-runtime-references)
  friend void from_json(const nlohmann::json& j, BlockChain& obj);

  static const std::string kMetadataFilename;

 private:
  void createBlock(const std::string& data);
  Common::optional_ref<Block> loadBlock(unsigned int id);
  bool saveMetadata() const;
  static std::optional<BlockChain> loadMetadataBlockChain(const std::filesystem::path& blockDir);

  unsigned int difficulty_;
  std::filesystem::path blockDir_;
  std::map<unsigned int, Block> blocks_;

  const std::string kDifficulty_ = "difficulty";
  const std::string kLastBlock_ = "last_block";
};

} // namespace Miner

#endif // MINER_BLOCKCHAIN_HPP
