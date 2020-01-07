#include "definitions.h"
#include "score.h"
#include "rules.h"
#include "ruleCheck.h"

#include <sstream>

std::set<Rule> buildRuleSet(std::string rulesList)
{
  std::set<Rule> ruleSet = 
  {
    Rules::parallelFifths,
    Rules::parallelOctaves
  };

  return ruleSet;
}

std::string processScore(std::string musicXml, std::string rulesList)
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
  std::set<Rule> ruleSet = buildRuleSet(rulesList);

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
  std::cout << processScore(filePath, "rules");

  return 0;
}