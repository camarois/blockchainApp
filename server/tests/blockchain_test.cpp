#include <cstdlib>
#include <exception>
#include <filesystem>
#include <gtest/gtest.h>
#include <miner/blockchain.hpp>

#include "utils.hpp"

TEST(BlockChainTest, loadFromEmptyDir) {
  std::filesystem::path blockDir = Tests::createEmptyDir("blockchain/load-from-empty-dir");
  Miner::BlockChain loaded(blockDir);

  ASSERT_EQ(loaded.difficulty(), 3);
  ASSERT_EQ(loaded.lastBlock()->getID(), 0);
}

TEST(BlockChainTest, loadWithoutMetadata) {
  std::filesystem::path blockDir = Tests::getDir("blockchain/load-without-metadata");
  Miner::BlockChain loaded(blockDir);

  ASSERT_EQ(loaded.difficulty(), 3);
  ASSERT_EQ(loaded.lastBlock()->getID(), 0);
}

TEST(BlockChainTest, loadWithMissingBlock) {
  std::filesystem::path blockDir = Tests::getDir("blockchain/load-with-missing-block");

  ASSERT_THROW(Miner::BlockChain loaded(blockDir), std::runtime_error);
}

TEST(BlockChainTest, loadWithMismatchID) {
  std::filesystem::path blockDir = Tests::getDir("blockchain/load-with-mismatch-id");

  ASSERT_THROW(Miner::BlockChain loaded(blockDir), std::runtime_error);
}

TEST(BlockChainTest, loadFromDir) {
  std::filesystem::path blockDir = Tests::getDir("blockchain/load-from-file");
  Miner::BlockChain loaded(blockDir);

  std::string previous("eed82d9682fb1a4a37ecceba76052738f687f3c5e5fd317c940471e8413d140f");
  ASSERT_EQ(loaded.difficulty(), 10);
  ASSERT_EQ(loaded.lastBlock()->getID(), 420);
  ASSERT_EQ(loaded.lastBlock()->getNonce(), 0);
  ASSERT_EQ(loaded.lastBlock()->getPreviousHash(), previous);
  ASSERT_EQ(loaded.lastBlock()->getData()[0], "Kathy Brousseau");
  ASSERT_EQ(loaded.lastBlock()->getData()[1], "Aya Bordeleau");
  ASSERT_EQ(loaded.lastBlock()->getData()[2], "Jeffrey Brisebois");
}
