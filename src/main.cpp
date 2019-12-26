#include "definitions.h"
#include "score.h"

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
  std::cout << score.serialize() << "\n\n";

  // run rules on score
  // return analysis in json

  return 0;
}