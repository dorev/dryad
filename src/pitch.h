#pragma once

#include "definitions.h"

struct Pitch 
{
  std::string _step;
  int _alter;
  int _octave;
  int _duration;
  int _num;
  int _measure;
  NodePtr _nodePtr;

  Pitch() // default pitch like this might be a rest
    : _step("")
    , _alter(0)
    , _octave(0)
    , _duration(-1)
    , _num(-1)
    , _measure(-1)
    , _nodePtr(nullptr)
  {}

  Pitch(std::string step, int alter, int octave, int duration, NodePtr nodePtr)
    : _step(step)
    , _alter(alter)
    , _octave(octave)
    , _duration(duration)
    , _num(_octave * 12 + (noteNum(_step) + _alter))
    , _measure(nodePtr ? nodePtr->parent().attribute("number").as_int() : -1)
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
    
    if(xml_node octave = noteNode.child("pitch").child("octave"))
      _octave = octave.text().as_int();
    else
      return false;

    _duration = noteNode.child("duration").text().as_int();
    
    _num = _octave * 12 + (noteNum(_step) + _alter);


    _nodePtr = makeNodePtr(noteNode);
    _measure = _nodePtr->parent().attribute("number").as_int();

    return isValid();
  }

  bool isValid()
  {
    if(_num > 144)
      throw "Unexpected note value";
      
    return  ::noteNumber[_step] >= 0 
            && _octave >= 0
            && _duration > 0
            && _measure >= 0
            && _num == _octave * 12 + (noteNum(_step) + _alter)
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

using PitchPtr = std::shared_ptr<Pitch>;
