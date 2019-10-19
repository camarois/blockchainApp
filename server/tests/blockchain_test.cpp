#include <cstdlib>
#include <exception>
#include <filesystem>
#include <gtest/gtest.h>
#include <miner/blockchain.hpp>

#include "utils.hpp"

TEST(BlockChainTest, loadFromEmptyDir) {
  std::filesystem::path blockDir = Tests::createEmptyDir("blockchain/load-from-empty-dir");
  Miner::BlockChain loaded(blockDir);

  ASSERT_EQ(loaded.difficulty(), 3U);
  ASSERT_EQ(loaded.lastBlock()->id(), 0U);
}

TEST(BlockChainTest, loadWithoutMetadata) {
  std::filesystem::path blockDir = Tests::getDir("blockchain/load-without-metadata");
  Miner::BlockChain loaded(blockDir);

  ASSERT_EQ(loaded.difficulty(), 3U);
  ASSERT_EQ(loaded.lastBlock()->id(), 0U);
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
  ASSERT_EQ(loaded.difficulty(), 10U);
  ASSERT_EQ(loaded.lastBlock()->id(), 420U);
  ASSERT_EQ(loaded.lastBlock()->nonce(), 0U);
  ASSERT_EQ(loaded.lastBlock()->previousHash(), previous);
  ASSERT_EQ(loaded.lastBlock()->data()[0], "Kathy Brousseau");
  ASSERT_EQ(loaded.lastBlock()->data()[1], "Aya Bordeleau");
  ASSERT_EQ(loaded.lastBlock()->data()[2], "Jeffrey Brisebois");
}
