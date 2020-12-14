#pragma once

#include "degree.h"
#include "scaleAnalysis.h"
#include "chordAnalysis.h"

// find degree(s) of chord at position

Degree degreeOfChordInScale(const Chord &chord, const Scale &scale)
{
  // Check if chord notes are all in scale
  bool notesMatch = std::all_of(ALL(chord._absoluteNotes), [&](int chordNote) {
      return std::find_if(ALL(scale._notes), [&](int scaleNote)
      {
        return scaleNote == chordNote;
      }) != scale._notes.end();
  });

  // Return invalid Degree
  if (!notesMatch)
    return Degree();

  // Find index of root in scale
  int chordRoot = chord._root % 12;
  int scaleRootIndex = -1;
  int scaleSize = scale._notes.size();

  for (int i = 0; i < scaleSize; ++i)
    if (scale._notes[i] == scale._root)
    {
      scaleRootIndex = i;
      break;
    }

  for (int i = 0; i < scaleSize; ++i)
    if (scale._notes[(i + scaleRootIndex) % scaleSize] == chordRoot)
      return Degree(i, scale);

  return Degree();
}

Scale findMostLikelyScaleAtPos(const ScorePosition &pos)
{
  // look at key signature
  // look at absolute notes over the last X measures
  // find chords and look for 1-4-5

  return {};
}

// chords of degree I, IV, V (+) and Imaj7, V7 (++) increase the relevance of a scale