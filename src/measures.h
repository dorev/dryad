#pragma once

#include "definitions.h"
#include "scales.h"
#include "scorePosition.h"

struct Measure
{
  std::set<const ScorePosition*> _scorePositions;
  std::multimap<int, Scale> _scales;

  Measure(std::set<const ScorePosition*> scorePositions = {}, std::multimap<int,Scale> scales = {})
    : _scorePositions(scorePositions)
    , _scales(scales)
    {}

  std::set<int> allNotes()
  {
    std::set<int> notes;
    for(auto pos : _scorePositions)
      for(auto& note : pos->_notes)
        notes.insert(note._num);
    
    return notes;
  }
};