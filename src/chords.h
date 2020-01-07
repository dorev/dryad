#pragma once

#include "definitions.h"

struct Chord
{
  std::vector<int> _notes;
  std::set<int> _pureNotes;

  Chord(std::vector<int> notes) : _notes(notes)
  {
    for(int note : _notes)
      _pureNotes.insert(note % 12);
  }
};
