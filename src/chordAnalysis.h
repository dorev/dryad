#pragma once

#include <deque>
#include <algorithm>

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
      bool flipPair = false;
      if(interval < 0)
      {
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
  // Inversion must be set
  if(chord._inversion == ChordInversion::Invalid)
    throw "Invalid chord, invalid chord inversion";

  std::string name = noteName(chord._root);
  name += toString(chord._triads);
  name += toString(chord._extension);
  
  if(chord._root % 12 != chord._bass % 12)
  {
    name += "/";
    name += noteName(chord._bass);
  }

  chord._name = name;

  return chord;
}

Chord& resolveChordInversion(Chord& chord)
{
  // Chord root and triads must be set  
  if(chord._root < 0 || chord._root > 144 || chord._triads == ChordTriad::Invalid)
    throw "Invalid chord, root range or triads are invalid";

  // Find pure interval between bass and root  
  int interval = (chord._bass % 12) - (chord._root % 12);

  if(interval < 0)
    interval += 12; 

  if(interval == 0)
    chord._inversion = ChordInversion::Root;
  else if(interval == 4 || interval == 5)
    chord._inversion = ChordInversion::Six;
  else if(interval == 7)
    chord._inversion = ChordInversion::FourSix;
  else
    // handle sus2, sus4, 6 and 7 inversions later
    chord._inversion = ChordInversion::NotImplementedYet;

  return chord;
}

Chord& postProcessChord(Chord& chord)
{
  resolveChordInversion(chord);
  resolveChordName(chord);
  return chord;
}


// std::set<Chord> findChordInPos(const ScorePosition& pos)
// {
//   // Quick escape
//   if(pos._notes.size() < 2)
//     return {};

//   std::set<Chord> result;
  
//   // Find interesting pure intervals
//   auto majorSeconds = findPureIntervalsInPos(2, pos);
//   auto minorThirds = findPureIntervalsInPos(3, pos);
//   auto majorThirds = findPureIntervalsInPos(4, pos);
//   auto fourths = findPureIntervalsInPos(5, pos);
//   auto fifths = findPureIntervalsInPos(7, pos);

//   // For every fifth, look if we have a connected triad
//   for(auto& fifth : fifths)
//   {
//     // We start with the 5 chord
//     // Not even considered a chord in our logic yet though....
//     std::set<int> notes({fifth.first->_num, fifth.second->_num});
//     ChordTriad triads = ChordTriad::Invalid;
//     ChordInversion inversion = ChordInversion::Invalid;
//     std::set<ChordExtension> variations({});
    
//     // A triad connected to the lowest note is 100% a chord
//     const auto minorThirdItr = std::find_if(ALL(minorThirds), 
//       [&](PitchInterval third)->bool{ return third.first == fifth.first; });
    
//     const auto majorThirdItr = std::find_if(ALL(majorThirds), 
//       [&](PitchInterval third)->bool{ return third.first == fifth.first; });
    
//     const bool minorThirdFound = minorThirdItr != minorThirds.end();
//     const bool majorThirdFound = majorThirdItr != majorThirds.end();

//     if(!minorThirdFound && !majorThirdFound)
//     {
//       // We have a fifth but no third matching its root

//       // Look for a fourth of the lowest note for sus4
//       const auto fourthItr = std::find_if(ALL(fourths), 
//         [&](PitchInterval fourth)->bool{ return fourth.first == fifth.first; });

//       bool fourthFound = fourthItr != minorThirds.end();

//       // Look for a major second of the lowest note for sus2
//       const auto majorSecondItr = std::find_if(ALL(majorSeconds), 
//         [&](PitchInterval second)->bool{ return second.first == fifth.first; });
      
//       bool majorsecondFound = majorSecondItr != majorThirds.end();

//       // Give sus4 priority
//       if(fourthFound)
//       {
//         notes.insert(fourthItr->second->_num);
//         variations.insert(ChordExtension::Sus4);
//       }
//       // Fallback to sus2
//       else if(majorsecondFound)
//       {
//         notes.insert(majorSecondItr->second->_num);
//         variations.insert(ChordExtension::Sus2);
//       }
//     }
//     else
//     {
//       // We have found a third intervalmatching the root of the fifth
//       if(minorThirdFound && majorThirdFound)
//       { 
//         /* Let's handle jazz stuff later, check for 9th, 11th, 13th,... */ 
//       }
    
//       // Look for a major second of the highest note to find a 6 chord
//       const auto majorSecondItr = std::find_if(ALL(majorSeconds), 
//         [&](PitchInterval second)->bool{ return second.first == fifth.second; });

//       if(minorThirdFound)
//       {
//         notes.insert(minorThirdItr->second->_num);
//         triads = ChordTriad::Minor;
//       }
//       else
//       {
//         notes.insert(majorThirdItr->second->_num);
//         triads = ChordTriad::Major;
//       }
//     }

