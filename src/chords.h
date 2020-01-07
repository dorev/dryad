#pragma once

#include "definitions.h"
#include "scales.h"

struct ScaleDegree
{
  Scale scale;
  int degree;
};




struct Chord
{
  std::vector<int> _notes;
  std::set<int> _fundamentalNotes;
  std::vector<ScaleDegree> _scaleDegrees;

  Chord(std::vector<int> notes) : _notes(notes)
  {
    for(int note : _notes)
      _fundamentalNotes.insert(note % 12);

    // for each note
      // for each scale type interval
        // if all notes are possible
          // if we have consecutive triads
            // what degree of that scale are we

  }

};
