#include "gtest/gtest.h"
#include "tests/MXLTestUtils.h"

#include "rules.h"

class RulesTest : public ::testing::Test 
{};

TEST_F(RulesTest, parallelFifth) 
{
  std::string parallelFifthTest 
  = mxlBeg 
  + genNote("C",4,1) 
  + genNote("D",4,1)
  + genNote("D",4,1) 
  + genNote("C",4,1) 
  + genBackup(4)
  + genNote("G",4,1) 
  + genNote("A",4,1)
  + genNote("A",4,1)
  + genNote("G",4,1) 
  + mxlEnd;

  xml_document xmlScore;
  if(!xmlScore.load_string(parallelFifthTest.c_str()))
    FAIL() << "Unable to parse test string";

  Score score(xmlScore);
  
  std::vector<RuleCheckResult> errors;

  for(auto& pos : score.score)
  {
    auto result = __rules["parallelFifths"].func(pos.second);
    if(!result.rulePassed)
      errors.push_back(result);  
  }

  EXPECT_EQ(errors.size(),3);
}
