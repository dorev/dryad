#pragma once

#include "definitions.h"
#include "notes.h"

struct Pitch 
{
  std::string _step;
  int _alter;
  int _octave;
  int _duration;
  int _num;
  int _measure;
  const xml_node* _nodePtr;

  Pitch() // default pitch like this might be a rest
    : _step("")
    , _alter(0)
    , _octave(0)
    , _duration(-1)
    , _num(-1)
    , _measure(-1)
    , _nodePtr(nullptr)
  {}

  Pitch(std::string step, int alter = 0, int octave = 4, int duration = 0, const xml_node* nodePtr = nullptr)
    : _step(step)
    , _alter(alter)
    , _octave(octave)
    , _duration(duration)
    , _num(octave * 12 + (noteNum(step) + alter))
    , _measure(nodePtr ? nodePtr->parent().attribute("number").as_int() : -1)
    , _nodePtr(nodePtr)
  {}

  bool fromNode(const xml_node& noteNode)
  {
    if(!(!strcmp(noteNode.name(),"note") 
    && noteNode.child("pitch") 
    && noteNode.child("duration")))
      return false;
    
    if(xml_node stepNode = noteNode.child("pitch").child("step"))
      _step = stepNode.text().as_string();
    else 
      return false;
    
    if(xml_node alterNode = noteNode.child("pitch").child("alter"))
      _alter = alterNode.text().as_int();
    
    if(xml_node octaveNode = noteNode.child("pitch").child("octave"))
      _octave = octaveNode.text().as_int();
    else
      return false;

    _duration = noteNode.child("duration").text().as_int();
    
    _num = _octave * 12 + (noteNum(_step) + _alter);


    _nodePtr = &noteNode;
    _measure = _nodePtr->parent().attribute("number").as_int();

    return isValid();
  }

  bool isValid()
  {
    if(_num > 144)
      throw "Unexpected note value";
      
    return  noteNum(_step) >= 0 
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
