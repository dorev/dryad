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

INSTANTIATE_TEST_SUITE_P 
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
  EXPECT_EQ(result.begin()->_triads,ChordTriad::Major);
  EXPECT_EQ(result.begin()->_inversion,ChordInversion::Root);
  EXPECT_EQ(result.begin()->_extension, ChordExtension::None);   
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
  EXPECT_EQ(result.begin()->_triads,ChordTriad::Minor);
  EXPECT_EQ(result.begin()->_inversion,ChordInversion::Root);
  EXPECT_EQ(result.begin()->_extension, ChordExtension::None);  
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
  EXPECT_EQ(result.begin()->_triads,ChordTriad::Dim);
  EXPECT_EQ(result.begin()->_inversion,ChordInversion::Root);
  EXPECT_EQ(result.begin()->_extension, ChordExtension::None);  
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

  EXPECT_EQ(result.size(),3);
  EXPECT_EQ(result.begin()->_name,"C+");
  EXPECT_EQ(result.begin()->_triads,ChordTriad::Aug);
  EXPECT_EQ(result.begin()->_inversion,ChordInversion::Root);
  EXPECT_EQ(result.begin()->_extension, ChordExtension::None);  
}

TEST_F(ChordAnalysisTest, SevenChord) 
{
  auto pos = ScorePosition();

  pos._notes = {
    Pitch("C"),
    Pitch("E"),
    Pitch("G"),
    Pitch("Bb")
  };

  auto result = findChordInPos(pos);

  EXPECT_EQ(result.size(),1);
  EXPECT_EQ(result.begin()->_name,"C7");
  EXPECT_EQ(result.begin()->_triads,ChordTriad::Major);
  EXPECT_EQ(result.begin()->_inversion,ChordInversion::Root);
  EXPECT_EQ(result.begin()->_extension, ChordExtension::Seven);  
}

TEST_F(ChordAnalysisTest, MinorSevenChord) 
{
  auto pos = ScorePosition();

  pos._notes = {
    Pitch("C"),
    Pitch("Eb"),
    Pitch("G"),
    Pitch("Bb")
  };

  auto result = findChordInPos(pos);

  EXPECT_EQ(result.size(),2);
  EXPECT_EQ(result.begin()->_name,"Cm7");
  EXPECT_EQ(result.begin()->_triads,ChordTriad::Minor);
  EXPECT_EQ(result.begin()->_inversion,ChordInversion::Root);
  EXPECT_EQ(result.begin()->_extension, ChordExtension::Seven);  
}

TEST_F(ChordAnalysisTest, MajorSevenChord) 
{
  auto pos = ScorePosition();

  pos._notes = {
    Pitch("C"),
    Pitch("E"),
    Pitch("G"),
    Pitch("B")
  };

  auto result = findChordInPos(pos);

  EXPECT_EQ(result.size(),1);
  EXPECT_EQ(result.begin()->_name,"CM7");
  EXPECT_EQ(result.begin()->_triads,ChordTriad::Major);
  EXPECT_EQ(result.begin()->_inversion,ChordInversion::Root);
  EXPECT_EQ(result.begin()->_extension, ChordExtension::MajorSeven);  
}

TEST_F(ChordAnalysisTest, MinorMajorSevenChord) 
{
  auto pos = ScorePosition();

  pos._notes = {
    Pitch("C"),
    Pitch("Eb"),
    Pitch("G"),
    Pitch("B")
  };

  auto result = findChordInPos(pos);

  EXPECT_EQ(result.size(),1);
  EXPECT_EQ(result.begin()->_name,"CmM7");
  EXPECT_EQ(result.begin()->_triads,ChordTriad::Minor);
  EXPECT_EQ(result.begin()->_inversion,ChordInversion::Root);
  EXPECT_EQ(result.begin()->_extension, ChordExtension::MajorSeven);  
}

TEST_F(ChordAnalysisTest, Sus2Chord) 
{
  auto pos = ScorePosition();

  pos._notes = {
    Pitch("C"),
    Pitch("D"),
    Pitch("G")
  };

  auto result = findChordInPos(pos);

  EXPECT_EQ(result.size(),2);
  EXPECT_EQ(result.begin()->_name,"Csus2");
  EXPECT_EQ(result.begin()->_triads,ChordTriad::Sus2);
  EXPECT_EQ(result.begin()->_inversion,ChordInversion::Root);
  EXPECT_EQ(result.begin()->_extension, ChordExtension::None);  
}

TEST_F(ChordAnalysisTest, Sus4Chord) 
{
  auto pos = ScorePosition();

  pos._notes = {
    Pitch("C"),
    Pitch("F"),
    Pitch("G")
  };

  auto result = findChordInPos(pos);

  EXPECT_EQ(result.size(),2);
  EXPECT_EQ(result.begin()->_name,"Csus4");
  EXPECT_EQ(result.begin()->_triads,ChordTriad::Sus4);
  EXPECT_EQ(result.begin()->_inversion,ChordInversion::Root);
  EXPECT_EQ(result.begin()->_extension, ChordExtension::None);  
}

TEST_F(ChordAnalysisTest, Major6Chord) 
{
  auto pos = ScorePosition();

  pos._notes = {
    Pitch("C"),
    Pitch("E"),
    Pitch("G"),
    Pitch("A")
  };

  auto result = findChordInPos(pos);

  // also expecting Am7

  EXPECT_EQ(result.size(),2);
  EXPECT_EQ((++result.begin())->_name,"C6");
  EXPECT_EQ((++result.begin())->_triads,ChordTriad::Major);
  EXPECT_EQ((++result.begin())->_inversion,ChordInversion::Root);
  EXPECT_EQ((++result.begin())->_extension, ChordExtension::Six);  
}

TEST_F(ChordAnalysisTest, Minor6Chord) 
{
  auto pos = ScorePosition();

  pos._notes = {
    Pitch("C"),
    Pitch("Eb"),
    Pitch("G"),
    Pitch("A")
  };

  auto result = findChordInPos(pos);

  EXPECT_EQ(result.size(),2);
  EXPECT_EQ((++result.begin())->_name,"Cm6");
  EXPECT_EQ((++result.begin())->_triads,ChordTriad::Minor);
  EXPECT_EQ((++result.begin())->_inversion,ChordInversion::Root);
  EXPECT_EQ((++result.begin())->_extension, ChordExtension::Six);  
}


