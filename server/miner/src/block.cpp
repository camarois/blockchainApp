#include <algorithm>
#include <exception>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <miner/block.hpp>
#include <nlohmann/json.hpp>
#include <picosha2.h>
#include <sstream>

namespace Miner {

Block::Block() {
  id_ = 0;
  dirty_ = true;
  nonce_ = 0;
}

Block::Block(unsigned int id, std::string previous) : Block() {
  id_ = id;
  previousHash_ = previous;
}

Block::Block(std::filesystem::path blockPath) : Block() {
  std::ifstream blockFile(blockPath, std::ifstream::in);
  if (blockFile.fail()) {
    throw std::invalid_argument("couldn't open `" + blockPath.string() + "`");
  }

  nlohmann::json json;
  blockFile >> json;
  blockFile.close();

  auto parsed = json.get<Block>();
  id_ = parsed.id();
  nonce_ = parsed.nonce();
  previousHash_ = parsed.previousHash();
  data_ = parsed.data();
}

void Block::append(const std::string& data) {
  dirty_ = true;
  data_.push_back(data);
}

void Block::mine(int difficulty) {
  nonce_ = 0;
  while (true) {
    std::string blockHash = hash();

    bool invalid = false;
    for (int i = 0; i < difficulty; i++) {
      if (blockHash[i] != '0') {
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

void Block::save(std::filesystem::path blockDir) const {
  blockDir.append(std::to_string(id_));
  std::ofstream file(blockDir, std::ofstream::out);
  std::string json = static_cast<nlohmann::json>(*this).dump();
  file << json;
  file.close();
}

unsigned int Block::id() const { return id_; }

unsigned int Block::nonce() const { return nonce_; }

std::string Block::hash() {
  if (!dirty_) {
    return hash_;
  }

  std::string json = static_cast<nlohmann::json>(*this).dump();
  std::vector<unsigned char> hash(picosha2::k_digest_size);
  picosha2::hash256(json.begin(), json.end(), hash.begin(), hash.end());
  hash_ = picosha2::bytes_to_hex_string(hash.begin(), hash.end());
  dirty_ = false;

  return hash_;
}

std::string Block::previousHash() const { return previousHash_; }

const std::vector<std::string>& Block::data() const { return data_; }

}  // namespace Miner
