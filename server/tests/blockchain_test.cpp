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

TEST(BlockChainTest, createBlocksAndSave) {
  std::filesystem::path blockDir = Tests::createEmptyDir("blockchain/create-blocks-and-save");
  Miner::BlockChain blockchain(blockDir);
  Miner::BlockPtr last;

  last = blockchain.lastBlock();
  blockchain.appendTransaction("Éva Duchesne");
  blockchain.appendTransaction("Natacha Forest");
  blockchain.appendTransaction("Xavier Lalancette");
  blockchain.nextBlock();
  std::string block0Hash("000034f32b0855161a1c2b5db42bff42ef6e7c704b600dcb4e439b83caae0196");

  ASSERT_EQ(last->id(), 0U);
  ASSERT_EQ(last->nonce(), 3091U);
  ASSERT_EQ(last->hash(), block0Hash);
  last = blockchain.lastBlock();

  blockchain.appendTransaction("Marc-Olivier Arsenault");
  blockchain.appendTransaction("Stéphanie Lacerte");
  blockchain.appendTransaction("Zachary Rousseau");
  blockchain.nextBlock();
  std::string block1Hash("0008756f8bfbf6166d3b8030e7a98c0ec7c58cff02c9458c4bed6dc46611a3b6");

  ASSERT_EQ(last->id(), 1U);
  ASSERT_EQ(last->nonce(), 6681U);
  ASSERT_EQ(last->hash(), block1Hash);

  blockchain.saveAll();
  std::string block2Hash("a615db4568289dfb5686f40618e333fd867c009a7997bc97ecafd53b01e42e13");
  std::string metadataHash("f318bef5341fd7cd0baeee4739e0f7dccaf70e8f3b0581e321c11e7c639f576b");

  ASSERT_TRUE(Tests::checkFileSHA256Hash(blockDir / "0", block0Hash));
  ASSERT_TRUE(Tests::checkFileSHA256Hash(blockDir / "1", block1Hash));
  ASSERT_TRUE(Tests::checkFileSHA256Hash(blockDir / "2", block2Hash));
  ASSERT_TRUE(Tests::checkFileSHA256Hash(blockDir / "metadata", metadataHash));
}
