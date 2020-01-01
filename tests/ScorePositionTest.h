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
  
  EXPECT_EQ(s.findInterval(0).size(), 0)  << "Should have found no interval of 0 semitone";
  EXPECT_EQ(s.findInterval(1).size(), 12) << "Should have found 12 intervals of 1 semitone";
  EXPECT_EQ(s.findInterval(2).size(), 11) << "Should have found 11 intervals of 2 semitones";
  EXPECT_EQ(s.findInterval(3).size(), 10) << "Should have found 10 intervals of 3 semitones";
  EXPECT_EQ(s.findInterval(4).size(), 9)  << "Should have found 9 intervals of 4 semitones";
  EXPECT_EQ(s.findInterval(5).size(), 8)  << "Should have found 8 intervals of 5 semitones";
  EXPECT_EQ(s.findInterval(6).size(), 7)  << "Should have found 7 intervals of 6 semitones";
  EXPECT_EQ(s.findInterval(7).size(), 6)  << "Should have found 6 intervals of 7 semitones";
  EXPECT_EQ(s.findInterval(8).size(), 5)  << "Should have found 5 intervals of 8 semitones";
  EXPECT_EQ(s.findInterval(9).size(), 4)  << "Should have found 4 intervals of 9 semitones";
  EXPECT_EQ(s.findInterval(10).size(), 3) << "Should have found 3 intervals of 10 semitones";
  EXPECT_EQ(s.findInterval(11).size(), 2) << "Should have found 2 intervals of 11 semitones";
  EXPECT_EQ(s.findInterval(12).size(), 1) << "Should have found 1 interval of 12 semitones";
}


TEST_F(ScorePositionTest, FindMeasure) 
{  
  GTEST_SKIP() << "Not implemented";
  EXPECT_TRUE(true);
}
