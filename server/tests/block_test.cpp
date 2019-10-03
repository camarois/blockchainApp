#include <gtest/gtest.h>
#include <miner/block.hpp>

TEST(BlockTest, simple_hash) {
  std::string previous = "eed82d9682fb1a4a37ecceba76052738f687f3c5e5fd317c940471e8413d140f";
  std::vector<std::string> data;
  data.push_back("Cédrik Deschênes");
  data.push_back("Anne-Sophie Provencher");
  data.push_back("Ellie Marier");
  Block block(previous, 15, data);

  std::string received = block.get_hash();
  std::string expected = "7969e3efd42eeb6249f211f131667b6996d644db63c9ccea49c9b8812e524cd2";
  ASSERT_EQ(expected, received);
}

TEST(BlockTest, mine_block) {
  std::string previous = "eed82d9682fb1a4a37ecceba76052738f687f3c5e5fd317c940471e8413d140f";
  std::vector<std::string> data;
  data.push_back("Chloë Berger");
  data.push_back("Sébastien Valcourt");
  data.push_back("Dave Potvin");
  Block block(previous, 15, data);

  block.Mine(3);

  std::string received = block.get_hash();
  std::string expected = "000202656242b495c07ab8261a3a0a5a1ac65ac5c971f84220b6eba67eb1b414";
  ASSERT_EQ(expected, received);
}
