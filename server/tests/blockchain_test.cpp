#include <cstdlib>
#include <exception>
#include <filesystem>
#include <gtest/gtest.h>
#include <miner/blockchain.hpp>

#include "utils.hpp"

TEST(BlockChainTest, loadFromEmptyDir) {
  std::filesystem::path blockDir = Tests::createEmptyDir("blockchain/load-from-empty-dir");
  std::optional<Miner::BlockChain> blockchain = Miner::BlockChain::fromDirectory(blockDir);

  ASSERT_TRUE(blockchain);
  ASSERT_TRUE(blockchain->lastBlock());
  ASSERT_EQ(blockchain->lastBlock()->get().id(), 0U);
  ASSERT_EQ(blockchain->difficulty(), 3U);
}

TEST(BlockChainTest, loadWithoutMetadata) {
  std::filesystem::path blockDir = Tests::getDir("blockchain/load-without-metadata");
  std::optional<Miner::BlockChain> blockchain = Miner::BlockChain::fromDirectory(blockDir);

  ASSERT_TRUE(blockchain.has_value());
  ASSERT_TRUE(blockchain->lastBlock());
  ASSERT_EQ(blockchain->lastBlock()->get().id(), 0U);
  ASSERT_EQ(blockchain->difficulty(), 3U);
}

TEST(BlockChainTest, loadWithMissingBlock) {
  std::filesystem::path blockDir = Tests::getDir("blockchain/load-with-missing-block");
  std::optional<Miner::BlockChain> blockchain = Miner::BlockChain::fromDirectory(blockDir);

  ASSERT_FALSE(blockchain);
}

TEST(BlockChainTest, loadWithMismatchID) {
  std::filesystem::path blockDir = Tests::getDir("blockchain/load-with-mismatch-id");
  std::optional<Miner::BlockChain> blockchain = Miner::BlockChain::fromDirectory(blockDir);

  ASSERT_FALSE(blockchain);
}

TEST(BlockChainTest, loadFromDir) {
  std::filesystem::path blockDir = Tests::getDir("blockchain/load-from-dir");
  std::optional<Miner::BlockChain> blockchain = Miner::BlockChain::fromDirectory(blockDir);
  ASSERT_TRUE(blockchain);
  ASSERT_TRUE(blockchain->lastBlock());

  std::string previous("eed82d9682fb1a4a37ecceba76052738f687f3c5e5fd317c940471e8413d140f");
  ASSERT_EQ(blockchain->difficulty(), 10U);
  ASSERT_EQ(blockchain->lastBlock()->get().id(), 420U);
  ASSERT_EQ(blockchain->lastBlock()->get().nonce(), 0U);
  ASSERT_EQ(blockchain->lastBlock()->get().previousHash(), previous);
  ASSERT_EQ(blockchain->lastBlock()->get().data()[0], "Kathy Brousseau");
  ASSERT_EQ(blockchain->lastBlock()->get().data()[1], "Aya Bordeleau");
  ASSERT_EQ(blockchain->lastBlock()->get().data()[2], "Jeffrey Brisebois");
}

TEST(BlockChainTest, createBlocksAndSave) {
  std::filesystem::path blockDir = Tests::createEmptyDir("blockchain/create-blocks-and-save");
  std::optional<Miner::BlockChain> blockchain = Miner::BlockChain::fromDirectory(blockDir);
  ASSERT_TRUE(blockchain);
  ASSERT_TRUE(blockchain->lastBlock());

  Common::optional_ref<Miner::Block> last1 = blockchain->lastBlock();
  blockchain->appendTransaction("Éva Duchesne");
  blockchain->appendTransaction("Natacha Forest");
  blockchain->appendTransaction("Xavier Lalancette");
  blockchain->nextBlock();
  std::string block0Hash("000034f32b0855161a1c2b5db42bff42ef6e7c704b600dcb4e439b83caae0196");

  ASSERT_EQ(last1->get().id(), 0U);
  ASSERT_EQ(last1->get().nonce(), 3091U);
  ASSERT_EQ(last1->get().hash(), block0Hash);
  Common::optional_ref<Miner::Block> last2 = blockchain->lastBlock();

  blockchain->appendTransaction("Marc-Olivier Arsenault");
  blockchain->appendTransaction("Stéphanie Lacerte");
  blockchain->appendTransaction("Zachary Rousseau");
  blockchain->nextBlock();
  std::string block1Hash("0008756f8bfbf6166d3b8030e7a98c0ec7c58cff02c9458c4bed6dc46611a3b6");

  ASSERT_EQ(last2->get().id(), 1U);
  ASSERT_EQ(last2->get().nonce(), 6681U);
  ASSERT_EQ(last2->get().hash(), block1Hash);

  blockchain->saveAll();
  std::string block2Hash("a615db4568289dfb5686f40618e333fd867c009a7997bc97ecafd53b01e42e13");
  std::string metadataHash("f318bef5341fd7cd0baeee4739e0f7dccaf70e8f3b0581e321c11e7c639f576b");

  ASSERT_TRUE(Tests::checkFileSHA256Hash(blockDir / "0", block0Hash));
  ASSERT_TRUE(Tests::checkFileSHA256Hash(blockDir / "1", block1Hash));
  ASSERT_TRUE(Tests::checkFileSHA256Hash(blockDir / "2", block2Hash));
  ASSERT_TRUE(Tests::checkFileSHA256Hash(blockDir / "metadata", metadataHash));
}
