#include "definitions.h"
#include "score.h"
#include "rules.h"

int main()
{
  // ADD GTEST!!!

  xml_document xmlScore;
  cstr filePath("./extern/musicxml/MozaChloSample.xml");

  xml_parse_result result = xmlScore.load_file(filePath);

  if (result)
  {
    std::cout << "XML file '" << filePath << "' successfully parsed.\n\n";
  }
  else
  {
    std::cout << "XML file '" << filePath << "' parsed with errors!\n";
    std::cout << "Error description: " << result.description() << "\n";
    return 0;
  }

  Score score(xmlScore);

  std::vector<Rule> ruleSet = 
  {
    Rules::parallelFifths,
    Rules::parallelOctaves
  };


  std::cout << "Analyzing with ruleset : \n";
  for(auto& rule : ruleSet)
    std::cout << "  - " << rule.name << "\n";

  std::vector<RuleCheckResult> errors;

  for(auto& pos : score._score)
  {
    for(auto& rule : ruleSet)
    {
      auto result = rule.func(pos.second);
      if(!result.rulePassed)
        errors.push_back(result);
    }    
  }

  for(auto error : errors)
  {
    std::cout << error.message << " at measure " << error.measure << "\n";
  }

  // return analysis in json

  return 0;
}