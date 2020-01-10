#pragma once

#include "definitions.h"
#include "scales.h"

struct Measure
{
  int num;
  std::set<int> scorePositions;
  std::multimap<int, Scale> scales;

  Measure(int measureNum, std::set<int> scorePositions = emptyIntSet, std::multimap<int,Scale> scales = emptyScaleMultimap) 
    : num(measureNum)
    {
      
    }

};