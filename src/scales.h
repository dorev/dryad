#pragma once

#include "definitions.h"

const std::vector<int> majorIntervals          ({0,2,4,5,7,9,11});
const std::vector<int> melodicMinorIntervals   ({0,2,3,5,7,9,11});
const std::vector<int> harmonicMinorIntervals  ({0,2,3,5,7,8,11});
const std::vector<int> naturalMinorIntervals   ({0,2,3,5,7,8,10});
const std::vector<int> mixedIntervals          ({0,2,3,4,5,7,8,9,10,11});

const std::vector<std::string> circleOfFifths({"C", "G", "D", "A", "E", "B", "Gb", "Db", "Ab", "Eb", "Bb", "F"});

enum class ScaleType
{
  Major,
  MelodicMinor,
  NaturalMinor,
  HarmonicMinor,
  Mixed
};

std::map<ScaleType, const std::vector<int>> __scaleList
({
  { ScaleType::Major,         majorIntervals},
  { ScaleType::MelodicMinor,  melodicMinorIntervals},
  { ScaleType::NaturalMinor,  naturalMinorIntervals},
  { ScaleType::HarmonicMinor, harmonicMinorIntervals},
  { ScaleType::Mixed,         mixedIntervals}
});

std::vector<int> notesOfScale(int rootNum, ScaleType type)
{
  std::vector<int> result;
  for(auto interval : __scaleList[type])
    result.push_back((rootNum + interval) % 12);
  return result;
}

std::vector<int> notesOfScale(std::string rootName, ScaleType type)
{
  return notesOfScale(noteNum(rootName), type);
}

struct Scale
{
  std::string _rootName;
  int _rootNum;
  ScaleType _type;
  std::vector<int> _notes;

  Scale(int rootNum, ScaleType type = ScaleType::Major)
    : _rootName(noteName(rootNum))
    , _type(type)
    , _rootNum(rootNum)
    , _notes(notesOfScale(rootNum, type))
  {}

  Scale(const std::string rootName, ScaleType type = ScaleType::Major)
    : _rootName(rootName)
    , _type(type)
    , _rootNum(noteNum(rootName))
    , _notes(notesOfScale(rootName, type))
  {}

  std::string toString()
  {
    std::string output = _rootName;

    switch(_type)
    {
      case ScaleType::Major :           return output + " major";
      case ScaleType::MelodicMinor :    return output + " minor melodic";
      case ScaleType::NaturalMinor :    return output + " minor natural";
      case ScaleType::HarmonicMinor :   return output + " minor harmonic";
      case ScaleType::Mixed :           return output + " mixed";
      default : return output;
    }    
  }

  bool operator==(const Scale& other) const
  {
    return _notes == other._notes 
        && _type == other._type;
  }

  bool operator<(const Scale& other) const
  {
    return _rootNum < other._rootNum;    
  }
};

Scale relativeMinor(Scale& scale)
{
  if(scale._type != ScaleType::Major)
    return scale;
  
  return Scale((scale._rootNum + 9 % 12), ScaleType::NaturalMinor);
}