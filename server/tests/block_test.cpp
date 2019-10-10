#include <gtest/gtest.h>
#include <miner/block.hpp>

TEST(BlockTest, simple_hash) {
  std::string previous = "eed82d9682fb1a4a37ecceba76052738f687f3c5e5fd317c940471e8413d140f";
  Miner::Block block(previous);
  block.append("Cédrik Deschênes");
  block.append("Anne-Sophie Provencher");
  block.append("Ellie Marier");

  std::string received = block.getHash();
  std::string expected = "c918e346a971a1f6ab12a2e90c5e3516890a2c6388df017eff629719e27f4db1";
  ASSERT_EQ(expected, received);
}

TEST(BlockTest, mine_block) {
  std::string previous = "eed82d9682fb1a4a37ecceba76052738f687f3c5e5fd317c940471e8413d140f";
  Miner::Block block(previous);
  block.append("Chloë Berger");
  block.append("Sébastien Valcourt");
  block.append("Dave Potvin");

  block.mine(3);

  std::string received = block.getHash();
  std::string expected = "000202656242b495c07ab8261a3a0a5a1ac65ac5c971f84220b6eba67eb1b414";
  ASSERT_EQ(expected, received);
}
