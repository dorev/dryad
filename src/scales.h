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

std::map<ScaleType, const std::vector<int>&> scaleTypes
({
  { ScaleType::Major,         majorIntervals},
  { ScaleType::MelodicMinor,  melodicMinorIntervals},
  { ScaleType::NaturalMinor,  naturalMinorIntervals},
  { ScaleType::HarmonicMinor, harmonicMinorIntervals},
  { ScaleType::Mixed,         mixedIntervals}
});

struct Scale
{
  std::string _rootName;
  int _rootNum;
  ScaleType _type;
  std::vector<int> _notes;

  Scale(std::string rootName, ScaleType type)
    : _rootName(rootName)
    , _type(type)
    , _rootNum(noteNum(rootName))
    , _notes(notesOfScale(rootName, type))
  {}

  static std::vector<int> notesOfScale(std::string rootName, ScaleType type)
  {
    std::vector<int> result;
    for(auto interval : scaleTypes[type])
      result.push_back((noteNum(rootName) + interval) % 12);
    return result;
  }
};