#include <sstream>
#include <iostream>
#include <algorithm>
#include <miner/block.hpp>
#include <nlohmann/json.hpp>
#include <picosha2.h>

Block::Block(std::string previous) {
  dirty_ = true;
  nonce_ = 0;
  previous_hash_ = previous;
}

Block::Block(std::string previous, int nonce, std::vector<std::string> data) : Block(previous) {
  nonce_ = nonce;
  data_ = data;
}

void Block::AppendData(std::string data) {
  dirty_ = true;
}

void Block::Mine(int difficulty) {
  assert(difficulty > 0);
  assert(difficulty < 64);

  nonce_ = 0;
  while(1) {
    std::string hash = get_hash();
    assert(hash.length() == 64);

    bool invalid = false;
    for(int i = 0; i < difficulty; i++) {
      if(hash[i] != '0') {
        nonce_++;
        dirty_ = true;
        invalid = true;
      }
    }

    if(invalid) {
        continue;
    } else {
        break;
    }
  }
}

std::string Block::get_hash() {
  if(!dirty_) {
    return hash_;
  }

  std::stringstream stream;
  stream << nonce_ << previous_hash_;
  for (auto str = data_.begin(); str != data_.end(); ++str) {
    stream << *str;
  }
  std::string str = stream.str();

  std::vector<unsigned char> hash(picosha2::k_digest_size);
  picosha2::hash256(str.begin(), str.end(), hash.begin(), hash.end());
  hash_ = picosha2::bytes_to_hex_string(hash.begin(), hash.end());
  dirty_ = false;

  return hash_;
}

std::string Block::get_previous_hash() const {
  return previous_hash_;
}
