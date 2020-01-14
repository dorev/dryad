#pragma once

#include "score.h"
#include "scales.h"
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


std::set<Scale> listScalesOfUniqueNotes(std::set<int> uniqueNotes)
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
      if(std::any_of(ALL(uniqueNotes), 
         [&](int i){ return std::find(ALL(shiftedIntervals), i) == shiftedIntervals.end();}))
        continue;

      matchingScales.emplace(Scale(root, scaleType));
    }
  }
  
  return matchingScales;  
}


std::set<Scale> listScalesOfChord(const Chord& chord)
{
  return listScalesOfUniqueNotes(chord._pureNotes);
}


std::set<Scale> listScalesOfNotes(const std::set<int>& notes)
{
  std::set<int> uniqueNotes;
  std::for_each(ALL(notes), [&](int note){ uniqueNotes.insert(note % 12); });

  return listScalesOfUniqueNotes(uniqueNotes);
}


std::map<int,int> noteOccurencesInRange(const Score& score, int startPos, int endPos)
{
  std::map<int,int> result;

  for(int pos : score.findPosInRange(startPos, endPos))
    for(auto note : score[pos]->_notes)
      if(result.find(note._num) == result.end())
        result[note._num] = 0;
      else
        result[note._num]++;

  return result;
}


std::map<int, Scale> findScalesOfScore(const Score& score)
{
  std::map<int, Scale> scalesFound;

  for (auto& keyNode : score._xml->select_nodes("//key"))
  {
    // Gather "fifths" and "mode" values of key nodes
    int fifths = keyNode.node().child("fifths").text().as_int();
    bool minorMode = strcmp(keyNode.node().child("mode").text().as_string(), "minor");

    int circleOfFifthIndex = fifths >= 0 ? fifths : fifths + 12;
    
    Scale scale(circleOfFifths[circleOfFifthIndex]);

    // Find the measure of the key change
    int measure = keyNode.parent().parent().attribute("number").as_int();

    if(scalesFound.find(measure) == scalesFound.end())
    {
      std::cerr << "Multiple key change found in same measure of score\n"
                << (minorMode ? relativeMinor(scale) : scale).toString() 
                << " at measure " << measure << "\n";
    }

    scalesFound.emplace(measure, minorMode ? relativeMinor(scale) : scale);
  }

  return scalesFound;
}


std::multimap<int, Scale> countAndSortScales(std::set<Scale> scales)
{
  // Structure go store and count scales
  std::map<Scale, int> scaleOccurences;

  // Fill scaleOccurences map
  for(auto& scale : scales)
    scaleOccurences.find(scale) != scaleOccurences.end() 
      ? scaleOccurences[scale]++ 
      : scaleOccurences[scale] = 1;

  // Sort found scales by occurences
  std::multimap<int, Scale> sortedScaleOccurences;

  for(auto scaleOccurence : scaleOccurences)
    sortedScaleOccurences.emplace(scaleOccurence.second, scaleOccurence.first);

  return sortedScaleOccurences;
}


std::multimap<int, Scale> findScalesOfMeasure(Score& score, int measureNum)
{
  auto measureFound = score._measures.find(measureNum);

  if(measureFound == score._measures.end())
    return {};
  
  auto& measure = measureFound->second;
  
  // Gather all notes of measure
  std::set<int> notes;
  for(auto pos : measure._scorePositions)
    for(auto& note : pos->_notes)
      notes.insert(note._num);

  // List scales based on found notes
  std::set<Scale> scalesOfMeasure = listScalesOfNotes(notes);
  
  return countAndSortScales(scalesOfMeasure);
}


std::multimap<int, Scale> findScalesInRange(const Score& score, int startPos, int endPos)
{
  if(endPos < startPos)
    throw "Invalid range asked to findScalesInRange";

  std::set<int> activePos = score.findPosInRange(startPos, endPos);

  // Gather all notes of range
  std::set<int> notes;
  for(auto pos : activePos)
    for(auto& note : score[pos]->_notes)
      notes.insert(note._num);

  // List scales based on found notes
  std::set<Scale> scalesOfRange = listScalesOfNotes(notes);
  
  return countAndSortScales(scalesOfRange);
}