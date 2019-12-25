#include <functional>
#include <iostream>
#include <set>
#include <string>
#include <cstring>
#include <map>
#include <memory>
#include <algorithm>
#include <cassert>

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



void uniformizeDivisions(xml_document& xml)
{
  std::map<size_t, int> divisions = {};
  std::map<size_t, int> divisionFactors = {};

  // Find all division nodes
  for(auto& division : xml.select_nodes("//divisions"))
    divisions[division.node().offset_debug()] = std::stoi(division.node().child_value());

  int maxDivision = std::max_element(divisions.begin(), divisions.end())->second;
  
  // Set all factors to normalize quarter-note durations
  for(auto& division : divisions)
    divisionFactors[division.first] = maxDivision / division.second;

  // Set all division nodes to the same value
  for(auto& division : xml.select_nodes("//divisions"))
    division.node().text().set(std::to_string(maxDivision).c_str());

  // Adjust all durations of the document
  for(auto& duration : xml.select_nodes("//duration"))
  {
    size_t offset = duration.node().offset_debug();
    int value = std::stoi(duration.node().child_value());

    // Find modifier to apply
    auto revItr = std::find_if
    (
      divisionFactors.rbegin(), 
      divisionFactors.rend(), 
      [&](auto& kv) { return kv.first < offset; }
    );

    // Iterator should never reach rend because a 'duration' node
    // should never be before the first 'divisions' node
    assert(revItr != divisionFactors.rend());

    // Update duration value
    duration.node().text().set(std::to_string(value * (*revItr).second).c_str());
  }
}


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

  uniformizeDivisions(xmlScore);

  for (auto& part : xmlScore.select_nodes("//part"))
  {
    for (auto& measure : part.node().children())
    {
      for(auto& node : measure.children())
      {
        if
        (
          (!strcmp(node.name(),"backup") || !strcmp(node.name(),"forward"))
          && node.child("duration")
        )
          true; //std::cout << "    " << node.name() << " : " << node.child("duration").child_value() << "\n";
        
      } // end of for nodes
    } // end of for measures
  } // end of for parts

  return 0;
}

/*

Scan for backup nodes
Scan for division nodes
Parse by parts
Keep node ptr for indicative styling

*/