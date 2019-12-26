#pragma once

#include "definitions.h"
#include "pitch.h"

using NotePairList = std::set<std::pair<PitchPtr,PitchPtr>> ;

struct ScorePosition 
{  
  std::set<Pitch> _notes;
  std::shared_ptr<ScorePosition> _prev;
  std::shared_ptr<ScorePosition> _next;

  ScorePosition()
    : _notes({})
    , _prev(nullptr)
    , _next(nullptr)
  {}

  bool insert(Pitch& pitch)
  {    
    return _notes.emplace(pitch).second;
  }

  NotePairList&& findInterval(int semitoneInterval)
  {
    NotePairList output = {};

    for(auto note1 : _notes)
    {
      for(auto note2 : _notes)
      {
        if(note1 == note2)
          continue;

        if(note1._num - note2._num == semitoneInterval)
          output.emplace(makePitchPtr(note1), makePitchPtr(note2));
      }
    }
    return std::move(output);
  }

};

using ScorePositionPtr = std::shared_ptr<ScorePosition>;