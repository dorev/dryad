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
    return _notes.insert(pitch).second;
  }

  bool addResonating(PitchPtr pitchPtr)
  {
    return _resonatingNotes.insert(pitchPtr).second;
  }

  NotePairList findInterval(int semitoneInterval) const
  {
    NotePairList output = {};

    std::set<PitchPtr> allSounds;
    for(auto& note : _notes)
      allSounds.insert(makePitchPtr(note));    

    allSounds.insert(_resonatingNotes.begin(), _resonatingNotes.end());
    
    for(auto& note1 : allSounds)
    {
      for(auto& note2 : allSounds)
      {
        if(note1 == note2)
          continue;

        if(note1->_num - note2->_num == semitoneInterval)
          output.emplace(note1, note2);
      }
    }
    return output;
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