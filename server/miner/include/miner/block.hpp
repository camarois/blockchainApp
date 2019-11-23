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
  explicit Block(int id, const std::string& previous);
  static std::optional<Block> fromBlockFile(const std::filesystem::path& blockDir);

  void setData(const std::string& data);
  void mine(int difficulty);
  void save() const;
  void queueNonce(int nonce);
  void increaseVerification();

  int id() const;
  int nonce() const;
  int numberOfVerifications() const;
  std::string hash();
  std::string previousHash() const;
  std::string data() const;

  // NOLINTNEXTLINE(readability-identifier-naming, google-runtime-references)
  friend void to_json(nlohmann::json& j, const Block& obj);

  // NOLINTNEXTLINE(readability-identifier-naming, google-runtime-references)
  friend void from_json(const nlohmann::json& j, Block& obj);

 private:
  bool dirty_;
  int id_;
  int nonce_;
  int numberOfVerifications_;
  std::string hash_;
  std::string previousHash_;
  std::string data_;
  std::queue<int> receivedNonces_;
  std::filesystem::path blockDir_;

  const std::string kId_ = "id";
  const std::string kNonce_ = "nonce";
  const std::string kPreviousHash_ = "previous_hash";
  const std::string kData_ = "data";
  const std::string kNumberOfVerifications_ = "number_of_verifications";
};

} // namespace Miner

#endif // MINER_BLOCK_HPP
