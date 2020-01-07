#include "definitions.h"
#include "score.h"
#include "rules.h"
#include "ruleChecker.h"

#include <sstream>

std::vector<Rule> buildRuleSet(std::string rulesList)
{
  std::vector<Rule> ruleSet = 
  {
    Rules::parallelFifths,
    Rules::parallelOctaves
  };

  return ruleSet;
}

std::string crunch(std::string musicXml, std::string rulesList)
{
  xml_document xmlScore;
  xml_parse_result result = xmlScore.load(musicXml.c_str());

  if (result)
  {
    std::cout << "XML document '" << musicXml << "' successfully parsed.\n\n";
  }
  else
  {
    std::cout << "XML document '" << musicXml << "' parsed with errors!\n";
    std::cout << "Error description: " << result.description() << "\n";
    return 0;
  }

  Score score(xmlScore);

  // ruleset will be provided from an input json string 
  std::vector<Rule> ruleSet = buildRuleSet(rulesList);

  std::stringstream ss;

  for(auto error : checkRulesOnScore(ruleSet, score))
    ss << error.message << " at measure " << error.measure << "\n";

  // return analysis in json
  return ss.str();
}

int main()
{
  cstr filePath("./extern/musicxml/MozaChloSample.xml");

  // open file as string
  std::cout << crunch(filePath, "rules");

  return 0;
}