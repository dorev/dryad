#pragma once

#include "definitions.h"
#include "pitch.h"

struct ScorePosition 
{  
  std::set<Pitch> _notes;
  std::set<const Pitch*> _resonatingNotes;
  const ScorePosition* _prev;
  const ScorePosition* _next;
  int _scoreIndex;

  ScorePosition()
    : _notes({})
    , _resonatingNotes({})
    , _prev(nullptr)
    , _next(nullptr)
    , _scoreIndex(-1)
  {}

  bool insert(const Pitch& pitch)
  {
    return _notes.emplace(pitch).second;
  }

  bool addResonating(const Pitch* pitchPtr)
  {
    return _resonatingNotes.emplace(pitchPtr).second;
  }

  int measure() const
  {
    if(_notes.size() == 0)
      return 0;
    
    // Return measure value of the first note of position
    return _notes.begin()->_measure;
  }

};