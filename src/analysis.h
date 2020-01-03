#pragma once

#include "score.h"

using NotePairList = std::set<std::pair<PitchPtr,PitchPtr>> ;

NotePairList findInterval(int semitoneInterval, const ScorePosition& pos)
{
  NotePairList output = {};

  std::set<PitchPtr> allSounds;
  for(auto& note : pos._notes)
    allSounds.insert(makePitchPtr(note));    

  allSounds.insert(pos._resonatingNotes.begin(), pos._resonatingNotes.end());

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