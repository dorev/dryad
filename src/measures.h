#pragma once

#include "definitions.h"
#include "scales.h"
#include "scorePosition.h"

struct Measure
{
  std::set<const ScorePosition*> _scorePositions;
  std::multimap<int, Scale> _scales;
  Scale _keySignature;

  Measure(std::set<const ScorePosition*> scorePositions = {}, 
          std::multimap<int,Scale> scales = {},
          Scale keySignature = Scale())
    : _scorePositions(scorePositions)
    , _scales(scales)
    , _keySignature()
    {}

  std::set<int> allUniqueNotes()
  {
    std::set<int> notes;
    for(auto pos : _scorePositions)
      for(auto& note : pos->_notes)
        notes.insert(note._num);
    
    return notes;
  }
};