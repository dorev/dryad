#pragma once

#include "definitions.h"
#include "pitch.h"

struct ScorePosition 
{  
  std::set<Pitch> notes;
  std::set<const Pitch*> resonatingNotes;
  const ScorePosition* _prev;
  const ScorePosition* _next;
  int _scoreIndex;

  ScorePosition()
    : notes({})
    , resonatingNotes({})
    , _prev(nullptr)
    , _next(nullptr)
    , _scoreIndex(-1)
  {}

  bool insert(const Pitch& pitch)
  {
    return notes.emplace(pitch).second;
  }

  bool addResonating(const Pitch* pitchPtr)
  {
    return resonatingNotes.emplace(pitchPtr).second;
  }

  int measure() const
  {
    if(notes.size() == 0)
      return 0;
    
    // Return measure value of the first note of position
    return notes.begin()->measure;
  }

};