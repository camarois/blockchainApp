#ifndef MINER_BLOCK_HPP
#define MINER_BLOCK_HPP

#include <string>
#include <vector>

class Block {
 public:
  explicit Block(std::string previous);
  explicit Block(std::string previous, int nonce, std::vector<std::string> data);

  void append(const std::string& data);
  void mine(int difficulty);

  std::string getHash();
  std::string getPreviousHash() const;

 private:
  bool dirty_;
  int nonce_;
  std::string hash_;
  std::string previous_hash_;
  std::vector<std::string> data_;
};

#endif // MINER_BLOCK_HPP
