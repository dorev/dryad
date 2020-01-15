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

// Returns a map indicating at what measure new scales are declared in the key signature
std::map<int, Scale> findKeySignaturesOfScore(const Score& score)
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

using SortedScalesMap = std::multimap<int, Scale>;

template <typename T1, typename T2>
std::multimap<T2, T1> reverseMapToMultimap(const std::map<T1, T2> map)
{
  // Sort found scales by occurences
  std::multimap<T2, T1> multimap;

  for(auto kv : map)
    multimap.emplace(kv.second, kv.first);

  return multimap;
}


SortedScalesMap combineScalesMaps(std::multiset<SortedScalesMap> scalesMaps)
{
  // Structure to store and count scales
  std::map<Scale, int> combinedScaleOccurences;

  for(auto& scalesMap : scalesMaps)
  {
    for(auto& scaleKV : scalesMap)
    {
      int scaleOccurences = scaleKV.first;
      auto& scale = scaleKV.second;

      auto itrFound = combinedScaleOccurences.find(scale);

      // Combine scale occurences (initial entry or add)
      if(itrFound == combinedScaleOccurences.end())
        combinedScaleOccurences[scale] = scaleOccurences;
      else
        combinedScaleOccurences[scale] += scaleOccurences;      
    }
  }

  return reverseMapToMultimap(combinedScaleOccurences);
}


SortedScalesMap countAndSortScales(std::set<Scale> scales)
{
  // Structure to store and count scales
  std::map<Scale, int> scaleOccurences;

  // Fill scaleOccurences map
  for(auto& scale : scales)
    scaleOccurences.find(scale) != scaleOccurences.end() 
      ? scaleOccurences[scale]++ 
      : scaleOccurences[scale] = 1;

  
  return reverseMapToMultimap(scaleOccurences);
}

// Return a map sorting scales by occurence 
SortedScalesMap findScalesOfMeasure(Score& score, int measureNum)
{
  auto measureFound = score._measures.find(measureNum);

  if(measureFound == score._measures.end())
    return {};
  
  auto& measure = measureFound->second;
  
  return countAndSortScales(listScalesOfNotes(measure.allUniqueNotes()));
}


SortedScalesMap findScalesInRange(const Score& score, int startPos, int endPos)
{
  if(endPos < startPos)
    throw "Invalid range asked to findScalesInRange";
    
  return countAndSortScales(listScalesOfNotes(score.listUniqueNotesInRange(startPos, endPos)));
}