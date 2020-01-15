#pragma once

#include "score.h"
#include "chords.h"

using Interval = std::pair<int, int>;
using IntervalList = std::set<Interval>;
using PitchInterval = std::pair<const Pitch*,const Pitch*>;
using PitchIntervalList = std::set<PitchInterval>;

std::set<const Pitch*> allSoundsOfPos(const ScorePosition& pos)
{
  // Build set of all notes heard at that position
  std::set<const Pitch*> allNotes;
  for(auto& note : pos._notes)
    allNotes.insert(&note);    

  allNotes.insert(ALL(pos._resonatingNotes));

  return allNotes;
}

PitchIntervalList findIntervalsInPos(int semitoneInterval, const ScorePosition& pos)
{
  PitchIntervalList output;

  std::set<const Pitch*> allSounds = allSoundsOfPos(pos);
  
  for(auto& note1 : allSounds)
  {
    for(auto& note2 : allSounds)
    {
      if(note1 == note2)
        continue;

      // Store pair of note matching the requested interval
      if(note1->_num - note2->_num == semitoneInterval)
        output.emplace(note1, note2);
    }
  }
  return output;
}

// Is that relevant??!?! Chord are unlikely to have 
// consecutive triads, this is not the good approach
std::set<IntervalList> findConsecutiveTriads(const ScorePosition& pos)
{
  auto minorTriads = findIntervalsInPos(3, pos);
  auto majorTriads = findIntervalsInPos(4, pos);

  return {};
}

// structure for Chord vs Scale Degree

// findChordInPos()

// degreeOfChordInScale()

