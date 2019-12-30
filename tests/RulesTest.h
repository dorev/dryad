#include "gtest/gtest.h"
#include "tests/TestValues.h"

#include "rules.h"

class RulesTest : public ::testing::Test 
{};

TEST_F(RulesTest, parallelFifth) 
{
  //std::cout << parallelFifthTest << std::endl;
  
  xml_document xmlScore;
  if(!xmlScore.load_string(parallelFifthTest.c_str()))
    FAIL() << "Unable to parse test string";

  //xmlScore.save(std::cout);

  Score score(xmlScore);
  
  
  std::vector<RuleCheckResult> errors;

  for(auto& pos : score._score)
  {
    auto result = Rules::parallelFifths.func(pos.second);
    if(!result.rulePassed)
      errors.push_back(result);  
  }

  EXPECT_TRUE(errors.size() > 0);
}
