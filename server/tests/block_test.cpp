#include <cstdlib>
#include <filesystem>
#include <gtest/gtest.h>
#include <gflags/gflags.h>
#include <miner/block.hpp>
#include <common/database.hpp>

#include "utils.hpp"

DEFINE_string(blockchain, "blockchain/", "Path to blockchain folder");

TEST(BlockTest, simple_hash) {
  std::string previous = "eed82d9682fb1a4a37ecceba76052738f687f3c5e5fd317c940471e8413d140f";

  Miner::Block block(420, previous);
  block.setData("Cédrik Deschênes");

  std::string received = block.hash();
  std::string expected = "658fd6f585e17ac41d895f11ca9231f576c5ae84db33346c1566863e8c766702";
  ASSERT_EQ(expected, received);
}

TEST(BlockTest, mine_block) {
  std::string previous = "eed82d9682fb1a4a37ecceba76052738f687f3c5e5fd317c940471e8413d140f";

  Miner::Block block(420, previous);
  block.setData("Chloë Berger");
  block.mine(3);

  std::string received = block.hash();
  std::string expected = "0001ec9a920798351b50f91c1adec8fcbdc1731a61a1c220fc8b738f4f0330b9";
  ASSERT_EQ(expected, received);
}

TEST(BlockTest, save_load_block) {
  std::filesystem::path path = Tests::createEmptyDir("block/test-save-load-block");
  std::string previous = "eed82d9682fb1a4a37ecceba76052738f687f3c5e5fd317c940471e8413d140f";
  FLAGS_blockchain = path;

  Miner::Block original(420, previous);
  original.setData("Ellie Marier");
  original.save();

  path.append(std::to_string(original.id()));
  ASSERT_TRUE(std::filesystem::exists(path));

  std::optional<Miner::Block> loaded = Miner::Block::fromBlockFile(path);
  std::string received = loaded->hash();
  std::string expected = "f4e8eced0291ad153837d361c09786058947bf1d19db17878fbbcaa2a2d4d495";
  ASSERT_TRUE(loaded.has_value());
  ASSERT_EQ(expected, received);
}

TEST(BlockTest, load_missing_block) {
  Common::Database::init("test-blockchain.db");

  std::filesystem::path flagPath = Tests::createEmptyDir("block/test-load-missing-block-flag");
  std::filesystem::path blockPath = Tests::createEmptyDir("block/test-load-missing-block") / "420";
  FLAGS_blockchain = flagPath;

  ASSERT_TRUE(!Miner::Block::fromBlockFile(blockPath));
}

TEST(BlockTest, load_invalid_block) {
  Common::Database::init("test-blockchain.db");

  std::filesystem::path flagPath = Tests::createEmptyDir("block/test-load-invalid-block-flag");
  std::filesystem::path blockDir = Tests::getDir("block/test-load-invalid-block");
  FLAGS_blockchain = flagPath;

  ASSERT_TRUE(!Miner::Block::fromBlockFile(blockDir / "420"));
}

