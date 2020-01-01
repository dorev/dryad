#include "definitions.h"
#include "score.h"
#include "rules.h"
#include "ruleChecker.h"

int main()
{
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

  for(auto error : checkRulesOnScore(ruleSet, score))
    std::cout << error.message << " at measure " << error.measure << "\n";

  // return analysis in json

  return 0;
}