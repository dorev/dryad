#include "definitions.h"
#include "score.h"
#include "rules.h"
#include "ruleChecker.h"

std::string crunch(std::string musicXml)
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

  std::vector<Rule> ruleSet = 
  {
    Rules::parallelFifths,
    Rules::parallelOctaves
  };

  for(auto error : checkRulesOnScore(ruleSet, score))
    std::cout << error.message << " at measure " << error.measure << "\n";

  // return analysis in json

}

int main()
{
  cstr filePath("./extern/musicxml/MozaChloSample.xml");

  // open file as string
  std::cout << crunch(filePath);

  return 0;
}