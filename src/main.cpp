#include <sstream>

#include "definitions.h"
#include "score.h"
#include "rules.h"
#include "ruleCheck.h"

#ifdef __EMSCRIPTEN__
  #include <emscripten.h>
#endif

std::string processScore(std::string musicXml, std::string rulesListWithSeparators)
{
  //
  // Setup score
  //

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

  //
  // Setup rules to check
  //

  std::set<std::string> rulesList;
  rulesSplit(rulesList, rulesListWithSeparators, " ");

  std::set<Rule> ruleSet = buildRuleSet(rulesList);

  std::stringstream s1;
  std::stringstream s2;

  //
  // Run analysis
  //

  for(auto error : checkRulesOnScore(ruleSet, score))
    s1 << error.message[Lang::fr] << " at measure " << error.measure << "\n";

  // THIS MADE NO SENSE SINCE SCALES HAD TO BE FOUND BEFORE CHECKING RULES
  for(auto scale : findScalesInRange(score, 0, 400))
    s2 << "[" << scale.first << "]\t" << scale.second.toString() << "\n";
    
  s1 << "\n\nScale analysis :\n" << s2.str();

  return s1.str();
}

//
// Entry point for testing purposes
//

int main()
{
  cstr filePath("./extern/musicxml/MozaChloSample.xml");

  // open file as string
  std::cout << processScore(filePath, "parallelFifths parallelOctaves");

  return 0;
}