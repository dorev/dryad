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
  int _measure;

  ScorePosition()
    : notes({})
    , resonatingNotes({})
    , _prev(nullptr)
    , _next(nullptr)
    , _scoreIndex(-1)
  {}

  bool insert(const Pitch& pitch, int measure)
  {
    bool result = notes.emplace(pitch).second;

    if(result)
      _measure = measure;

    return result;
  }

  bool addResonating(const Pitch* pitchPtr)
  {
    return resonatingNotes.emplace(pitchPtr).second;
  }

};