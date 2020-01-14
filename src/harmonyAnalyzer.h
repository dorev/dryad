#pragma once

#include "definitions.h"
#include "analysis.h"

// set scales for measures
  // set according degrees for each note
  // set according degrees for each chord

void setScoreMeasuresScales(Score& score)
{
  for(auto& measureKV : score._measures)
    measureKV.second._scales = findScalesOfMeasure(score, measureKV.first);
}

void weigthMeasuresScales(Score& score)
{
  auto scalesOfScore = findScalesOfScore(score);

  // if a top measure scale is the same declared by the prior key signature
    // identify it as most significant scale
  
  // identify the degrees of the chords of these measures based on the measure's scales
  
}