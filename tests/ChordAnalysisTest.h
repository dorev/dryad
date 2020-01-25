#include "gtest/gtest.h"
#include "scaleAnalysis.h"
#include "chordAnalysis.h"

struct ChordTestParam
{
  Chord chord;
  int expected;
};

class ChordTest : public ::testing::TestWithParam<ChordTestParam>
{};

TEST_P(ChordTest, FindScale) 
{
  auto result = listScalesOfChord(GetParam().chord).size();

  for(auto scale : listScalesOfChord(GetParam().chord))
    std::cout << scale.toString() << "\n";

  EXPECT_EQ(result, GetParam().expected);
}

INSTANTIATE_TEST_CASE_P 
(
  Analysis,
  ChordTest,
  ::testing::Values
  (
    ChordTestParam {{{0,4,7,11}}, 6},
    ChordTestParam {{{0,2,3,4,5,7,8,9,10,11}}, 1},
    ChordTestParam {{{0,1,2,3,4,5,6,7,8,9,10,11}}, 0}
  )
);

class ChordAnalysisTest : public ::testing::Test
{};


TEST_F(ChordAnalysisTest, MajorChord) 
{
  auto pos = ScorePosition();

  pos._notes = {
    Pitch("C"),
    Pitch("E"),
    Pitch("G")
  };

  auto result = findChordInPos(pos);

  EXPECT_EQ(result.size(),1);
  EXPECT_EQ(result.begin()->_name,"C");
  EXPECT_EQ(result.begin()->_triads,ChordTriads::Maj);
  EXPECT_EQ(result.begin()->_inversion,ChordInversion::Root);
  EXPECT_EQ(result.begin()->_variations.size(), 0);  
}

TEST_F(ChordAnalysisTest, MinorChord) 
{
  auto pos = ScorePosition();

  pos._notes = {
    Pitch("C"),
    Pitch("Eb"),
    Pitch("G")
  };

  auto result = findChordInPos(pos);

  EXPECT_EQ(result.size(),1);
  EXPECT_EQ(result.begin()->_name,"Cm");
  EXPECT_EQ(result.begin()->_triads,ChordTriads::Min);
  EXPECT_EQ(result.begin()->_inversion,ChordInversion::Root);
  EXPECT_EQ(result.begin()->_variations.size(), 0);  
}

TEST_F(ChordAnalysisTest, DiminishedChord) 
{
  auto pos = ScorePosition();

  pos._notes = {
    Pitch("C"),
    Pitch("Eb"),
    Pitch("Gb")
  };

  auto result = findChordInPos(pos);

  EXPECT_EQ(result.size(),1);
  EXPECT_EQ(result.begin()->_name,"C-");
  EXPECT_EQ(result.begin()->_triads,ChordTriads::Dim);
  EXPECT_EQ(result.begin()->_inversion,ChordInversion::Root);
  EXPECT_EQ(result.begin()->_variations.size(), 0);  
}

TEST_F(ChordAnalysisTest, AugmentedChord) 
{
  auto pos = ScorePosition();

  pos._notes = {
    Pitch("C"),
    Pitch("E"),
    Pitch("G#")
  };

  auto result = findChordInPos(pos);

  EXPECT_EQ(result.size(),1);
  EXPECT_EQ(result.begin()->_name,"C+");
  EXPECT_EQ(result.begin()->_triads,ChordTriads::Aug);
  EXPECT_EQ(result.begin()->_inversion,ChordInversion::Root);
  EXPECT_EQ(result.begin()->_variations.size(), 0);  
}

