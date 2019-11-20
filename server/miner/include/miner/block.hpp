#ifndef MINER_BLOCK_HPP
#define MINER_BLOCK_HPP

#include <filesystem>
#include <nlohmann/json.hpp>
#include <optional>
#include <queue>
#include <string>
#include <vector>

namespace Miner {

class Block {
 public:
  Block();
  explicit Block(unsigned int id, const std::string& previous);
  static std::optional<Block> fromBlockFile(const std::filesystem::path& blockDir);

  void append(const std::string& data);
  void mine(unsigned int difficulty);
  void save() const;
  void queueNonce(unsigned int nonce);
  void increaseVerification();

  unsigned int id() const;
  unsigned int nonce() const;
  unsigned int numberOfVerifications() const;
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
  unsigned int numberOfVerifications_;
  std::string hash_;
  std::string previousHash_;
  std::vector<std::string> data_;
  std::queue<unsigned int> receivedNonces_;
  std::filesystem::path blockDir_;

  const std::string kId_ = "id";
  const std::string kNonce_ = "nonce";
  const std::string kPreviousHash_ = "previous_hash";
  const std::string kData_ = "data";
  const std::string kNumberOfVerifications_ = "number_of_verifications";
};

} // namespace Miner

#endif // MINER_BLOCK_HPP
