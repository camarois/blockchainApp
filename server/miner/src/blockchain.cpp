#include <exception>
#include <fstream>
#include <iostream>

#include "miner/blockchain.hpp"

static std::string kMetadata = "metadata";

namespace Miner {

BlockChain::BlockChain() {
  difficulty_ = 3;
  createBlock();
}

BlockChain::BlockChain(const std::filesystem::path& blockDir) : BlockChain() { blockDir_ = blockDir; }

BlockChainUPtr BlockChain::fromDirectory(const std::filesystem::path& blockDir) {
  if (blockDir.empty()) {
    return std::make_unique<BlockChain>(blockDir);
  }

  if (!std::filesystem::exists(blockDir / kMetadata)) {
    return std::make_unique<BlockChain>(blockDir);
  }

  BlockChainUPtr blockchain = loadMetadata(blockDir);
  if (blockchain == nullptr) {
    return nullptr;
  }

  unsigned int lastBlockID = blockchain->blocks().rbegin()->first;
  if (blockchain->getBlock(lastBlockID) == nullptr) {
    return nullptr;
  }

  return blockchain;
}

void BlockChain::appendTransaction(const std::string& transaction) { lastBlock()->append(transaction); }

void BlockChain::saveAll() const {
  lastBlock()->save(blockDir_);
  saveMetadata();
}

BlockPtr BlockChain::nextBlock() {
  lastBlock()->mine(difficulty_);
  lastBlock()->save(blockDir_);

  return createBlock();
}

BlockPtr BlockChain::lastBlock() const {
  if (blocks_.empty()) {
    return nullptr;
  }

  return blocks_.rbegin()->second;
}

BlockPtr BlockChain::getBlock(unsigned int id) {
  try {
    BlockPtr block = blocks_.at(id);
    if (block != nullptr) {
      return block;
    }
  } catch (const std::out_of_range& e) {
  }

  return loadBlock(id);
}

const std::map<unsigned int, BlockPtr>& BlockChain::blocks() { return blocks_; }

unsigned int BlockChain::difficulty() const { return difficulty_; }

BlockPtr BlockChain::createBlock() {
  unsigned int nextID;
  std::string previousHash;

  BlockPtr last = lastBlock();
  if (last == nullptr) {
    nextID = 0;
    previousHash = "";
  } else {
    nextID = last->id() + 1;
    previousHash = last->hash();
  }

  BlockPtr block = std::make_shared<Block>(nextID, previousHash);
  blocks_.insert(std::pair<unsigned int, BlockPtr>(block->id(), block));

  return block;
}

BlockPtr BlockChain::loadBlock(unsigned int id) {
  std::filesystem::path blockPath(blockDir_ / std::to_string(id));

  BlockPtr block = Block::fromBlockFile(blockPath);
  if (block == nullptr) {
    std::cerr << "block #" << std::to_string(id) << " doesn't exist" << std::endl;
    return nullptr;
  }

  if (block->id() != id) {
    std::cerr << "mismatch ID in block #" << std::to_string(id) << std::endl;
    return nullptr;
  }

  blocks_.erase(block->id());
  blocks_.insert(std::pair<unsigned int, BlockPtr>(block->id(), block));
  return block;
}

bool BlockChain::saveMetadata() const {
  std::ofstream metadataFile(blockDir_ / kMetadata, std::ofstream::out);
  if (metadataFile.fail()) {
    std::cerr << "blockchain: failed to open metadata in `" << blockDir_ << "`" << std::endl;
    return false;
  }

  std::string json = static_cast<nlohmann::json>(*this).dump();
  metadataFile << json;
  metadataFile.close();

  return true;
}

BlockChainUPtr BlockChain::loadMetadata(const std::filesystem::path& blockDir) {
  std::ifstream metadataFile(blockDir / kMetadata, std::ifstream::in);
  if (metadataFile.fail()) {
    std::cerr << "blockchain: failed to open metadata in `" << blockDir << "`" << std::endl;
    return nullptr;
  }

  nlohmann::json json;
  metadataFile >> json;
  metadataFile.close();

  BlockChainUPtr metadata = std::make_unique<BlockChain>(json.get<BlockChain>());
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
  obj.blocks_.insert(std::pair<unsigned int, BlockPtr>(lastBlock, nullptr));
}

}  // namespace Miner
