#include "src/gtest-all.cc"

// Tests
#include "tests/RulesTest.h"
#include "tests/IntervalAnalysisTest.h"
#include "tests/ChordAnalysisTest.h"
#include "tests/ScaleAnalysisTest.h"
#include "tests/NotesAnalysisTest.h"

int main(int argc, char **argv) 
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}