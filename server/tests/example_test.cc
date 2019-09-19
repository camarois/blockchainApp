#include "common/example.h"
#include <gtest/gtest.h>

class ExampleTest : public ::testing::Test {
protected:
  void SetUp() override { example = std::make_unique<Example>(); }

  void TearDown() override { example.reset(); }

  std::unique_ptr<Example> example;
};

TEST_F(ExampleTest, sample_method_1) {
  ASSERT_EQ(example->example(), 123);
}

TEST_F(ExampleTest, sample_method_2) {
  ASSERT_NE(example->example(), 0);
}
