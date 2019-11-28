#include "miner/blockchain.hpp"

#include "common/gflags_helper.hpp"
#include "common/logger.hpp"
#include <exception>
#include <fstream>
#include <iostream>

DECLARE_int32(difficulty);

namespace Miner {

const std::string BlockChain::kMetadataFilename = "metadata";

BlockChain::BlockChain() {
  difficulty_ = FLAGS_difficulty;
  Common::Logger::get()->info("Starting blockchain with difficulty " + std::to_string(difficulty_) + "\n");
}

BlockChain::BlockChain(const std::filesystem::path& blockDir) : BlockChain() { blockDir_ = blockDir; }

std::optional<BlockChain> BlockChain::fromDirectory(const std::filesystem::path& blockDir) {
  std::filesystem::create_directories(blockDir);
  if (blockDir.empty()) {
    return BlockChain(blockDir);
  }

  if (!std::filesystem::exists(blockDir / BlockChain::kMetadataFilename)) {
    return BlockChain(blockDir);
  }

  std::optional<BlockChain> blockchain = loadMetadataBlockChain(blockDir);
  if (!blockchain) {
    return {};
  }

  int lastBlockID = blockchain->lastBlockID();
  blockchain->clearAll();
  if (!blockchain->getBlock(lastBlockID)) {
    return {};
  }

  return blockchain;
}

void BlockChain::addTransaction(const std::string& transaction, int nonce) {
  createBlock(transaction);
  std::cout << "Queuing nonce " << nonce << std::endl;
  lastBlock()->get().queueNonce(nonce);
  lastBlock()->get().mine(difficulty_);
  saveAll();
}

void BlockChain::saveAll() {
  Common::optional_ref<Block> last = lastBlock();
  if (last) {
    lastBlock()->get().save();
  }
  saveMetadata();
}

void BlockChain::clearAll() { blocks_.clear(); }

Common::optional_ref<Block> BlockChain::lastBlock() {
  if (blocks_.empty()) {
    return {};
  }

  return blocks_.rbegin()->second;
}

Common::optional_ref<Block> BlockChain::getBlock(int id) {
  auto it = blocks_.find(id);
  if (it == blocks_.end()) {
    return loadBlock(id);
  }

  return it->second;
}

int BlockChain::lastBlockID() const { return blocks_.empty() ? -1 : blocks_.rbegin()->first; }

const std::map<int, Block>& BlockChain::blocks() { return blocks_; }

int BlockChain::difficulty() const { return difficulty_; }

void BlockChain::createBlock(const std::string& data) {
  int nextID = 0;
  std::string previousHash;

  Common::optional_ref<Block> last = lastBlock();
  if (last) {
    nextID = last->get().id() + 1;
    previousHash = last->get().hash();
  }

  Block block(nextID, previousHash);
  block.setData(data);
  blocks_.emplace(nextID, block);
}

Common::optional_ref<Block> BlockChain::loadBlock(int id) {
  std::filesystem::path blockPath(blockDir_ / std::to_string(id));

  std::optional<Block> block = Block::fromBlockFile(blockPath);
  if (!block) {
    Common::Logger::get()->error("Block #" + std::to_string(id) + " doesn't exist\n");
    return {};
  }

  if (block->id() != id) {
    Common::Logger::get()->error("Mismatch ID in block #" + std::to_string(id) + "\n");
    return {};
  }

  blocks_.erase(block->id());
  blocks_.emplace(block->id(), block.value());
  return block.value();
}

bool BlockChain::saveMetadata() const {
  std::ofstream metadataFile(blockDir_ / BlockChain::kMetadataFilename, std::ofstream::out);
  if (metadataFile.fail()) {
    Common::Logger::get()->error("Blockchain: failed to open metadata in `" + std::string(blockDir_) + "`\n");
    return false;
  }

  std::string json = static_cast<nlohmann::json>(*this).dump();
  metadataFile << json;
  metadataFile.close();

  return true;
}

std::optional<BlockChain> BlockChain::loadMetadataBlockChain(const std::filesystem::path& blockDir) {
  std::ifstream metadataFile(blockDir / BlockChain::kMetadataFilename, std::ifstream::in);
  if (metadataFile.fail()) {
    Common::Logger::get()->error("Blockchain: failed to open metadata in `" + std::string(blockDir) + "`\n");
    return {};
  }

  nlohmann::json json;
  metadataFile >> json;
  metadataFile.close();

  std::optional<BlockChain> metadata = json.get<BlockChain>();
  metadata->blockDir_ = blockDir;

  return metadata;
}

// NOLINTNEXTLINE(readability-identifier-naming, google-runtime-references)
inline void to_json(nlohmann::json& j, const BlockChain& obj) {
  j = {
      {obj.kLastBlock_, obj.lastBlockID()},
      {obj.kDifficulty_, obj.difficulty()},
  };
}

// NOLINTNEXTLINE(readability-identifier-naming, google-runtime-references)
inline void from_json(const nlohmann::json& j, BlockChain& obj) {
  int lastBlock;
  j.at(obj.kLastBlock_).get_to(lastBlock);
  j.at(obj.kDifficulty_).get_to(obj.difficulty_);
  obj.blocks_.insert(std::pair<int, Block>(lastBlock, Block()));
}

}  // namespace Miner
