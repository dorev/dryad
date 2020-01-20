#pragma once

#include "definitions.h"

struct Chord
{
  std::set<int> _notes;
  std::set<int> _pureNotes;

  Chord(std::set<int> notes) : _notes(notes)
  {
    for(int note : _notes)
      _pureNotes.insert(note % 12);
  }
};
