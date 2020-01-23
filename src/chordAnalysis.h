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
        // DOES NOT SEEM TO BE WORKING!!!!
        flipPair = true;
        interval += 12; 
      }

      if(interval == searchedInterval)
      {
        if(flipPair)
          result.emplace(note1, note2);
        else
          result.emplace(note2, note1);
      }
    }
  }
  
  return result;
}

Chord& resolveChordName(Chord& chord)
{
  // based initially on pure notes
    // check for fifths
      // check for thirds
        // check for sus4/sus2
      // check for 6
      // check for seventh
    //

  // then refined with real notes
    // check inversion
    // check root

  return chord;
}


std::set<Chord> findChordInPos(const ScorePosition& pos)
{
  // Quick escape
  if(pos._notes.size() < 2)
    return {};

  std::set<Chord> result;
  
  // Find interesting pure intervals
  auto majorSeconds = findPureIntervalsInPos(2, pos);
  auto minorThirds = findPureIntervalsInPos(3, pos);
  auto majorThirds = findPureIntervalsInPos(4, pos);
  auto fourths = findPureIntervalsInPos(5, pos);
  auto fifths = findPureIntervalsInPos(7, pos);

  // For every fifth, look if we have a connected triad
  for(auto& fifth : fifths)
  {
    // We start with the 5 chord
    // Not even considered a chord in our logic yet though....
    std::set<int> notes({fifth.first->_num, fifth.second->_num});
    ChordTriads triads = ChordTriads::Invalid;
    ChordInversion inversion = ChordInversion::Invalid;
    std::set<ChordVariations> variations({});
    
    // A triad connected to the lowest note is 100% a chord
    const auto minorThirdItr = std::find_if(ALL(minorThirds), 
      [&](PitchInterval third)->bool{ return third.first == fifth.first; });
    
    const auto majorThirdItr = std::find_if(ALL(majorThirds), 
      [&](PitchInterval third)->bool{ return third.first == fifth.first; });
    
    const bool minorThirdFound = minorThirdItr != minorThirds.end();
    const bool majorThirdFound = majorThirdItr != majorThirds.end();

    if(!minorThirdFound && !majorThirdFound)
    {
      // We have a fifth but no third matching its root

      // Look for a fourth of the lowest note for sus4
      const auto fourthItr = std::find_if(ALL(fourths), 
        [&](PitchInterval fourth)->bool{ return fourth.first == fifth.first; });

      // Look for a major second of the lowest note for sus2
      const auto majorSecondItr = std::find_if(ALL(majorSeconds), 
        [&](PitchInterval second)->bool{ return second.first == fifth.first; });
      
      const bool fourthFound = fourthItr != minorThirds.end();
      const bool majorsecondFound = majorSecondItr != majorThirds.end();

      // Give sus4 priority
      if(fourthFound)
        notes.insert(fourthItr->second->_num);
      // Fallback to sus2
      else if(majorsecondFound)
        notes.insert(majorSecondItr->second->_num);
    }
    else
    {
      // We have found a third intervalmatching the root of the fifth
      if(minorThirdFound && majorThirdFound)
      { 
        /* Let's handle jazz stuff later, check for 9th, 11th, 13th,... */ 
      }
    
      // Look for a major second of the highest note to find a 6 chord
      const auto majorSecondItr = std::find_if(ALL(majorSeconds), 
        [&](PitchInterval second)->bool{ return second.first == fifth.second; });

      notes.insert(minorThirdFound 
        ? minorThirdItr->second->_num 
        : majorThirdItr->second->_num);

      if(minorThirdFound)
      {
        minorThirdItr->second->_num;
        triads = ChordTriads::Min;
      }
      else
      {
        majorThirdItr->second->_num;
        triads = ChordTriads::Maj;
      }
    }

    // Look for a major second above the fifth to find sixth
    const auto sixthItr = std::find_if(ALL(majorSeconds), 
      [&](PitchInterval second)->bool{ return second.first == fifth.second; });
    
    if(sixthItr != majorSeconds.end())
    {
      notes.insert(sixthItr->second->_num);
      variations.insert(ChordVariations::Six);
    }

    // Look for a third above the fifth to find seventh
    const auto minorSeventhItr = std::find_if(ALL(minorThirds), 
      [&](PitchInterval third)->bool{ return third.first == fifth.second; });
    
    const auto majorSeventhItr = std::find_if(ALL(majorThirds), 
      [&](PitchInterval third)->bool{ return third.first == fifth.second; });
    
    const bool minorSeventhFound = minorSeventhItr != minorThirds.end();
    const bool majorSeventhFound = majorSeventhItr != majorThirds.end();
    
    if(minorSeventhFound || majorSeventhFound)
    {
      // We have found a third interval matching the top of the fifth
      if(minorSeventhFound && majorSeventhFound)
      { 
        /* Let's handle jazz stuff later, check for weird inversions possibilities... */ 
      }

      if(minorSeventhFound)
      {
        minorSeventhItr->second->_num;
        variations.insert(ChordVariations::Seven);
      } 
      else
      {
        majorSeventhItr->second->_num;
        variations.insert(ChordVariations::MajorSeven);
      }
          
    }

    Chord chord(notes);
    chord._triads = triads;
    chord._variations = variations;
    resolveChordName(chord);
    result.emplace(chord);
  }

  if(fifths.size() != 0)
  {

    // Diminished chord
    // For each distinct minor third, find a connected minor third
    for(auto& minorThird : minorThirds)
    {
      std::set<int> notes;

      const auto minorThirdItr = std::find_if(ALL(minorThirds), 
        [&](PitchInterval third)->bool{ return third.first == minorThird.second; });
    
      if(minorThirdItr != minorThirds.end())
        notes.insert(
        {
          minorThird.first->_num,
          minorThird.second->_num,
          minorThirdItr->second->_num,
        });

      Chord chord(notes);
      resolveChordName(chord);
      result.emplace(chord);
    }

    // Augmented chord
    // For each distinct major third, find a connected major third
    for(auto& majorThird : majorThirds)
    {
      std::set<int> notes;

      const auto majorThirdItr = std::find_if(ALL(majorThirds), 
        [&](PitchInterval third)->bool{ return third.first == majorThird.second; });
    
      if(majorThirdItr != majorThirds.end())
        notes.insert(
        {
          majorThird.first->_num,
          majorThird.second->_num,
          majorThirdItr->second->_num,
        });

      Chord chord(notes);
      resolveChordName(chord);
      result.emplace(chord);
    }
  }


  return result;
}

// degreeOfChordInScale()

