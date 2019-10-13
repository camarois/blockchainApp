#include <cstdlib>
#include <filesystem>
#include <gtest/gtest.h>
#include <miner/block.hpp>

#include "utils.hpp"

TEST(BlockTest, simple_hash) {
  std::string previous = "eed82d9682fb1a4a37ecceba76052738f687f3c5e5fd317c940471e8413d140f";

  Miner::Block block(420, previous);
  block.append("Cédrik Deschênes");
  block.append("Anne-Sophie Provencher");
  block.append("Ellie Marier");

  std::string received = block.hash();
  std::string expected = "e65e6f90beba9a03c10270da7aae49e78c115d3116772d0c1abee6616f75cf34";
  ASSERT_EQ(expected, received);
}

TEST(BlockTest, mine_block) {
  std::string previous = "eed82d9682fb1a4a37ecceba76052738f687f3c5e5fd317c940471e8413d140f";

  Miner::Block block(420, previous);
  block.append("Chloë Berger");
  block.append("Sébastien Valcourt");
  block.append("Dave Potvin");
  block.mine(3);

  std::string received = block.hash();
  std::string expected = "000685913a46eff7c6be31415fa81935e499029b342b28ee48980a518dfa8429";
  ASSERT_EQ(expected, received);
}

TEST(BlockTest, save_load_block) {
  std::filesystem::path path = Tests::createEmptyDir("block/test-save-load-block");
  std::string previous = "eed82d9682fb1a4a37ecceba76052738f687f3c5e5fd317c940471e8413d140f";

  Miner::Block original(420, previous);
  original.append("Cédrik Deschênes");
  original.append("Anne-Sophie Provencher");
  original.append("Ellie Marier");
  original.save(path);

  path.append(std::to_string(original.id()));
  ASSERT_TRUE(std::filesystem::exists(path));

  Miner::Block loaded(path);
  std::string received = loaded.hash();
  std::string expected = "e65e6f90beba9a03c10270da7aae49e78c115d3116772d0c1abee6616f75cf34";
  ASSERT_EQ(expected, received);
}
