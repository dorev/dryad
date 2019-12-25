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
    {"", -1},
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
    : _step("")
    , _alter(0)
    , _octave(0)
    , _duration(-1)
    , _num(-1)
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

  bool fromNode(xml_node& noteNode)
  {
    if(!(!strcmp(noteNode.name(),"note") 
    && noteNode.child("pitch") 
    && noteNode.child("duration")))
      return false;
    
    if(xml_node step = noteNode.child("pitch").child("step"))
      _step = step.text().as_string();
    else 
      return false;
    
    if(xml_node alter = noteNode.child("pitch").child("alter"))
      _alter = alter.text().as_int();
    else 
      return false;
    
    if(xml_node octave = noteNode.child("pitch").child("octave"))
      _octave = octave.text().as_int();
    else
      return false;

    _duration = noteNode.child("duration").text().as_int();
    
    _num = _octave * (::noteNumber[_step] + _alter);

    _nodePtr = std::make_shared<xml_node>(noteNode);

    return true;
  }

  

  bool operator==(const Pitch& other) const
  {
    return _duration == other._duration && _num == other._num;
  }

  bool operator<(const Pitch& other) const
  {
    return  _num  < other._num 
        || (_num == other._num && _duration < other._duration);
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

  Score(xml_document& xmlScore)
  {
    uniformizeDivisions(xmlScore);
    fillScore(xmlScore);


  }

  

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

  bool updateResonatingNotes()
  {
    // add resonating notes
    return true;
  }

  void fillScore(xml_document& xmlScore)
  {
    for (auto& part : xmlScore.select_nodes("//part"))
    {
      int pos = 0;

      for (auto& measure : part.node().children())
      {
        int shift = 0;

        for(auto& node : measure.children())
        {
          // Check for position shift
          if(!strcmp(node.name(),"backup"))
          {
            shift -= std::stoi(node.child_value());
            continue;
          }
          if(!strcmp(node.name(),"forward"))
          {
            shift += std::stoi(node.child_value());
            continue;
          }
          
          if(shift)
          {
            pos += shift;
            shift = 0;
          }

          if(strcmp(node.name(),"note"))
            continue;
          
          


          if(auto pitch = node.child("pitch"))
          {
            // add Pitch ctor from node
          }




          // pitch
          // rest

          // duration
          // chord



        } // end of for nodes
      } // end of for measures
    } // end of for parts

  }

  void uniformizeDivisions(xml_document& xml)
  {
    std::map<size_t, int> divisions = {};
    std::map<size_t, int> divisionFactors = {};

    // Find all division nodes
    for(auto& division : xml.select_nodes("//divisions"))
      divisions[division.node().offset_debug()] = division.node().text().as_int();

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
      int value = duration.node().text().as_int();

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

};

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

  return 0;
}