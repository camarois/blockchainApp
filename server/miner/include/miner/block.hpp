#ifndef MINER_BLOCK_HPP
#define MINER_BLOCK_HPP

#include <filesystem>
#include <string>
#include <vector>

namespace Miner {

class Block {
 public:
  explicit Block(unsigned int id, std::string previous);
  explicit Block(std::filesystem::path blockPath);

  void append(const std::string& data);
  void mine(int difficulty);
  void save(std::filesystem::path blockDir) const;

  std::string getHash();
  std::string getPreviousHash() const;

 private:
  bool dirty_;
  unsigned int id_;
  unsigned int nonce_;
  std::string hash_;
  std::string previous_hash_;
  std::vector<std::string> data_;
};

} // namespace Miner

#endif // MINER_BLOCK_HPP
