#include <fstream>

#include "miner/blockchain.hpp"

namespace Miner {

BlockChain::BlockChain() {
  difficulty_ = 3;
  createBlock();
}

BlockChain::BlockChain(std::filesystem::path blockDir) {
  blockDir_ = blockDir;

  if (blockDir_.empty()) {
    difficulty_ = 3;
    createBlock();
    return;
  }

  std::filesystem::path metadataPath(blockDir_ / kMetadata_);
  if (!std::filesystem::exists(metadataPath)) {
    difficulty_ = 3;
    createBlock();
    return;
  }

  BlockChainUPtr metadata = loadFromJSON(metadataPath);
  difficulty_ = metadata->difficulty_;
  unsigned int lastBlockID = metadata->blocks_.rbegin()->first;

  std::filesystem::path blockPath(blockDir_ / std::to_string(lastBlockID));

  BlockPtr lastBlock;
  try {
    lastBlock = std::make_shared<Block>(blockPath);
  } catch (const std::invalid_argument& e) {
    throw std::runtime_error("block #" + std::to_string(lastBlockID) + " doesn't exist");
  }

  if (lastBlock->id() != lastBlockID) {
    throw std::runtime_error("mismatch ID in block #" + std::to_string(lastBlockID));
  }

  blocks_.insert(std::pair<unsigned int, BlockPtr>(lastBlock->id(), lastBlock));
}

void BlockChain::appendTransaction(std::string transaction) { lastBlock()->append(transaction); }

void BlockChain::saveAll() const {
  lastBlock()->save(blockDir_);
  // TODO(gabriel): save JSON
}

BlockPtr BlockChain::nextBlock() {
  lastBlock()->mine(difficulty_);

  return createBlock();
}

BlockPtr BlockChain::lastBlock() const {
  if (blocks_.empty()) {
    return nullptr;
  }

  return blocks_.rbegin()->second;
}

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

BlockChainUPtr BlockChain::loadFromJSON(std::filesystem::path metadataPath) {
  std::ifstream metadataFile(metadataPath, std::ifstream::in);
  if (metadataFile.fail()) {
    return nullptr;
  }

  nlohmann::json json;
  metadataFile >> json;
  metadataFile.close();

  return std::make_unique<BlockChain>(json.get<BlockChain>());
}

}  // namespace Miner
