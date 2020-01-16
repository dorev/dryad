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
      
      // EXPERIMENT
      // The objective here is to have the first element of the output
      // pair being the lowest note of the interval, this way, if we're looking
      // for fifths, pairs returns by this method will always have the root
      // in the first element of the pair
      bool flipPair = false;

      // Measure the interval as if it was from another octave
      if(interval < 0)
      {
        flipPair = true;
        interval += 12; 
      }

      if(interval == searchedInterval)
      {
        if(flipPair)
          result.emplace(note2, note1);
        else
          result.emplace(note1, note2);
      }
    }
  }
  
  return result;
}



std::set<Chord> findChordInPos(const ScorePosition& pos)
{
  // Quick escape
  if(pos._notes.size() < 2)
    return {};

  // Find interesting pure intervals
  auto minorTriads = findPureIntervalsInPos(3, pos);
  auto majorTriads = findPureIntervalsInPos(4, pos);
  auto fifths = findPureIntervalsInPos(7, pos);

  // For every fifth, look if we have a connected triad
  for(auto& fifth : fifths)
  {
    // A triad connected to the lowest note is 100% a chord
    const auto minorTriadItr = std::find_if(ALL(minorTriads), 
      [&](PitchInterval& triad){ return triad.first == fifth.first; });
    
    const auto majorTriadItr = std::find_if(ALL(majorTriads), 
      [&](PitchInterval& triad){ return triad.first == fifth.first; });
    
    const bool minorTriadFound = minorTriadItr == minorTriads.end();
    const bool majorTriadFound = majorTriadItr == majorTriads.end();

    if(!minorTriadFound && !majorTriadFound) 
      continue;

    if(minorTriadFound && majorTriadFound)
      { /* Let's handle jazz stuff later */ }
  
      // Look for triad above fifth to find seventh
    
    // Look for a fourth of the lowest note for sus4
    // Look for a major second of the lowest note for sus2
    // Look for a major second of the highest note to find a 6 chord
  }

  if(fifths.size() != 0)
  {

  }

  // If we have no fifth
    // Look for degrees sequence that might imply a chord
      // 1-3-7/M7
      // 3-5-7/M7
      // 1-3
      // 1-4
    

  return {};
}

// degreeOfChordInScale()

