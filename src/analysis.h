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
  for(auto& note : pos.notes)
    allSounds.insert(&note);    

  allSounds.insert(ALL(pos.resonatingNotes));

  for(auto& note1 : allSounds)
  {
    for(auto& note2 : allSounds)
    {
      if(note1 == note2)
        continue;

      if(note1->num - note2->num == semitoneInterval)
        output.emplace(note1, note2);
    }
  }
  return output;
}


std::set<Scale> findScalesOfChord(const Chord& chord)
{
  std::set<Scale> matchingScales;

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

      matchingScales.emplace(Scale(root, scaleType));
    }
  }
  return matchingScales;
}


std::set<Scale> findScalesOfNotes(const std::set<int>& notes)
{
  std::set<Scale> matchingScales;

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

      matchingScales.emplace(Scale(root, scaleType));
    }
  }
  return matchingScales;
}


std::map<int,int> noteOccurencesInRange(const Score& score, int startPos, int endPos)
{
  std::map<int,int> result;

  for(int pos : score.findPosInRange(startPos, endPos))
    for(auto note : score[pos]->notes)
      if(result.find(note.num) == result.end())
        result[note.num] = 0;
      else
        result[note.num]++;

  return result;
}


std::set<Scale> findScalesOfScore(const Score& score)
{
  std::set<Scale> scalesFound;

  for (auto& keyNode : score.xml->select_nodes("//key"))
  {
    // Gather "fifths" and "mode" values of key nodes
    int fifths = keyNode.node().child("fifths").text().as_int();
    bool minorMode = strcmp(keyNode.node().child("mode").text().as_string(), "minor");

    int circleOfFifthIndex = fifths >= 0 ? fifths : fifths + 12;
    
    Scale scale(circleOfFifths[circleOfFifthIndex]);

    scalesFound.emplace(minorMode ? relativeMinor(scale) : scale);
  }

  return scalesFound;
}


std::multimap<int, Scale> findScalesByMeasure(const Score& score, int startPos = -1, int endPos = -1)
{
  if(startPos == endPos && startPos == -1)
  {
    startPos = 0;
    endPos = score.score.rbegin()->first;
  }
  else if(endPos < startPos)
    throw "Invalid range asked to findScalesByMeasure";

  std::set<int> activePos = score.findPosInRange(startPos, endPos);

  // Group score positions by measure
  std::map<int, std::set<const ScorePosition*>> scorePosByMeasure;

  for(int pos : activePos)
    scorePosByMeasure[score[pos]->measure()].insert(score[pos]);

  // Find unique scales by measures
  std::map<Scale, int> scaleOccurences;

  for(auto measure : scorePosByMeasure)
  {
    // Gather all notes of measure
    std::set<int> notes;
    for(auto pos : measure.second)
      for(auto& note : pos->notes)
        notes.insert(note.num);

    // List scales based on found notes
    std::set<Scale> scalesOfMeasure = findScalesOfNotes(notes);
    
    // Fill scaleOccurences map
    for(auto& scale : scalesOfMeasure)
      scaleOccurences.find(scale) != scaleOccurences.end() 
        ? scaleOccurences[scale]++ 
        : scaleOccurences[scale] = 1;
  }

  // Sort found scales by occurences
  std::multimap<int, Scale> sortedScaleOccurences;

  for(auto scaleOccurence : scaleOccurences)
    sortedScaleOccurences.emplace(scaleOccurence.second, scaleOccurence.first);

  return sortedScaleOccurences;
}