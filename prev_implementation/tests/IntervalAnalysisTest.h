#include "gtest/gtest.h"
#include "chordAnalysis.h"

struct IntervalTestParam
{
  int interval;
  int expectedCount;
};

class IntervalTest : public ::testing::TestWithParam<IntervalTestParam>
{
protected :
  std::set<Pitch> notes;
  ScorePosition scorePos;
  
  IntervalTest()
  {
    notes.insert(Pitch("C",  0, 4, 1, nullptr));
    notes.insert(Pitch("C#", 0, 4, 1, nullptr));
    notes.insert(Pitch("D",  0, 4, 1, nullptr));
    notes.insert(Pitch("D#", 0, 4, 1, nullptr));
    notes.insert(Pitch("E",  0, 4, 1, nullptr));
    notes.insert(Pitch("F",  0, 4, 1, nullptr));
    notes.insert(Pitch("F#", 0, 4, 1, nullptr));
    notes.insert(Pitch("G",  0, 4, 1, nullptr));
    notes.insert(Pitch("G#", 0, 4, 1, nullptr));
    notes.insert(Pitch("A",  0, 4, 1, nullptr));
    notes.insert(Pitch("A#", 0, 4, 1, nullptr));
    notes.insert(Pitch("B",  0, 4, 1, nullptr));
    notes.insert(Pitch("C",  0, 5, 1, nullptr));

    for(auto& note : notes)
      scorePos.insert(note, 0, 0);
  }
};

TEST_P(IntervalTest, FindInterval) 
{ 
  EXPECT_EQ(
    findIntervalsInPos(GetParam().interval,scorePos).size(), 
    GetParam().expectedCount
  )  
  << "Should have found " << GetParam().expectedCount 
  << " intervals of "     << GetParam().interval << " semitones"; 
}

INSTANTIATE_TEST_SUITE_P 
(
  Analysis,
  IntervalTest,
  ::testing::Values
  (
    IntervalTestParam {0,0},
    IntervalTestParam {1,12},
    IntervalTestParam {2,11},
    IntervalTestParam {3,10},
    IntervalTestParam {4,9},
    IntervalTestParam {5,8},
    IntervalTestParam {6,7},
    IntervalTestParam {7,6},
    IntervalTestParam {8,5},
    IntervalTestParam {9,4},
    IntervalTestParam {10,3},
    IntervalTestParam {11,2},
    IntervalTestParam {12,1}
  )
);

