#include "common/example.hpp"
#include <gtest/gtest.h>

class ExampleTest : public ::testing::Test {
 protected:
  void SetUp() override { example_ = std::make_unique<Example>(); }

  void TearDown() override { example_.reset(); }

  std::unique_ptr<Example> example_;
};

TEST_F(ExampleTest, sample_method_1) { ASSERT_EQ(example_->example(), 123); }

TEST_F(ExampleTest, sample_method_2) { ASSERT_NE(example_->example(), 0); }
