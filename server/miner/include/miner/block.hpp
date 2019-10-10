#ifndef MINER_BLOCK_HPP
#define MINER_BLOCK_HPP

#include <filesystem>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

namespace Miner {

class Block {
 public:
  Block();
  explicit Block(unsigned int id, std::string previous);
  explicit Block(std::filesystem::path blockPath);

  void append(const std::string& data);
  void mine(int difficulty);
  void save(std::filesystem::path blockDir) const;

  unsigned int getID() const;
  unsigned int getNonce() const;
  std::string getHash();
  std::string getPreviousHash() const;
  const std::vector<std::string>& getData() const;

  // NOLINTNEXTLINE(readability-identifier-naming, google-runtime-references)
  friend void to_json(nlohmann::json& j, const Block& obj);

  // NOLINTNEXTLINE(readability-identifier-naming, google-runtime-references)
  friend void from_json(const nlohmann::json& j, Block& obj);

 private:
  bool dirty_;
  unsigned int id_;
  unsigned int nonce_;
  std::string hash_;
  std::string previous_hash_;
  std::vector<std::string> data_;

  const std::string kId_ = "id";
  const std::string kNonce_ = "nonce";
  const std::string kPreviousHash_ = "previous_hash";
  const std::string kData_ = "data";
};

// NOLINTNEXTLINE(readability-identifier-naming, google-runtime-references)
inline void to_json(nlohmann::json& j, const Block& obj) {
  j = {
    {obj.kId_, obj.getID()},
    {obj.kNonce_, obj.getNonce()},
    {obj.kPreviousHash_, obj.getPreviousHash()},
    {obj.kData_, obj.getData()},
  };
}

// NOLINTNEXTLINE(readability-identifier-naming, google-runtime-references)
inline void from_json(const nlohmann::json& j, Block& obj) {
  obj.dirty_ = true;
  j[obj.kId_].get_to(obj.id_);
  j[obj.kNonce_].get_to(obj.nonce_);
  j[obj.kPreviousHash_].get_to(obj.previous_hash_);
  j[obj.kData_].get_to(obj.data_);
}

} // namespace Miner

#endif // MINER_BLOCK_HPP
