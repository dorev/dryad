#include <functional>
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <memory>

#include "pugixml.hpp"

using namespace pugi;
using NodePtr = std::shared_ptr<xml_node>;
using cstr = const char *;


struct RuleCheckResult {};


struct Pitch 
{
  std::string step;
  int alter;
  int octave;
  int duration;
};



struct ScorePosition 
{
  std::vector<Pitch> notes;
  NodePtr node;
};


using ScorePositionPtr = std::shared_ptr<ScorePosition>;
using Rule = std::function<RuleCheckResult(ScorePosition)>;

struct Score
{
  std::map<size_t, ScorePosition> _score;

  ScorePositionPtr operator[](size_t index)
  {
    return (_score.find(index) == _score.end())
      ? nullptr
      : std::make_shared<ScorePosition>(_score[index]);
  }

};



int main()
{

  xml_document xmlScore;
  xml_node xmlElement;
  cstr filePath("..\\extern\\musicxml\\Chant.xml");

  xml_parse_result result = xmlScore.load_file(filePath);

  if (result)
  {
    std::cout << "XML file '" << filePath << "' parsed without errors.\n\n";
  }
  else
  {
    std::cout << "XML file '" << filePath << "' parsed with errors!\n";
    std::cout << "Error description: " << result.description() << "\n";
    std::cout << "Error offset: " << result.offset << "\n\n";
  }

  xmlScore.save(std::cout);
  
  xpath_node_set nodes = xmlScore.select_nodes("//part");

  for (auto itr : nodes)
  {
    std::cout << itr.node().name() << "\n";
    for (auto child : itr.node().children())
    {
      std::cout << " " << child.name() << "\n";
    }
  }

  getchar();
  return 0;
}