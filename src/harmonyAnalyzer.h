#pragma once

#include "definitions.h"
#include "analysis.h"

// set scales for measures
  // set according degrees for each note
  // set according degrees for each chord

void setScoreMeasuresScales(Score& score)
{
  for(auto& measureKV : score._measures)
  {
    int measureNum = measureKV.first;
    auto measure = measureKV.second;

    measure._scales = findScalesOfMeasure(score, measureNum);
  }
}



void weigthMeasuresScalesBasedOnKeySignature(Score& score)
{
  auto scalesOfScore = findScalesOfScore(score);

  std::multimap<Scale, std::pair<int,int> > rangeOfScales;

  if(scalesOfScore.size() < 1)
    return;
  else if(scalesOfScore.size() == 1)
  {
    auto& scale = scalesOfScore.begin()->second;  
    rangeOfScales.emplace(scale, 0, score._lastPos);
  }
  else
  {
    for(auto itr = scalesOfScore.begin(); itr != scalesOfScore.end(); ++itr)
    {
      // Find last position before next key change
      auto next = std::next(itr,1);
      int endOfRange = next != scalesOfScore.end() ? next->first - 1 : score._lastPos;
      
      //
      auto& scale = itr->second;
      int startOfRange = itr->first;
      rangeOfScales.emplace(scale, startOfRange, endOfRange);      
    }

  }


  

  // if a top measure scale is the same declared by the prior key signature
    // identify it as most significant scale
  
  // identify the degrees of the chords of these measures based on the measure's scales
  
}