//     // Look for a major second above the fifth to find sixth
//     const auto sixthItr = std::find_if(ALL(majorSeconds), 
//       [&](PitchInterval second)->bool{ return second.first == fifth.second; });
    
//     if(sixthItr != majorSeconds.end())
//     {
//       notes.insert(sixthItr->second->_num);
//       variations.insert(ChordExtension::Six);
//     }

//     // Look for a third above the fifth to find seventh
//     const auto minorSeventhItr = std::find_if(ALL(minorThirds), 
//       [&](PitchInterval third)->bool{ return third.first == fifth.second; });
    
//     const auto majorSeventhItr = std::find_if(ALL(majorThirds), 
//       [&](PitchInterval third)->bool{ return third.first == fifth.second; });
    
//     bool minorSeventhFound = minorSeventhItr != minorThirds.end();
//     bool majorSeventhFound = majorSeventhItr != majorThirds.end();
    
//     if(minorSeventhFound || majorSeventhFound)
//     {
//       // We have found a third interval matching the top of the fifth
//       if(minorSeventhFound && majorSeventhFound)
//       { 
//         /* Let's handle jazz stuff later, check for weird inversions possibilities... */ 
//       }

//       if(minorSeventhFound)
//       {
//         notes.insert(minorSeventhItr->second->_num);
//         variations.insert(ChordExtension::Seven);
//       } 
//       else
//       {
//         notes.insert(majorSeventhItr->second->_num);
//         variations.insert(ChordExtension::MajorSeven);
//       }
          
//     }

//     // 9th?


//     // 11th?


//     Chord chord(notes);
//     chord._root = fifth.first->_num;
//     chord._triads = triads;
//     chord._extension = variations;
//     postProcessChord(chord);
//     result.emplace(chord);
//   }

//   if(fifths.size() == 0)
//   {
//     // Diminished chord
//     // For each distinct minor third, find a connected minor third
//     for(auto& minorThird : minorThirds)
//     {
//       const auto minorThirdItr = std::find_if(ALL(minorThirds), 
//         [&](PitchInterval third)->bool{ return third.first == minorThird.second; });
    
//       if(minorThirdItr != minorThirds.end())
//       {
//         Chord chord
//         ({
//           minorThird.first->_num,
//           minorThird.second->_num,
//           minorThirdItr->second->_num,
//         });
//         chord._root = minorThird.first->_num;
//         chord._triads = ChordTriad::Dim;
//         postProcessChord(chord);
//         result.emplace(chord);
//       }
//     }

//     // Augmented chord
//     // For each distinct major third, find a connected major third
//     for(auto& majorThird : majorThirds)
//     {
//       const auto majorThirdItr = std::find_if(ALL(majorThirds), 
//         [&](PitchInterval third)->bool{ return third.first == majorThird.second; });
    
//       if(majorThirdItr != majorThirds.end())
//       {
//         Chord chord
//         ({
//           majorThird.first->_num,
//           majorThird.second->_num,
//           majorThirdItr->second->_num,
//         });
//         chord._root = majorThird.first->_num;
//         chord._triads = ChordTriad::Aug;
//         postProcessChord(chord);
//         result.emplace(chord);
//       }
//     }
//   }

//   // post process chords to 


//   return result;
// }

// degreeOfChordInScale()



std::set<Chord> findChordInPos(const ScorePosition& pos)
{
  // Quick escape
  if(pos._notes.size() < 3)
    return {};

  // Load absolute notes
  std::deque<int> notes;
  for(auto& note : pos._notes)
    notes.push_back(note._num % 12 );

  // Prepare notes rotations
  std::set<std::deque<int>> rotations;
  rotations.insert(notes);
  for(int i = 0; i < notes.size()-1; ++i)
  {
    notes.push_back(notes.front() + 12);
    notes.pop_front();
    rotations.insert(notes);
  }

  // Generate chord intervals
  std::set<std::pair<std::deque<int>, ChordIntervals>> intervalsSet;
  for(auto& notes : rotations)
  {
    ChordIntervals intervals;
    for(int i = 0; i < notes.size()-1; ++i)
      intervals.push_back(notes[i+1] - notes[i]);
    
    intervalsSet.insert(std::make_pair(notes, intervals));
  }

  std::set<Chord> chordsFound;

  for(auto& notesAndIntervals : intervalsSet)
  {
    // notes dont follow interval rotation!!!
    auto notes = notesAndIntervals.first;
    auto intervals = notesAndIntervals.second;
    
    auto itr = __chordsTypes.find(intervals);
    if(itr != __chordsTypes.end())
    {
      std::set<int> chordNotes;
      std::for_each(ALL(pos._notes), [&](const Pitch& pitch){ chordNotes.insert(pitch._num);});
      Chord chord(chordNotes);

      chord._root = *notes.begin();
      chord._triads = itr->second.triads;
      chord._extension = itr->second.extension;
      postProcessChord(chord);
      chordsFound.insert(chord);
    }
  }

  return chordsFound;
}





