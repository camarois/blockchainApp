#include <algorithm>
#include <iostream>
#include <miner/block.hpp>
#include <nlohmann/json.hpp>
#include <picosha2.h>
#include <sstream>

Block::Block(std::string previous) {
  dirty_ = true;
  nonce_ = 0;
  previous_hash_ = previous;
}

Block::Block(std::string previous, int nonce, std::vector<std::string> data) : Block(previous) {
  nonce_ = nonce;
  data_ = data;
}

void Block::append(const std::string& data) {
  dirty_ = true;
  data_.push_back(data);
}

void Block::mine(int difficulty) {
  nonce_ = 0;
  while (true) {
    std::string hash = getHash();

    bool invalid = false;
    for (int i = 0; i < difficulty; i++) {
      if (hash[i] != '0') {
	nonce_++;
	dirty_ = true;
	invalid = true;
      }
    }

    if (invalid) {
      continue;
    }

    break;
  }
}

std::string Block::getHash() {
  if (!dirty_) {
    return hash_;
  }

  std::stringstream stream;
  stream << nonce_ << previous_hash_;
  for (std::string& str : data_) {
    stream << str;
  }
  std::string str = stream.str();

  std::vector<unsigned char> hash(picosha2::k_digest_size);
  picosha2::hash256(str.begin(), str.end(), hash.begin(), hash.end());
  hash_ = picosha2::bytes_to_hex_string(hash.begin(), hash.end());
  dirty_ = false;

  return hash_;
}

std::string Block::getPreviousHash() const { return previous_hash_; }
