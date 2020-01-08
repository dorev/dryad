#include "gtest/gtest.h"
#include "analysis.h"

struct ChordTestParam
{
  Chord chord;
  int expectedCount;
};

class ChordTest : public ::testing::TestWithParam<ChordTestParam>
{};

TEST_P(ChordTest, FindScale) 
{
  auto result = findScalesOfChord(GetParam().chord).size();

  EXPECT_EQ(result, GetParam().expectedCount);
}

INSTANTIATE_TEST_CASE_P 
(
  Analysis,
  ChordTest,
  ::testing::Values
  (
    ChordTestParam {{{0,4,7,11}},11},
    ChordTestParam {{{0,2,3,4,5,7,8,9,10,11}},1},
    ChordTestParam {{{0,1,2,3,4,5,6,7,8,9,10,11}},0}
  )
);

