#pragma once

#include "definitions.h"
#include "scales.h"

struct Measure
{
  int _num;
  std::set<int> _scorePositions;
  std::multimap<int, Scale> _scales;

  Measure(int measureNum, std::set<int> scorePositions = {}, std::multimap<int,Scale> scales = {}) 
    : _num(measureNum)
    , _scorePositions(scorePositions)
    , _scales(scales)
    {}

  bool operator<(const Measure& other) const
  {
    return _num < other._num;
  }

  bool operator==(const Measure& other) const
  {
    return _num == other._num
        && _scorePositions == other._scorePositions
        && _scales == other._scales;
  }
};