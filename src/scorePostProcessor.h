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

void setMeasuresKeySignature(Score& score)
{
  auto keySignatures = findKeySignaturesOfScore(score);

  // Define the range of the score covered by the declared scales
  std::multimap<Scale, std::pair<int,int> > rangesOfScales;

  // If key signature is only declared once in the score
  if(keySignatures.size() == 1)
  {
    const auto& scale = keySignatures.begin()->second;  
    rangesOfScales.emplace(scale, std::make_pair(0, score._measures.rbegin()->first + 1));
  }
  else if(keySignatures.size() > 1)
  {
    for(auto itr = keySignatures.begin(); itr != keySignatures.end(); ++itr)
    {
      // Find last position before next key change
      const auto next = std::next(itr,1);
      const int endOfRange = next != keySignatures.end() ? next->first : score._measures.rbegin()->first;
      
      // Insert scale and range
      const auto& scale = itr->second;
      const int startOfRange = itr->first;
      rangesOfScales.emplace(scale, std::make_pair(startOfRange, endOfRange));      
    }
  }

  // For each range of key signature
  for(const auto& rangeOfScale : rangesOfScales)
  {
    const auto& scale = rangeOfScale.first;
    const int startMeasure = rangeOfScale.second.first;
    const int endMeasure = rangeOfScale.second.second;

    // Check every contained measure and set key signature
    for(int measureNum = startMeasure; measureNum < endMeasure; ++measureNum)
    {
      const auto measureItr = score._measures.find(measureNum);
      
      if(measureItr != score._measures.end())
        measureItr->second._keySignature = scale;
    }
  }  
}


// maybe put that in analysis
// detect chords (find consecutive triads, inversions, stuff....)
// compare chords to scales to find their degrees
// chords of degree I, IV, V (+) and Imaj7, V7 (++) increase the relevance of a scale