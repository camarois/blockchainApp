#include <exception>
#include <fstream>
#include <iostream>

#include "miner/blockchain.hpp"

namespace Miner {

const std::string BlockChain::kMetadata = "metadata";

BlockChain::BlockChain() {
  difficulty_ = 3;
  createBlock();
}

BlockChain::BlockChain(const std::filesystem::path& blockDir) : BlockChain() { blockDir_ = blockDir; }

std::optional<BlockChain> BlockChain::fromDirectory(const std::filesystem::path& blockDir) {
  if (blockDir.empty()) {
    return BlockChain(blockDir);
  }

  if (!std::filesystem::exists(blockDir / BlockChain::kMetadata)) {
    return BlockChain(blockDir);
  }

  std::optional<BlockChain> blockchain = loadMetadata(blockDir);
  if (!blockchain.has_value()) {
    return {};
  }

  unsigned int lastBlockID = blockchain->blocks().rbegin()->first;
  if (blockchain->getBlock(lastBlockID) == nullptr) {
    return {};
  }

  return blockchain;
}

void BlockChain::appendTransaction(const std::string& transaction) { lastBlock()->append(transaction); }

void BlockChain::saveAll() const {
  lastBlock()->save(blockDir_);
  saveMetadata();
}

std::shared_ptr<Block> BlockChain::nextBlock() {
  lastBlock()->mine(difficulty_);
  lastBlock()->save(blockDir_);

  return createBlock();
}

std::shared_ptr<Block> BlockChain::lastBlock() const {
  if (blocks_.empty()) {
    return nullptr;
  }

  return blocks_.rbegin()->second;
}

std::shared_ptr<Block> BlockChain::getBlock(unsigned int id) {
  try {
    std::shared_ptr<Block> block = blocks_.at(id);
    if (block != nullptr) {
      return block;
    }
  } catch (const std::out_of_range& e) {
  }

  return loadBlock(id);
}

const std::map<unsigned int, std::shared_ptr<Block>>& BlockChain::blocks() { return blocks_; }

unsigned int BlockChain::difficulty() const { return difficulty_; }

std::shared_ptr<Block> BlockChain::createBlock() {
  unsigned int nextID;
  std::string previousHash;

  std::shared_ptr<Block> last = lastBlock();
  if (last == nullptr) {
    nextID = 0;
    previousHash = "";
  } else {
    nextID = last->id() + 1;
    previousHash = last->hash();
  }

  std::shared_ptr<Block> block = std::make_shared<Block>(nextID, previousHash);
  blocks_.insert(std::pair<unsigned int, std::shared_ptr<Block>>(block->id(), block));

  return block;
}

std::shared_ptr<Block> BlockChain::loadBlock(unsigned int id) {
  std::filesystem::path blockPath(blockDir_ / std::to_string(id));

  std::shared_ptr<Block> block = Block::fromBlockFile(blockPath);
  if (block == nullptr) {
    std::cerr << "block #" << std::to_string(id) << " doesn't exist" << std::endl;
    return nullptr;
  }

  if (block->id() != id) {
    std::cerr << "mismatch ID in block #" << std::to_string(id) << std::endl;
    return nullptr;
  }

  blocks_.erase(block->id());
  blocks_.insert(std::pair<unsigned int, std::shared_ptr<Block>>(block->id(), block));
  return block;
}

bool BlockChain::saveMetadata() const {
  std::ofstream metadataFile(blockDir_ / BlockChain::kMetadata, std::ofstream::out);
  if (metadataFile.fail()) {
    std::cerr << "blockchain: failed to open metadata in `" << blockDir_ << "`" << std::endl;
    return false;
  }

  std::string json = static_cast<nlohmann::json>(*this).dump();
  metadataFile << json;
  metadataFile.close();

  return true;
}

std::optional<BlockChain> BlockChain::loadMetadata(const std::filesystem::path& blockDir) {
  std::ifstream metadataFile(blockDir / BlockChain::kMetadata, std::ifstream::in);
  if (metadataFile.fail()) {
    std::cerr << "blockchain: failed to open metadata in `" << blockDir << "`" << std::endl;
    return {};
  }

  nlohmann::json json;
  metadataFile >> json;
  metadataFile.close();

  std::optional<BlockChain> metadata = std::make_optional<BlockChain>(json.get<BlockChain>());
  metadata->blockDir_ = blockDir;

  return metadata;
}

// NOLINTNEXTLINE(readability-identifier-naming, google-runtime-references)
inline void to_json(nlohmann::json& j, const BlockChain& obj) {
  j = {
      {obj.kLastBlock_, obj.lastBlock()->id()},
      {obj.kDifficulty_, obj.difficulty()},
  };
}

// NOLINTNEXTLINE(readability-identifier-naming, google-runtime-references)
inline void from_json(const nlohmann::json& j, BlockChain& obj) {
  unsigned int lastBlock;
  j[obj.kLastBlock_].get_to(lastBlock);
  j[obj.kDifficulty_].get_to(obj.difficulty_);
  obj.blocks_.insert(std::pair<unsigned int, std::shared_ptr<Block>>(lastBlock, nullptr));
}

}  // namespace Miner
