#pragma once

#include <deque>
#include <algorithm>

#include "score.h"
#include "chord.h"

using Interval = std::pair<int, int>;
using IntervalsList = std::set<Interval>;
using PitchInterval = std::pair<const Pitch *, const Pitch *>;
using PitchIntervalsList = std::set<PitchInterval>;

std::set<const Pitch *> everyPitchInPos(const ScorePosition &pos)
{
  // Build set of all notes heard at that position
  std::set<const Pitch *> allNotes;

  // Add pointer to all position's notes
  for (const auto &note : pos._notes)
    allNotes.insert(&note);

  // Add all pointers to resonating notes
  allNotes.insert(ALL(pos._resonatingNotes));

  return allNotes;
}

PitchIntervalsList findIntervalsInPos(int searchedInterval, const ScorePosition &pos)
{
  PitchIntervalsList output;

  const std::set<const Pitch *> allSounds = everyPitchInPos(pos);

  // For every note bi-directional combination
  for (auto &note1 : allSounds)
  {
    for (auto &note2 : allSounds)
    {
      if (note1 == note2)
        continue;

      // Store pair of note matching the requested interval
      if (note1->_num - note2->_num == searchedInterval)
        output.emplace(note1, note2);
    }
  }
  return output;
}

PitchIntervalsList findPureIntervalsInPos(int searchedInterval, const ScorePosition &pos)
{
  PitchIntervalsList result;

  const std::set<const Pitch *> allSounds = everyPitchInPos(pos);

  // For every note bi-directional combination
  for (auto &note1 : allSounds)
  {
    for (auto &note2 : allSounds)
    {
      if (note1 == note2)
        continue;

      int interval = (note1->_num % 12) - (note2->_num % 12);

      // Measure the interval as if it was from another octave
      bool flipPair = false;
      if (interval < 0)
      {
        flipPair = true;
        interval += 12;
      }

      if (interval == searchedInterval)
      {
        if (flipPair)
          result.emplace(note1, note2);
        else
          result.emplace(note2, note1);
      }
    }
  }

  return result;
}

Chord &resolveChordName(Chord &chord)
{
  // Inversion must be set
  if (chord._inversion == ChordInversion::Invalid)
    throw "Invalid chord, invalid chord inversion";

  std::string name = noteName(chord._root);
  name += toString(chord._triads);

  if ((chord._extension != ChordExtension::None) &&
          chord._triads == ChordTriad::Sus2 ||
      chord._triads == ChordTriad::Sus4)
    name += "-";

  name += toString(chord._extension);

  if (chord._root % 12 != chord._bass % 12)
  {
    name += "/";
    name += noteName(chord._bass);
  }

  chord._name = name;

  return chord;
}

Chord &resolveChordInversion(Chord &chord)
{
  // Chord root and triads must be set
  if (chord._root < 0 || chord._root > 144 || chord._triads == ChordTriad::Invalid)
    throw "Invalid chord, root range or triads are invalid";

  // Find pure interval between bass and root
  int interval = (chord._bass % 12) - (chord._root % 12);

  if (interval < 0)
    interval += 12;

  if (interval == 0)
    chord._inversion = ChordInversion::Root;
  else if (interval == 4 || interval == 5)
    chord._inversion = ChordInversion::Six;
  else if (interval == 7)
    chord._inversion = ChordInversion::FourSix;
  else
    // handle sus2, sus4, 6 and 7 inversions later
    chord._inversion = ChordInversion::NotImplementedYet;

  return chord;
}

Chord &postProcessChord(Chord &chord)
{
  resolveChordInversion(chord);
  resolveChordName(chord);
  return chord;
}

std::set<Chord> findChordInPos(const ScorePosition &pos)
{
  // Quick escape
  if (pos._notes.size() < 3)
    return {};

  // Load absolute notes
  std::deque<int> notes;
  for (auto &note : pos._notes)
    notes.push_back(note._num % 12);

  // Prepare notes rotations
  std::set<std::deque<int>> rotations;
  rotations.insert(notes);
  for (int i = 0; i < notes.size() - 1; ++i)
  {
    notes.push_back(notes.front() + 12);
    notes.pop_front();
    rotations.insert(notes);
  }

  // Generate chord intervals
  std::set<std::pair<std::deque<int>, ChordIntervals>> intervalsSet;
  for (auto &notes : rotations)
  {
    ChordIntervals intervals;
    for (int i = 0; i < notes.size() - 1; ++i)
      intervals.push_back(notes[i + 1] - notes[i]);

    intervalsSet.insert(std::make_pair(notes, intervals));
  }

  std::set<Chord> chordsFound;
  int rotationCount = 0;

  for (auto &notesAndIntervals : intervalsSet)
  {
    auto notes = notesAndIntervals.first;
    auto intervals = notesAndIntervals.second;

    auto itr = __chordsTypes.find(intervals);
    if (itr != __chordsTypes.end())
    {
      std::deque<int> chordNotes;
      std::for_each(ALL(pos._notes), [&](const Pitch &pitch) { chordNotes.push_back(pitch._num); });

      for (int i = 0; i < rotationCount; ++i)
      {
        chordNotes.push_back(chordNotes.front());
        chordNotes.pop_front();
      }
      ++rotationCount;

      std::set<int> rotatedChordNotes;
      std::for_each(ALL(rotatedChordNotes), [&](int note) { rotatedChordNotes.insert(note); });

      Chord chord(rotatedChordNotes);

      chord._root = *notes.begin();
      chord._triads = itr->second.triads;
      chord._extension = itr->second.extension;
      postProcessChord(chord);
      chordsFound.insert(chord);
    }
  }

  return chordsFound;
}
