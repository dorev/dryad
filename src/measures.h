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
};