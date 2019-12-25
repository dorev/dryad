#pragma once

#include "definitions.h"

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

  Pitch() // default pitch like this might be a rest
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

    _nodePtr = makeNodePtr(noteNode);

    return isValid();
  }

  bool isValid()
  {
    return  ::noteNumber[_step] >= 0 
            && _octave >= 0
            && _duration > 0
            && _num == _octave * (::noteNumber[_step] + _alter)
            && _nodePtr != nullptr;
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
