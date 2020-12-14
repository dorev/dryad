#pragma once

#include "definitions.h"
#include "pitch.h"

struct ScorePosition
{
  std::set<Pitch> _notes;
  std::set<const Pitch *> _resonatingNotes;
  const ScorePosition *_prev;
  const ScorePosition *_next;
  int _scoreIndex;
  int _measure;

  ScorePosition()
      : _notes({})
      , _resonatingNotes({})
      , _prev(nullptr)
      , _next(nullptr)
      , _scoreIndex(-1)
  {
  }

  bool insert(const Pitch &pitch, int measure, int scoreIndex)
  {
    bool result = _notes.emplace(pitch).second;

    if (result)
    {
      _measure = measure;
      _scoreIndex = scoreIndex;
    }

    return result;
  }

  bool addResonating(const Pitch *pitchPtr)
  {
    return _resonatingNotes.emplace(pitchPtr).second;
  }
};