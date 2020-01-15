#pragma once

#include "score.h"
#include "chords.h"

using Interval = std::pair<int, int>;
using IntervalsList = std::set<Interval>;
using PitchInterval = std::pair<const Pitch*,const Pitch*>;
using PitchIntervalsList = std::set<PitchInterval>;

std::set<const Pitch*> everyPitchInPos(const ScorePosition& pos)
{
  // Build set of all notes heard at that position
  std::set<const Pitch*> allNotes;
  for(auto& note : pos._notes)
    allNotes.insert(&note);    

  allNotes.insert(ALL(pos._resonatingNotes));

  return allNotes;
}

PitchIntervalsList findIntervalsInPos(int semitoneInterval, const ScorePosition& pos)
{
  PitchIntervalsList output;

  std::set<const Pitch*> allSounds = everyPitchInPos(pos);
  
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


IntervalsList findIntervalsInPureNotes(int interval, std::set<int> notes)
{

  
  return {};
}

std::set<Chord> findChordInPos(const ScorePosition& pos)
{
  // how to identify chords qualities
  
  // what's the maximum amount of notes 
  //I want to deal with in a chord (4? 5? more?)

  // what makes a chord!?
    // root + fifth or root + third...
    // let's start with that!!

  std::set<const Pitch*> pitches = everyPitchInPos(pos);
  std::set<int> pureNotes;

  // Extract pure notes from pitch
  std::for_each(ALL(pitches), 
    [&](auto pitch) { pureNotes.insert(pitch->_num % 12); });

  // Find interesting intervals in pure notes

  auto minorTriads = findIntervalsInPureNotes(3, pureNotes);
  auto majorTriads = findIntervalsInPureNotes(4, pureNotes);
  auto majorFifths = findIntervalsInPureNotes(7, pureNotes);

  return {};
}

// degreeOfChordInScale()

