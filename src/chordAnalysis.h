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

  // Add pointer to all position's notes
  for(const auto& note : pos._notes)
    allNotes.insert(&note);    

  // Add all pointers to resonating notes
  allNotes.insert(ALL(pos._resonatingNotes));

  return allNotes;
}

PitchIntervalsList findIntervalsInPos(int searchedInterval, const ScorePosition& pos)
{
  PitchIntervalsList output;

  const std::set<const Pitch*> allSounds = everyPitchInPos(pos);
  
  // For every note bi-directional combination
  for(auto& note1 : allSounds)
  {
    for(auto& note2 : allSounds)
    {
      if(note1 == note2)
        continue;

      // Store pair of note matching the requested interval
      if(note1->_num - note2->_num == searchedInterval)
        output.emplace(note1, note2);
    }
  }
  return output;
}


PitchIntervalsList findPureIntervalsInPos(int searchedInterval, const ScorePosition& pos)
{
  PitchIntervalsList result;

  const std::set<const Pitch*> allSounds = everyPitchInPos(pos);

  // For every note bi-directional combination
  for(auto& note1 : allSounds)
  {
    for(auto& note2 : allSounds)
    {
      if(note1 == note2)
        continue;
      
      int interval = (note1->_num % 12) - (note2->_num % 12);
      
      // Measure the interval as if it was from another octave
      if(interval < 0)
        interval += 12; 

      if(interval == searchedInterval)
        result.emplace(note1, note2);
    }
  }
  
  return result;
}



std::set<Chord> findChordInPos(const ScorePosition& pos)
{
  // Find interesting intervals in pure notes

  auto minorTriads = findPureIntervalsInPos(3, pos);
  auto majorTriads = findPureIntervalsInPos(4, pos);
  auto majorFifths = findPureIntervalsInPos(7, pos);

  // For every major 5th, look if we have a connected triad
    // A triad connected to the lowest note is 100% a chord
      // Look for triad above 5th to find seventh
    
    // Look for a major fourth of the lowest note for sus4
    // Look for a major second of the lowest note for sus2
    // Look for a major second of the highest note to find a 6 chord

  
    

  return {};
}

// degreeOfChordInScale()

