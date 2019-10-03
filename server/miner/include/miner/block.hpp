#ifndef MINER_BLOCK_HPP
#define MINER_BLOCK_HPP

#include <vector>
#include <string>

class Block {
 public:
  Block(std::string previous);
  Block(std::string previous, int nonce, std::vector<std::string> data);

  void AppendData(std::string data);
  void Mine(int difficulty);

  std::string get_hash();
  std::string get_previous_hash() const;

 private:
  bool dirty_;
  int nonce_;
  std::string hash_;
  std::string previous_hash_;
  std::vector<std::string> data_;
};

#endif // MINER_BLOCK_HPP
