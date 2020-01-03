#pragma once

#include "definitions.h"
#include "pitch.h"

using NotePairList = std::set<std::pair<PitchPtr,PitchPtr>> ;

struct ScorePosition 
{  
  std::set<Pitch> _notes;
  std::set<PitchPtr> _resonatingNotes;
  std::shared_ptr<ScorePosition> _prev;
  std::shared_ptr<ScorePosition> _next;
  int _scoreIndex;

  ScorePosition()
    : _notes({})
    , _resonatingNotes({})
    , _prev(nullptr)
    , _next(nullptr)
    , _scoreIndex(-1)
  {}

  bool insert(Pitch& pitch)
  {
    return _notes.emplace(pitch).second;
  }

  bool addResonating(PitchPtr pitchPtr)
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

using ScorePositionPtr = std::shared_ptr<ScorePosition>;