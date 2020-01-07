#include "gtest/gtest.h"
#include "scorePosition.h"

class ScorePositionTest : public ::testing::Test 
{};

TEST_F(ScorePositionTest, InsertNote) 
{  
  GTEST_SKIP() << "Not implemented";
  EXPECT_TRUE(true);
}


TEST_F(ScorePositionTest, InsertResonatingNote) 
{  
  GTEST_SKIP() << "Not implemented";
  EXPECT_TRUE(true);
}


TEST_F(ScorePositionTest, FindInterval) 
{  
  std::set<Pitch> notes;
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

  ScorePosition s;
  for(auto note : notes)
    s.insert(note);
  
  EXPECT_EQ(findIntervalsInPos(0,s).size(), 0)  << "Should have found no interval of 0 semitone";
  EXPECT_EQ(findIntervalsInPos(1,s).size(), 12) << "Should have found 12 intervals of 1 semitone";
  EXPECT_EQ(findIntervalsInPos(2,s).size(), 11) << "Should have found 11 intervals of 2 semitones";
  EXPECT_EQ(findIntervalsInPos(3,s).size(), 10) << "Should have found 10 intervals of 3 semitones";
  EXPECT_EQ(findIntervalsInPos(4,s).size(), 9)  << "Should have found 9 intervals of 4 semitones";
  EXPECT_EQ(findIntervalsInPos(5,s).size(), 8)  << "Should have found 8 intervals of 5 semitones";
  EXPECT_EQ(findIntervalsInPos(6,s).size(), 7)  << "Should have found 7 intervals of 6 semitones";
  EXPECT_EQ(findIntervalsInPos(7,s).size(), 6)  << "Should have found 6 intervals of 7 semitones";
  EXPECT_EQ(findIntervalsInPos(8,s).size(), 5)  << "Should have found 5 intervals of 8 semitones";
  EXPECT_EQ(findIntervalsInPos(9,s).size(), 4)  << "Should have found 4 intervals of 9 semitones";
  EXPECT_EQ(findIntervalsInPos(10,s).size(), 3) << "Should have found 3 intervals of 10 semitones";
  EXPECT_EQ(findIntervalsInPos(11,s).size(), 2) << "Should have found 2 intervals of 11 semitones";
  EXPECT_EQ(findIntervalsInPos(12,s).size(), 1) << "Should have found 1 interval of 12 semitones";
}

TEST_F(ScorePositionTest, FindMeasure) 
{  
  GTEST_SKIP() << "Not implemented";
  EXPECT_TRUE(true);
}
