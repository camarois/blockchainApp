#ifndef MINER_BLOCK_HPP
#define MINER_BLOCK_HPP

#include <filesystem>
#include <memory>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

namespace Miner {

class Block;
using BlockPtr = std::shared_ptr<Block>;

class Block {
 public:
  Block();
  explicit Block(unsigned int id, const std::string& previous);
  explicit Block(const std::filesystem::path& blockDir);
  static BlockPtr fromBlockFile(const std::filesystem::path& blockDir);

  void append(const std::string& data);
  void mine(unsigned int difficulty);
  void save(const std::filesystem::path& blockDir) const;

  unsigned int id() const;
  unsigned int nonce() const;
  std::string hash();
  std::string previousHash() const;
  const std::vector<std::string>& data() const;

  // NOLINTNEXTLINE(readability-identifier-naming, google-runtime-references)
  friend void to_json(nlohmann::json& j, const Block& obj);

  // NOLINTNEXTLINE(readability-identifier-naming, google-runtime-references)
  friend void from_json(const nlohmann::json& j, Block& obj);

 private:
  bool dirty_;
  unsigned int id_;
  unsigned int nonce_;
  std::string hash_;
  std::string previousHash_;
  std::vector<std::string> data_;

  const std::string kId_ = "id";
  const std::string kNonce_ = "nonce";
  const std::string kPreviousHash_ = "previous_hash";
  const std::string kData_ = "data";
};

} // namespace Miner

#endif // MINER_BLOCK_HPP
