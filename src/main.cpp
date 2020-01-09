#include <sstream>

#include "definitions.h"
#include "score.h"
#include "rules.h"
#include "ruleCheck.h"

std::set<Rule> buildRuleSet(std::set<std::string>& rulesList)
{
  std::set<Rule> ruleSet;

  for(auto& ruleName : rulesList)
    if(__rules.find(ruleName) != __rules.end())
      ruleSet.insert(__rules[ruleName]);

  return ruleSet;
}

std::string processScore(std::string musicXml, std::string rulesListWithSeparators)
{
  xml_document xmlScore;
  //xml_parse_result result = xmlScore.load(musicXml.c_str());
  xml_parse_result result = xmlScore.load_file(musicXml.c_str());

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

  std::set<std::string> rulesList;
  rulesSplit(rulesList, rulesListWithSeparators, " ");

  std::set<Rule> ruleSet = buildRuleSet(rulesList);

  std::stringstream s1;
  std::stringstream s2;

  for(auto error : checkRulesOnScore(ruleSet, score))
    s1 << error.message[Lang::fr] << " at measure " << error.measure << "\n";

  for(auto scale : findScalesByMeasure(score))
    s2 << "[" << scale.first << "]\t" << scale.second.toString() << "\n";
    
  s1 << "\n\nScale analysis :\n" << s2.str();

  // return analysis in json
  return s1.str();
}

int main()
{
  cstr filePath("./extern/musicxml/MozaChloSample.xml");

  // open file as string
  std::cout << processScore(filePath, "parallelFifths parallelOctaves");

  return 0;
}