#pragma once

#include "score.h"
#include "scales.h"
#include "chords.h"

using NotePairList = std::set<std::pair<const Pitch*,const Pitch*>> ;

NotePairList findIntervalsInPos(int semitoneInterval, const ScorePosition& pos)
{
  NotePairList output;

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

std::vector<Scale> findScalesOfChord(const Chord& chord)
{
  std::vector<Scale> matchingScales;

  // For all possible scales
  for(auto scale : __scaleList)
  {
    auto scaleType = scale.first;
    auto scaleIntervals = scale.second;

    // For all possible roots
    for(int root : __notesIndex)
    {
      // Shift scale notes based on scale intervals
      std::vector<int> shiftedIntervals;
      std::transform(ALL(scaleIntervals), std::back_inserter(shiftedIntervals), 
      [&](int interval){ return (interval + root) % 12;});

      // Skip scale if any note of the chord doesn't fit in
      if(std::any_of(ALL(chord._pureNotes), 
         [&](int i){ return std::find(ALL(shiftedIntervals), i) == shiftedIntervals.end();}))
        continue;

      matchingScales.push_back(Scale(root, scaleType));
    }
  }
  return matchingScales;
}

std::vector<Scale> findScalesOfNotes(const std::vector<int>& notes)
{
  std::vector<Scale> matchingScales;

  // List pure notes
  std::set<int> uniqueNotes;
  std::for_each(ALL(notes), [&](int note){ uniqueNotes.insert(note % 12); });

  // For all possible scales
  for(auto scale : __scaleList)
  {
    auto scaleType = scale.first;
    auto scaleIntervals = scale.second;

    // For all possible roots
    for(int root : __notesIndex)
    {
      // Shift scale notes based on scale intervals
      std::vector<int> shiftedIntervals;
      std::transform(ALL(scaleIntervals), std::back_inserter(shiftedIntervals), 
      [&](int interval){ return (interval + root) % 12; });

      // Skip scale if any note of the chord doesn't fit in
      if(std::any_of(ALL(uniqueNotes), 
         [&](int i){ return std::find(ALL(shiftedIntervals), i) == shiftedIntervals.end(); }))
        continue;

      matchingScales.push_back(Scale(root, scaleType));
    }
  }
  return matchingScales;
}


std::map<int,int> noteOccurencesInRange(const Score& score, int startPos, int endPos)
{
  std::map<int,int> result;

  for(int pos : score.findPosInRange(startPos, endPos))
  {
    for(auto note : score[pos]->_notes)
    {
      if(result.find(note._num) == result.end())
        result[note._num] = 0;
      else
        result[note._num]++;
    }
  }

  return result;
}


std::vector<Scale> findScalesInRange(const Score& score, int startPos, int endPos)
{
  // list all present notes
  // find chords
  // how do we represent chords?


  return std::vector<Scale>();
}