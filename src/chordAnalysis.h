#pragma once

#include "score.h"
#include "chords.h"

using Interval = std::pair<const Pitch*,const Pitch*>;
using IntervalList = std::set<Interval> ;

IntervalList findIntervalsInPos(int semitoneInterval, const ScorePosition& pos)
{
  IntervalList output;

  // Build set of all notes heard at that position
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

