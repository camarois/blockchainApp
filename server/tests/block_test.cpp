#include <gtest/gtest.h>
#include <miner/block.hpp>

TEST(BlockTest, simple_hash) {
  std::string previous = "eed82d9682fb1a4a37ecceba76052738f687f3c5e5fd317c940471e8413d140f";
  Miner::Block block(420, previous);
  block.append("Cédrik Deschênes");
  block.append("Anne-Sophie Provencher");
  block.append("Ellie Marier");

  std::string received = block.getHash();
  std::string expected = "3dcd913ba603db2a559226e28289c8931849fea5bdf8cf18ec0acca2a10fdfff";
  ASSERT_EQ(expected, received);
}

TEST(BlockTest, mine_block) {
  std::string previous = "eed82d9682fb1a4a37ecceba76052738f687f3c5e5fd317c940471e8413d140f";
  Miner::Block block(420, previous);
  block.append("Chloë Berger");
  block.append("Sébastien Valcourt");
  block.append("Dave Potvin");

  block.mine(3);

  std::string received = block.getHash();
  std::string expected = "000d33faeb3abcd6cb9900613d17869a6c91ca2bf49cf5ed2621cc2b78963a31";
  ASSERT_EQ(expected, received);
}
