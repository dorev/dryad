#include <functional>
#include <iostream>
#include <set>
#include <string>
#include <map>
#include <memory>
#include "algorithm"

// Extern
#include "pugixml.hpp"

using namespace pugi;
using NodePtr = std::shared_ptr<xml_node>;
using cstr = const char *;


struct RuleCheckResult {};

namespace 
{
  std::map<std::string, int> noteNumber(
  {
    {"C", 0},
    {"C#", 1},
    {"Db", 1},
    {"D", 2},
    {"D#", 3},
    {"Eb", 3},
    {"E", 4},
    {"F", 5},
    {"F#", 6},
    {"Gb", 6},
    {"G", 7},
    {"G#", 8},
    {"Ab", 8},
    {"A", 9},
    {"A#", 10},
    {"Bb", 10},
    {"B", 11}
  });
}

struct Pitch 
{
  std::string _step;
  int _alter;
  int _octave;
  int _duration;
  int _num;
  NodePtr _nodePtr;

  Pitch() 
    : _step("C")
    , _alter(0)
    , _octave(0)
    , _duration(0)
    , _num(_octave * (::noteNumber[_step] + _alter))
    , _nodePtr(nullptr)
  {}

  Pitch(std::string step, int alter, int octave, int duration, NodePtr nodePtr)
    : _step(step)
    , _alter(alter)
    , _octave(octave)
    , _duration(duration)
    , _num(_octave * (::noteNumber[_step] + _alter))
    , _nodePtr(nodePtr)
  {}

  bool operator==(const Pitch& other) const
  {
    return _duration == other._duration && _num == other._num;
  }

  bool operator<(const Pitch& other) const
  {
    return _num < other._num || _duration < other._duration;
  }

};



struct ScorePosition 
{  
  std::set<Pitch> _notes;
  std::set<Pitch> _resonatingNotes;

  bool insert(Pitch& pitch)
  {    
    return _notes.insert(pitch).second;
  }

};


using ScorePositionPtr = std::shared_ptr<ScorePosition>;
using Rule = std::function<RuleCheckResult(ScorePosition)>;

struct Score
{
  std::map<int, ScorePosition> _score;

  ScorePositionPtr operator[](int index)
  {
    return (_score.find(index) == _score.end())
      ? nullptr
      : std::make_shared<ScorePosition>(_score[index]);
  }

  bool insert(Pitch& pitch, int pos)
  {
    return _score[pos].insert(pitch);
  }

  bool finalize()
  {
    // normalize divisions
    // add resonating notes
    // combine tied notes

    return true;
  }

};



int main()
{
  // ADD GTEST!!!

  xml_document xmlScore;
  xml_node xmlElement;
  cstr filePath("./extern/musicxml/Chant.xml");

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
    return 0;
  }

  //xmlScore.save(std::cout);
  
  xpath_node_set nodes = xmlScore.select_nodes("//part");

  for (auto itr : nodes)
  {
    std::cout << itr.node().name() << "\n";
    for (auto child : itr.node().children())
    {
      std::cout << " " << child.name() << "\n";
    }
  }

  return 0;
}

/*

Challenges
----------
manage mxl compressed format
combine voices (forward and backup)
combine parts (variable quarter-note divisions)

Ideas
-----
map of positions (absolute values) filled with notes (pitch + duration)
convert tied notes to single note instance with combined duration
(forbid slides on frontend)

Scan for backup nodes
Scan for division nodes
Parse by parts
Keep node ptr for indicative styling

// scan everything for duration changes and normalize values across tree

for(auto part : parts)
  for(auto measure : part.measures)
    for(auto child : measure.children)
    {
      if(back || forward)
        movement += node.move
        continue;
      
      analyze child node

      score.insert(node, position)      
    }


*/