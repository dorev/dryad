#pragma once

#include "definitions.h"
#include "notes.h"

struct Pitch 
{
  std::string step;
  int alter;
  int octave;
  int duration;
  int num;
  int measure;
  const xml_node* nodePtr;

  Pitch() // default pitch like this might be a rest
    : step("")
    , alter(0)
    , octave(0)
    , duration(-1)
    , num(-1)
    , measure(-1)
    , nodePtr(nullptr)
  {}

  Pitch(std::string step, int alter, int octave, int duration, const xml_node* nodePtr)
    : step(step)
    , alter(alter)
    , octave(octave)
    , duration(duration)
    , num(octave * 12 + (noteNum(step) + alter))
    , measure(nodePtr ? nodePtr->parent().attribute("number").as_int() : -1)
    , nodePtr(nodePtr)
  {}

  bool fromNode(const xml_node& noteNode)
  {
    if(!(!strcmp(noteNode.name(),"note") 
    && noteNode.child("pitch") 
    && noteNode.child("duration")))
      return false;
    
    if(xml_node stepNode = noteNode.child("pitch").child("step"))
      step = stepNode.text().as_string();
    else 
      return false;
    
    if(xml_node alterNode = noteNode.child("pitch").child("alter"))
      alter = alterNode.text().as_int();
    
    if(xml_node octaveNode = noteNode.child("pitch").child("octave"))
      octave = octaveNode.text().as_int();
    else
      return false;

    duration = noteNode.child("duration").text().as_int();
    
    num = octave * 12 + (noteNum(step) + alter);


    nodePtr = &noteNode;
    measure = nodePtr->parent().attribute("number").as_int();

    return isValid();
  }

  bool isValid()
  {
    if(num > 144)
      throw "Unexpected note value";
      
    return  noteNum(step) >= 0 
            && octave >= 0
            && duration > 0
            && measure >= 0
            && num == octave * 12 + (noteNum(step) + alter)
            && nodePtr != nullptr;
  }
  

  bool operator==(const Pitch& other) const
  {
    return duration == other.duration && num == other.num;
  }

  bool operator<(const Pitch& other) const
  {
    return  num  < other.num 
        || (num == other.num && duration < other.duration);
  }

};
