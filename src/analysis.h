#pragma once

#include "score.h"
#include "scales.h"
#include "chords.h"

using NotePairList = std::set<std::pair<const Pitch*,const Pitch*>> ;

NotePairList findIntervalsInPos(int semitoneInterval, const ScorePosition& pos)
{
  NotePairList output = {};

  std::set<const Pitch*> allSounds;
  for(auto& note : pos._notes)
    allSounds.insert(&note);    

  allSounds.insert(ALL(pos._resonatingNotes));

  for(auto& note1 : allSounds)
  {
    for(auto& note2 : allSounds)
    {
      if(note1 == note2)
        continue;

      if(note1->_num - note2->_num == semitoneInterval)
        output.emplace(note1, note2);
    }
  }
  return output;
}

std::vector<Scale> findScalesOfChord(const Chord& chord)
{
  std::vector<Scale> matchingScales;

  for(auto scale : __scaleList)
  {
    auto scaleType = scale.first;
    auto scaleIntervals = scale.second;

    for(int root : __notesIndex)
    {
      // Shift notes of interval
      std::vector<int> shiftedIntervals;
      std::transform(ALL(scaleIntervals), std::back_inserter(shiftedIntervals), 
      [&](int interval){ return (interval + root) % 12;});

      // Skip if any not of the chord doesn't match the scale
      if(std::any_of(ALL(chord._pureNotes), 
         [&](int i){ return std::find(ALL(shiftedIntervals), i) == shiftedIntervals.end();}))
        continue;

      matchingScales.push_back(Scale(root, scaleType));
    }
  }
  return matchingScales;
}

std::vector<Scale> findScalesInRange(const Score& score, int startPos, int endPos)
{
  // list all present notes
  std::map<int,int> notesOccurences;

  // find chords
  // how do we represent chords?


  return std::vector<Scale>();
}