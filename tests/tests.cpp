#include "src/gtest-all.cc"

// Tests
#include "tests/RulesTest.h"

int main(int argc, char **argv) 
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}